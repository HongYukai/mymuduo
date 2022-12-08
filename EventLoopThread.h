//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"
#include "EventLoop.h"
#include "Thread.h"
#include <functional>
#include <mutex>
#include <condition_variable>

class EventLoopThread : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(), const std::string &name = std::string());
    ~EventLoopThread();
    EventLoop* startLoop();
private:
    void threadFunc();
    bool existing_;
    EventLoop *loop_;
    Thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallback callback_;
};
