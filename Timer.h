//
// Created by 12096 on 2022/10/15.
//

#pragma once

#include <time.h>
#include <netinet/in.h>
#include <functional>
#include <memory>
#include <utility>
#include "Channel.h"
#include "EventLoop.h"

class Timer;

using TimeOutCallback = std::function<void ()>;

class Timer {
public:
    explicit Timer(int delay, int interval, TimeOutCallback cb) : expire_(delay + time(NULL)), interval_(interval), repeat_(interval > 0), cb_(std::move(cb)) {}
    bool repeat() const {return repeat_;}
    time_t expiration() const {return expire_;}
    void run() const {if (cb_) cb_();}
    void cancel() {cb_ = nullptr;}
    void restart() {expire_ = time(NULL) + interval_;}
private:
    time_t expire_;  // 定时器过期的绝对时间
    const int interval_;
    const bool repeat_;
    TimeOutCallback cb_;
};

class TimerContainer {
public:
    int cur_;  // 现有定时器数
    int capacity_;  // 最大定时器数
public:
    TimerContainer() = default;
    virtual ~TimerContainer() = default;
    virtual void addTimer(std::shared_ptr<Timer> timer) = 0;
    virtual void deleteTimer(std::shared_ptr<Timer> timer) = 0;
    virtual std::shared_ptr<Timer> popTimer() = 0;
    virtual void tick() = 0;

};
