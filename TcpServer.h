<<<<<<< HEAD
//
// Created by 12096 on 2022/9/13.
//

#pragma once
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "noncopyable.h"
#include "Callbacks.h"
#include "TcpConnection.h"
#include <memory>
#include <functional>
#include <atomic>
#include <unordered_map>

class TcpServer : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    enum Option {
        kNoReusePort,
        kReusePort,
    };
    TcpServer(EventLoop* loop, const InetAddress &listenAddr, const std::string &name, Option option = kNoReusePort);
    ~TcpServer();
    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback &cb) {threadInitCallback_ = cb;}
    void start();
    void setConnectionCallback(const ConnectionCallback &cb) {connectionCallback_ = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) {writeCompleteCallback_ = cb;}
    void setMessageCallback(const MessageCallback &cb) {messageCallback_ = cb;}
    std::string name();
    std::string ipPort();
private:
    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);
    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;
    EventLoop *loop_;  // baseloop
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;  // 有新连接时的回调
    MessageCallback messageCallback_;  // 有读写消息的回调
    WriteCompleteCallback writeCompleteCallback_;  // 消息发送完成的回调
    ThreadInitCallback threadInitCallback_;  // loop线程初始化的回调
    std::atomic_int started_;
    int nextConnId_;
    ConnectionMap connections_;
};
=======
//
// Created by 12096 on 2022/9/13.
//

#pragma once
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "noncopyable.h"
#include "Callbacks.h"
#include "TcpConnection.h"
#include <memory>
#include <functional>
#include <atomic>
#include <unordered_map>

class TcpServer : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    enum Option {
        kNoReusePort,
        kReusePort,
    };
    TcpServer(EventLoop* loop, const InetAddress &listenAddr, const std::string &name, Option option = kNoReusePort);
    ~TcpServer();
    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback &cb) {threadInitCallback_ = cb;}
    void start();
    void setConnectionCallback(const ConnectionCallback &cb) {connectionCallback_ = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) {writeCompleteCallback_ = cb;}
    void setMessageCallback(const MessageCallback &cb) {messageCallback_ = cb;}
private:
    void newConnection(int sockfd, const InetAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);
    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;
    EventLoop *loop_;  // baseloop
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;  // 有新连接时的回调
    MessageCallback messageCallback_;  // 有读写消息的回调
    WriteCompleteCallback writeCompleteCallback_;  // 消息发送完成的回调
    ThreadInitCallback threadInitCallback_;  // loop线程初始化的回调
    std::atomic_int started_;
    int nextConnId_;
    ConnectionMap connections_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
