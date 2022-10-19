//
// Created by 12096 on 2022/9/13.
//
#include "TcpConnection.h"
#include "Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>


EventLoop* checkLoopNotNull(EventLoop *loop) {
    if (loop == nullptr) {
        LOG_FATAL("%s mainloop is null!\n", __FUNCTION__ );
    }
    return loop;
}

TcpConnection::TcpConnection(EventLoop *loop,
                             const std::string &name,
                             int sockfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr)
    : loop_(checkLoopNotNull(loop))
    , name_(name)
    , socket_(new Socket(sockfd))
    , localAddr_(localAddr)
    , peerAddr_(peerAddr)
    , state_(kConnecting)
    , reading_(true)
    , channel_(new Channel(loop, sockfd))
    , highWaterMark_(64*1024*1024)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    LOG_INFO("Tcp connection::ctor[%s] at fd=%d\n", name_.c_str(), sockfd);
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection() {
    LOG_INFO("Tcp connection::dtor[%s] at fd=%d state=%d\n", name_.c_str(), channel_->getFd(), state_.load());
}

void TcpConnection::handleRead(TimeStamp receiveTime) {
    int saveErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->getFd(), &saveErrno);
    if (n > 0) {  // 有可读事件发生
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if (n == 0) {   // 客户端断开
        handleClose();
    }
    else {
        errno = saveErrno;
        LOG_ERROR("TcpConnection::handleRead");
        handleError();
    }
}

void TcpConnection::handleWrite() {
    if (channel_->isWriting()) {  // fd对写事件感兴趣
        int saveErrno = 0;
        ssize_t n = outputBuffer_.writeFd(channel_->getFd(), &saveErrno);
        if (n > 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if (state_ == kDisconnecting) {
                    shutdown();
                }
            }
        }
        else {
            LOG_ERROR("TcpConnection::handleWrite");
        }
    }
    else {
        LOG_ERROR("TcpConnection fd=%d is down, no more writing", channel_->getFd());
    }
}

void TcpConnection::setState(int op) {
    state_ = op;
}

void TcpConnection::handleClose() {
    LOG_INFO("fd=%d state=%d\n", channel_->getFd(), state_.load());
    setState(kDisconnected);
    channel_->disableAll();
    TcpConnectionPtr connPtr(shared_from_this());
    connectionCallback_(connPtr);
    closeCallback_(connPtr);
}

void TcpConnection::handleError() {
    int optval;
    int err = 0;
    socklen_t optlen = sizeof(optval);
    if (::getsockopt(channel_->getFd(), SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
        err = errno;
    }
    else {
        err = optval;
    }
    LOG_ERROR("TcpConnection::handleError name:%s - SO_ERROR:%d\n", name_.c_str(), err);
}


/**
 * 发送数据  应用写的快， 而内核发送数据慢， 需要把待发送数据写入缓冲区， 而且设置了水位回调
 */
void TcpConnection::sendInLoop(const void *message, size_t len) {
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if (state_ == kDisconnected) {
        LOG_ERROR("disconnected, give up writing");
        return;
    }
    // channel write data for the first time, and there is no data to be sent within the buffer
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        nwrote = ::write(channel_->getFd(), message, len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
            if (remaining == 0 && writeCompleteCallback_)  {
                // since that the total data here all get sent, it is not necessary to set epollout event to the channel
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
            else {

            }
        }
        else {
            nwrote = 0;
            if (errno != EWOULDBLOCK) {
                LOG_ERROR("TcpConnection::sendInLoop");
                if (errno == EPIPE || errno == ECONNRESET) {
                    faultError = true;
                }
            }
        }
    }
    // it shows that the write operation did not send out all the data, the remaining data should be saved in the buffer
    // and then, register the epollout event of the channel. When poller discover the free space of tcp buffer, the relevant
    // sock-channel will be informed and call the handleWrite cb
    if (!faultError && remaining > 0) {
        size_t oldlen = outputBuffer_.readableBytes();
        if (oldlen + remaining >= highWaterMark_ && oldlen < highWaterMark_ && highMarkCallback_) {
            loop_->queueInLoop(std::bind(highMarkCallback_, shared_from_this(), oldlen + remaining));
        }
        outputBuffer_.append((char*)message + nwrote, remaining);
        if (!channel_->isWriting()) {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::send(const std::string &buf) {
    if (state_ == kConnected) {
        if (loop_->isInLoopThread()) {
            sendInLoop(buf.c_str(), buf.size());
        }
        else {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, buf.c_str(), buf.size()));
        }
    }
}

void TcpConnection::connectEstablished() {
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
    if (state_ == kConnected) {
        setState(kDisconnected);
        channel_->disableAll();
        connectionCallback_(shared_from_this());
    }
    channel_->remove();
}

void TcpConnection::shutdown() {
    if (state_ == kConnected) {
        setState(kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop() {
    if (!channel_->isWriting()) {  // data in outbuffer has been totally sent
        socket_->shutdownWrite();
    }
}