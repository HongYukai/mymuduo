//
// Created by 12096 on 2022/10/15.
//

#pragma once

#include "Timer.h"
#include <vector>
#include <queue>

struct cmp {
    bool operator() (const std::shared_ptr<Timer>& t1, const std::shared_ptr<Timer>& t2) {
        return t1->expire_ > t2->expire_;
    }
};

class HeapTimer : public TimerContainer {
private:
    std::priority_queue<std::shared_ptr<Timer>, std::vector<std::shared_ptr<Timer>>, cmp> timerHeap_;
    // std::vector<std::shared_ptr<Timer>> timerVec_;
public:
    explicit HeapTimer(int cap)  {
        capacity_ = cap;
        cur_ = 0;
    }
    void addTimer(std::shared_ptr<Timer> timer) override;
    void deleteTimer(std::shared_ptr<Timer> timer) override;
    void popTimer() override;
    void tick() override;
    ~HeapTimer() override;
};
