//
// Created by 12096 on 2022/10/15.
//

#pragma once

#include <time.h>
#include <netinet/in.h>
#include <functional>
#include <memory>

class Timer;

struct client_data {
    sockaddr_in address_;
    int sock_fd_;
    std::weak_ptr<Timer> timer_;
    client_data(sockaddr_in address, int sock_fd) : address_(address), sock_fd_(sock_fd) {}
};

struct Timer {
    time_t expire_;  // 定时器过期的绝对时间
    std::shared_ptr<client_data> user_data_;
    explicit Timer(int delay) : expire_(delay + time(NULL)) {}
    using TimeOutCallback = std::function<void (std::shared_ptr<client_data>)>;
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
    virtual void popTimer() = 0;
    virtual void tick() = 0;
};
