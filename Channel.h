//
// Created by 12096 on 2022/9/9.
//

#pragma once

#include "noncopyable.h"
#include "Timestamp.h"
#include <functional>
#include <memory>


class EventLoop;

/*
 * 封装了sockfd和其感兴趣的events，以及内核返回的实际发生的events，one channel one fd
 */
class Channel : noncopyable{
public:
    using EventCallback =  std::function<void()>;
    using ReadEventCallback =  std::function<void(TimeStamp)>;
    Channel(EventLoop *loop, int fd);
    ~Channel();
    void handleEvent(TimeStamp receiveTime);  // 调用回调处理事件

    // 设置回调函数
    void setReadCallback(ReadEventCallback cb) {
        readCallback_ = std::move(cb);
    }
    void setWriteCallback(EventCallback cb) {
        writeCallback_ = std::move(cb);
    }
    void setCloseCallback(EventCallback cb) {
        closeCallback_ = std::move(cb);
    }
    void setErrorCallback(EventCallback cb) {
        errorCallback_ = std::move(cb);
    }

    // 防止Channel被手动remove后依然执行回调
    void tie(const std::shared_ptr<void>&);
    int getFd() const {return fd_;}
    int getEvents() const {return events_;}
    void set_revents(int revt) {revents_ = revt;}
    bool isNoneEvent() const {return events_ == kNoneEvent;}
    void enableReading() {events_ |= kReadEvent;update();}  // 添加读事件并调用epoll_ctl进行实际添加
    void disableReading() {events_ &= ~kReadEvent;update();}
    void enableWriting() {events_ |= kWriteEvent;update();}  // 添加写事件并调用epoll_ctl进行实际添加
    void disableWriting() {events_ &= ~kWriteEvent;update();}
    void disableAll() {events_ = kNoneEvent; update();}
    bool isWriting() const {return events_ & kWriteEvent;}
    bool isReading() const {return events_ & kReadEvent;}
    int getIndex() const {return index_;}
    void setIndex(int idx) {index_ = idx;}
    EventLoop* ownerLoop() const {return loop_;}
    void remove();

private:
    void update();
    void handleEventWithGuard(TimeStamp receiveTime);
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    EventLoop *loop_;  // 事件循环
    const int fd_;  //  Poller监听的对象
    int events_;  // 注册fd感兴趣的事件
    int revents_;  // Poller返回的事件
    int index_;
    std::weak_ptr<void> tie_;
    bool tied_;

    // Channel负责调用具体revents发生后的回调函数
    ReadEventCallback readCallback_;
    EventCallback  writeCallback_;
    EventCallback  closeCallback_;
    EventCallback  errorCallback_;
};
