<<<<<<< HEAD
//
// Created by 12096 on 2022/9/13.
//

#include "Acceptor.h"
#include "Logger.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int createNonBlocking() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        LOG_FATAL("%s: listen socket create error\n", __FUNCTION__ );
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool request)
    : loop_(loop)
    , acceptSocket_(createNonBlocking())
    , acceptChannel_(loop, acceptSocket_.fd())
    , listening_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::handleRead() {
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0) {
        if (newConnectinCallback_) {
            newConnectinCallback_(connfd, peerAddr);
        }
        else {
            close(connfd);
            LOG_ERROR("%s: accept error %d", __FUNCTION__, errno );
            if (errno == EMFILE) {
                LOG_ERROR("%s: socke reached limit", __FUNCTION__);
            }
        }
    }
}

void Acceptor::listen() {
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

=======
//
// Created by 12096 on 2022/9/13.
//

#include "Acceptor.h"
#include "Logger.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int createNonBlocking() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        LOG_FATAL("%s: listen socket create error\n", __FUNCTION__ );
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool request)
    : loop_(loop)
    , acceptSocket_(createNonBlocking())
    , acceptChannel_(loop, acceptSocket_.fd())
    , listening_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::handleRead() {
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0) {
        if (newConnectinCallback_) {
            newConnectinCallback_(connfd, peerAddr);
        }
        else {
            close(connfd);
            LOG_ERROR("%s: accept error %d", __FUNCTION__, errno );
            if (errno == EMFILE) {
                LOG_ERROR("%s: socke reached limit", __FUNCTION__);
            }
        }
    }
}

void Acceptor::listen() {
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
