//
// Created by 12096 on 2022/10/15.
//

#pragma once

#include "Timer.h"
#include "Logger.h"
#include <sys/timerfd.h>
#include <vector>
#include <queue>

int createTimerfd();

struct cmp {
    bool operator() (const std::shared_ptr<Timer>& t1, const std::shared_ptr<Timer>& t2) {
        return t1->expiration() > t2->expiration();
    }
};

class TimerHeap : public TimerContainer {
private:
    std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer>>, cmp> timerHeap_;
    // std::vector<std::shared_ptr<Timer>> timerVec_;
public:
    explicit TimerHeap(int cap, EventLoop *loop) : loop_(loop), timerfd_(createTimerfd()), timerfdChannel_(loop, timerfd_)  {
        capacity_ = cap;
        cur_ = 0;
        timerfdChannel_.setReadCallback(std::bind(&TimerHeap::handleRead, this));
        timerfdChannel_.enableReading();
    }
    void addTimerInLoop(int delay, double interval, TimeOutCallback cb);
    ~TimerHeap() override;

private:
    void addTimer(std::shared_ptr<Timer> timer) override;
    void deleteTimer(std::shared_ptr<Timer> timer) override;
    std::shared_ptr<Timer> popTimer() override;
    void tick() override;
    void handleRead();
    void reset(const std::vector<std::shared_ptr<Timer>>& expired);
    std::vector<std::shared_ptr<Timer>> expired_;
    int timerfd_;
    Channel timerfdChannel_;
    EventLoop *loop_;


};
