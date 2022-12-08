<<<<<<< HEAD
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"

class InetAddress;

class Socket : noncopyable {
public:
    explicit Socket(int sockfd) : sockfd_(sockfd) {}
    ~Socket();
    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);
    void shutdownWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int sockfd_;
};
=======
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"

class InetAddress;

class Socket : noncopyable {
public:
    explicit Socket(int sockfd) : sockfd_(sockfd) {}
    ~Socket();
    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);
    void shutdownWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int sockfd_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
