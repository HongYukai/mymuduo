//
// Created by 12096 on 2022/9/12.
//

#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop, const std::string &name)
    : baseloop_(baseloop)
    , name_(name)
    , started_(false)
    , numThreads_(0)
    , next_(0)
{

}

void EventLoopThreadPool::start(const ThreadInitCallback &cb) {
    started_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        std::string t_name = name_ + std::to_string(i);
        EventLoopThread *t = new EventLoopThread(cb, t_name);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
    if (numThreads_ == 0 && cb) {
        cb(baseloop_);
    }
}

std::vector<EventLoop *> EventLoopThreadPool::getAllLoops() {
    if (loops_.empty()) {
        return std::vector<EventLoop*>(1, baseloop_);
    }
    return loops_;
}

EventLoop *EventLoopThreadPool::getNextLoop() {
    EventLoop *loop = baseloop_;
    if (!loops_.empty()) {
        loop = loops_[next_];
        ++next_;
        if (next_ >= loops_.size()) {
            next_ = 0;
        }
    }
    return loop;
}

EventLoopThreadPool::~EventLoopThreadPool() = default;
