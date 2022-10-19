//
// Created by 12096 on 2022/10/15.
//
#include "TimerHeap.h"
#include <string.h>
#include <unistd.h>

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_ERROR("Failed in timerfd_create");
    }
    return timerfd;
}

void readTimerfd(int timerfd) {
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    if (n != sizeof howmany)
    {
        LOG_ERROR("TimerQueue::handleRead() reads %zd bytes instead of 8", n);
    }
}

struct timespec howMuchTimeFromNow(time_t when)
{
    int64_t microseconds = 100000 * (when
                           - time(NULL));
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
            microseconds / (100000));
    ts.tv_nsec = static_cast<long>(
            (microseconds % 100000) * 1000);
    return ts;
}

void resetTimerfd(int timerfd, time_t expiration)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    struct timespec t;
    memset(&newValue, 0, sizeof newValue);
    memset(&oldValue, 0, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);;
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_ERROR("timerfd_settime()");
    }
}



void TimerHeap::addTimer(std::shared_ptr<Timer> timer) {
    if (!timer || cur_ >= capacity_) return;
    timerHeap_.push(timer);
    cur_++;
    resetTimerfd(timerfd_, timer->expiration());
}

std::shared_ptr<Timer> TimerHeap::popTimer() {
    if (cur_ == 0) return nullptr;
    auto top = timerHeap_.top();
    timerHeap_.pop();
    cur_--;
    return top;
}

void TimerHeap::tick() {
    time_t t = time( NULL );
    while (cur_ > 0) {
        auto temp = timerHeap_.top().get();
        if (!temp) return;
        if (t >= temp->expiration()) {
            expired_.push_back(popTimer());
        }
        else {
            break;
        }
    }
}

void TimerHeap::deleteTimer(std::shared_ptr<Timer> timer) {
    if (!timer) return;
    timer->cancel();
}

void TimerHeap::addTimerInLoop(int delay, double interval, TimeOutCallback cb) {
    loop_->runInLoop(std::bind(&TimerHeap::addTimer, this, std::make_shared<Timer>(delay, interval, std::move(cb))));
}

void TimerHeap::handleRead() {
    readTimerfd(timerfd_);
    tick();
    for (auto &timer : expired_) {
        if (timer) {
            timer->run();
        }
    }
    reset(expired_);
}


void TimerHeap::reset(const std::vector<std::shared_ptr<Timer>> &expired) {
    for (auto & timer : expired) {
        if (timer) {
            if (timer->repeat()) {
                timer->restart();
                addTimer(timer);
            }
        }
    }
    expired_.clear();
}


TimerHeap::~TimerHeap() = default;

