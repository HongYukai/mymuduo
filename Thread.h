<<<<<<< HEAD
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"
#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <atomic>

class Thread : noncopyable {
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc func, const std::string &name = std::string());
    ~Thread();
    void start();
    void join();
    bool started() const {return started_;}
    pid_t tid() const {return tid_;}
    const std::string & name() const {return name_;}
    void setDefaultName();
    static int numCreated() {return numCreated_;}
private:
    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    static std::atomic_int32_t numCreated_;
=======
//
// Created by 12096 on 2022/9/12.
//

#pragma once

#include "noncopyable.h"
#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <atomic>

class Thread : noncopyable {
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc func, const std::string &name = std::string());
    ~Thread();
    void start();
    void join();
    bool started() const {return started_;}
    pid_t tid() const {return tid_;}
    const std::string & name() const {return name_;}
    void setDefaultName();
    static int numCreated() {return numCreated_;}
private:
    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    static std::atomic_int32_t numCreated_;
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
};