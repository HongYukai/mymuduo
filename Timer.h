<<<<<<< HEAD
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
#include "Timestamp.h"

class Timer;

using TimeOutCallback = std::function<void ()>;

class Timer {
public:
    explicit Timer(double delay, bool repeat, TimeOutCallback cb) : expire_(static_cast<int64_t>(delay * 1000 * 1000) + TimeStamp::now()), interval_(static_cast<int64_t>(delay * 1000 * 1000)), repeat_(repeat), cb_(std::move(cb)) {}
    bool repeat() const {return repeat_;}
    int64_t expiration() const {return expire_;}
    void run() const {if (cb_) cb_();}
    void cancel() {cb_ = nullptr;}
    void restart(int64_t now) {expire_ = now + interval_;}
private:
    int64_t expire_;  // 定时器过期的绝对时间
    const int64_t interval_;
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
=======
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
#include "Timestamp.h"

class Timer;

using TimeOutCallback = std::function<void ()>;

class Timer {
public:
    explicit Timer(double delay, bool repeat, TimeOutCallback cb) : expire_(static_cast<int64_t>(delay * 1000 * 1000) + TimeStamp::now()), interval_(static_cast<int64_t>(delay * 1000 * 1000)), repeat_(repeat), cb_(std::move(cb)) {}
    bool repeat() const {return repeat_;}
    int64_t expiration() const {return expire_;}
    void run() const {if (cb_) cb_();}
    void cancel() {cb_ = nullptr;}
    void restart(int64_t now) {expire_ = now + interval_;}
private:
    int64_t expire_;  // 定时器过期的绝对时间
    const int64_t interval_;
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
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
