<<<<<<< HEAD
//
// Created by 12096 on 2022/9/13.
//

#pragma once

#include "noncopyable.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "Timestamp.h"
#include "HttpContext.h"
#include <memory>
#include <atomic>

class Channel;
class EventLoop;
class Socket;

/*
 * Acceptor将新连接信息返回给TcpConnection, TcpConnection设置回调并将channel注册到poller上
 * Acceptor->mainloop, TcpConnection->subloop
 */
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop *loop, const std::string &name, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr);
    ~TcpConnection();
    EventLoop* getLoop() const {return loop_;}
    const std::string& name() const {return name_;}
    const InetAddress& localAddress() const {return localAddr_;}
    const InetAddress& peerAddress() const {return peerAddr_;}
    bool connected() const {return state_ == kConnected;}
    void send(const std::string &buf);
    void shutdown();
    void shutdownInLoop();
    void setConnectionCallback(const ConnectionCallback &cb) {connectionCallback_ = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) {writeCompleteCallback_ = cb;}
    void setMessageCallback(const MessageCallback &cb) {messageCallback_ = cb;}
    void setCloseCallback(const CloseCallback &cb) {closeCallback_ = cb;}
    void setHighMarkCallback(const HighMarkCallback &cb, size_t highWaterMark) {highMarkCallback_ = cb; highWaterMark_ = highWaterMark;}
    void connectEstablished();
    void connectDestroyed();
    void handleRead(TimeStamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void setState(int op);
    void sendInLoop(const void* message, size_t len);
    void setContext(HttpContext context);
    HttpContext* getContext();
private:
    enum state {
        kDisconnected, kConnecting, kConnected, kDisconnecting,
    };
    EventLoop *loop_;  // 绝对不是baseloop
    const std::string name_;
    std::atomic_int state_{0};
    bool reading_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;  // 有新连接时的回调
    MessageCallback messageCallback_;  // 有读写消息的回调
    WriteCompleteCallback writeCompleteCallback_;  // 消息发送完成的回调
    CloseCallback closeCallback_;
    HighMarkCallback highMarkCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    HttpContext context_;
};
=======
//
// Created by 12096 on 2022/9/13.
//

#pragma once

#include "noncopyable.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "Timestamp.h"
#include <memory>
#include <atomic>

class Channel;
class EventLoop;
class Socket;

/*
 * Acceptor将新连接信息返回给TcpConnection, TcpConnection设置回调并将channel注册到poller上
 * Acceptor->mainloop, TcpConnection->subloop
 */
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop *loop, const std::string &name, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr);
    ~TcpConnection();
    EventLoop* getLoop() const {return loop_;}
    const std::string& name() const {return name_;}
    const InetAddress& localAddress() const {return localAddr_;}
    const InetAddress& peerAddress() const {return peerAddr_;}
    bool connected() const {return state_ == kConnected;}
    void send(const std::string &buf);
    void shutdown();
    void shutdownInLoop();
    void setConnectionCallback(const ConnectionCallback &cb) {connectionCallback_ = cb;}
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) {writeCompleteCallback_ = cb;}
    void setMessageCallback(const MessageCallback &cb) {messageCallback_ = cb;}
    void setCloseCallback(const CloseCallback &cb) {closeCallback_ = cb;}
    void setHighMarkCallback(const HighMarkCallback &cb, size_t highWaterMark) {highMarkCallback_ = cb; highWaterMark_ = highWaterMark;}
    void connectEstablished();
    void connectDestroyed();
    void handleRead(TimeStamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void setState(int op);
    void sendInLoop(const void* message, size_t len);
private:
    enum state {
        kDisconnected, kConnecting, kConnected, kDisconnecting,
    };
    EventLoop *loop_;  // 绝对不是baseloop
    const std::string name_;
    std::atomic_int state_{0};
    bool reading_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;  // 有新连接时的回调
    MessageCallback messageCallback_;  // 有读写消息的回调
    WriteCompleteCallback writeCompleteCallback_;  // 消息发送完成的回调
    CloseCallback closeCallback_;
    HighMarkCallback highMarkCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
