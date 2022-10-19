//
// Created by 12096 on 2022/9/12.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

std::atomic_int32_t Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false)
    , joined_(false)
    , tid_(0)
    , func_(func)
    , name_(name)
{
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();
    }
}

void Thread::start() {
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    thread_ = std::make_shared<std::thread>(std::thread([&](){  // 已进入新线程作用域
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        func_();
    }));
    sem_wait(&sem);
}

void Thread::join() {
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {
        name_ = "Thread" + std::to_string(num);
    }
}
