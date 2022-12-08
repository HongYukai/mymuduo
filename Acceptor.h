<<<<<<< HEAD
//
// Created by 12096 on 2022/9/12.
//

#pragma once
#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"
#include <functional>

class EventLoop;
class InetAddress;

class Acceptor : noncopyable {
public:
    using NewConnectinCallback = std::function<void(int sockfd, const InetAddress)>;
    Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool request);
    ~Acceptor();
    void setNewConnectinCallback(const NewConnectinCallback &cb) {
        newConnectinCallback_ = cb;
    }

    bool listening() const {return listening_;}
    void listen();
private:
    void handleRead();
    EventLoop *loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectinCallback newConnectinCallback_;
    bool listening_;
};
=======
//
// Created by 12096 on 2022/9/12.
//

#pragma once
#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"
#include <functional>

class EventLoop;
class InetAddress;

class Acceptor : noncopyable {
public:
    using NewConnectinCallback = std::function<void(int sockfd, const InetAddress)>;
    Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool request);
    ~Acceptor();
    void setNewConnectinCallback(const NewConnectinCallback &cb) {
        newConnectinCallback_ = cb;
    }

    bool listening() const {return listening_;}
    void listen();
private:
    void handleRead();
    EventLoop *loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectinCallback newConnectinCallback_;
    bool listening_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
