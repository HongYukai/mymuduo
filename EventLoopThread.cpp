//
// Created by 12096 on 2022/9/12.
//
#include "EventLoopThread.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb, const std::string &name)
    :loop_(nullptr)
    , existing_(false)
    , thread_(std::bind(&EventLoopThread::threadFunc, this), name)
{
}

EventLoopThread::~EventLoopThread() {
    existing_ = true;
    if (loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop *EventLoopThread::startLoop() {
    thread_.start();
    EventLoop *loop = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == nullptr) {
            cond_.wait(lock);
        }
        loop = loop_;
    }
    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;  // 创建一个eventloop，与以上线程一一对应
    if (callback_) {
        callback_(&loop);
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop();  // poller->poll
    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}