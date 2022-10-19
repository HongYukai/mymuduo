//
// Created by 12096 on 2022/9/13.
//

#include "TcpServer.h"
#include "TcpConnection.h"
#include "Logger.h"
#include <string.h>

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &name, Option option)
    : loop_(loop)
    , ipPort_(listenAddr.toIpPort())
    , name_(name)
    , acceptor_(new Acceptor(loop, listenAddr, option == kNoReusePort))
    , threadPool_(new EventLoopThreadPool(loop, name_))
    , connectionCallback_()
    , messageCallback_()
    , nextConnId_(1)
    , started_(0)
{
    acceptor_->setNewConnectinCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer() {
    for (auto & item : connections_) {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

// when new connection comes, acceptor will call this function
void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr) {
    // choose a subloop to manage the channel
    EventLoop *ioLoop = threadPool_->getNextLoop();
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_++);
    std::string connName = name_ + buf;
    LOG_INFO("TcpServer::newConnection [%s] - new connection[%s] from %s\n", name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());
    sockaddr_in local;
    memset(&local, 0, sizeof(local));
    socklen_t addrlen = sizeof(local);
    if (::getsockname(sockfd, (sockaddr*)&local, &addrlen) < 0) {
        LOG_ERROR("sockets::getLocalAddr");
    }
    const InetAddress localAddr(local);
    TcpConnectionPtr conn(new TcpConnection(
            ioLoop,
            connName,
            sockfd,
            localAddr,
            peerAddr));
    connections_[connName] = conn;
    // user->tcpserver->tcpconnection->channel->poller->notify channel
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::setThreadNum(int numThreads) {
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start() {
    if (started_++ == 0) {  // 防止被start多次
        threadPool_->start();
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn) {
    LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s\n", name_.c_str(), conn->name().c_str());
    connections_.erase(conn->name());
    EventLoop *loop = conn->getLoop();
    loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}


