//
// Created by 12096 on 2022/10/15.
//

#pragma once

#include "Timer.h"
#include "Logger.h"
#include "four_heap.h"
#include <sys/timerfd.h>
#include <vector>
#include <queue>
#include <sys/time.h>

int createTimerfd();

struct cmp {
    bool operator() (const std::shared_ptr<Timer>& t1, const std::shared_ptr<Timer>& t2) {
        return t1->expiration() > t2->expiration();
    }
};

class TimerHeap : public TimerContainer {
private:
//    Four_heap<std::shared_ptr<Timer>, cmp> timerHeap_;
    std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer>>, cmp> timerHeap_;
    // std::vector<std::shared_ptr<Timer>> timerVec_;
public:
    explicit TimerHeap(EventLoop *loop) : loop_(loop), timerfd_(createTimerfd()), timerfdChannel_(loop, timerfd_)  {
        timerfdChannel_.setReadCallback(std::bind(&TimerHeap::handleRead, this));
        timerfdChannel_.enableReading();
        timeDelay_.first = timeDelay_.second = 0;
    }
    void addTimerInLoop(double delay, bool repeat, TimeOutCallback cb);
    ~TimerHeap() override;

private:
    void addTimer(std::shared_ptr<Timer> timer) override;
    void deleteTimer(std::shared_ptr<Timer> timer) override;
    std::shared_ptr<Timer> popTimer() override;
    void tick() override;
    void handleRead();
    bool push(std::shared_ptr<Timer> timer);
    void reset(const std::vector<std::shared_ptr<Timer>>& expired, int64_t now);
    std::vector<std::shared_ptr<Timer>> expired_;
    int timerfd_;
    Channel timerfdChannel_;
    EventLoop *loop_;
    pair<int64_t , int> timeDelay_;
};
