<<<<<<< HEAD
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"
#include <functional>
#include <vector>
#include <memory>
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    EventLoopThreadPool(EventLoop *baseloop, const std::string &name);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) {numThreads_ = numThreads;}
    void start(const ThreadInitCallback &cb = ThreadInitCallback());
    // baseloop will allocate channels to loop by the way of round-robin
    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();
private:
    EventLoop *baseloop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};
=======
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"
#include <functional>
#include <vector>
#include <memory>
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    EventLoopThreadPool(EventLoop *baseloop, const std::string &name);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) {numThreads_ = numThreads;}
    void start(const ThreadInitCallback &cb = ThreadInitCallback());
    // baseloop will allocate channels to loop by the way of round-robin
    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();
private:
    EventLoop *baseloop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
