//
// Created by 12096 on 2022/10/15.
//
#include "HeapTimer.h"

void HeapTimer::addTimer(std::shared_ptr<Timer> timer) {
    if (!timer || cur_ >= capacity_) return;
    timerHeap_.push(std::move(timer));
    cur_++;
}

void HeapTimer::popTimer() {
    if (cur_ == 0) return;
    timerHeap_.pop();
    cur_--;
}

void HeapTimer::tick() {
    time_t t = time( NULL );
    while (cur_ > 0) {
        auto temp = timerHeap_.top().get();
        if (!temp) return;
        if (t >= temp->expire_) {
            if (temp->cb_) {
                temp->cb_(temp->user_data_);
            }
            popTimer();
        }
        else {
            break;
        }
    }
}

void HeapTimer::deleteTimer(std::shared_ptr<Timer> timer) {
    if (!timer) return;
    timer->cb_ = nullptr;
}

HeapTimer::~HeapTimer() = default;

