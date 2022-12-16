//
// Created by 12096 on 2022/10/15.
//
#include "TimerHeap.h"
#include "Clock.h"
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

struct timespec howMuchTimeFromNow(int64_t when)
{
    int64_t microseconds = when - TimeStamp::now();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
            microseconds / (TimeStamp::kMicroSecondsPerSecond));
    ts.tv_nsec = static_cast<long>(
            (microseconds % (TimeStamp::kMicroSecondsPerSecond)) * 1000);
    return ts;
}

void resetTimerfd(int timerfd, int64_t expiration)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue, 0, sizeof newValue);
    memset(&oldValue, 0, sizeof oldValue);

    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_ERROR("timerfd_settime()");
    }
}



void TimerHeap::addTimer(std::shared_ptr<Timer> timer) {
    if (!timer) return;
    bool earliestChanged = push(timer);

    if (earliestChanged) {
        resetTimerfd(timerfd_, timer->expiration());
    }

}

std::shared_ptr<Timer> TimerHeap::popTimer() {
    if (timerHeap_.empty()) return nullptr;
    auto top = std::move(timerHeap_.top());
    timerHeap_.pop();
    return top;
}

void TimerHeap::tick() {
    int64_t t = TimeStamp::now();
    while (!timerHeap_.empty()) {
        if (t >= timerHeap_.top()->expiration()) {
            expired_.emplace_back(std::move(timerHeap_.top()));
            timerHeap_.pop();
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

void TimerHeap::addTimerInLoop(double delay, bool repeat, TimeOutCallback cb) {
    loop_->runInLoop(std::bind(&TimerHeap::addTimer, this, std::make_shared<Timer>(delay, repeat, std::move(cb))));
}

void TimerHeap::handleRead() {
    auto now = TimeStamp::now();
    readTimerfd(timerfd_);
    tick();
    for (auto &timer : expired_) {
        if (timer) {
            struct timeval t;
            gettimeofday(&t, NULL);
            timeDelay_.first += (1000 * 1000) * t.tv_sec + t.tv_usec - timer->expiration();
            timeDelay_.second++;
            timer->run();
        }
    }
    reset(expired_, now);
}


void TimerHeap::reset(const std::vector<std::shared_ptr<Timer>> &expired, int64_t now) {
    for (auto & timer : expired) {
        if (timer) {
            if (timer->repeat()) {
                timer->restart(now);
                push(timer);
            }
        }
    }
    expired_.clear();
    if (!timerHeap_.empty()) {
        resetTimerfd(timerfd_, timerHeap_.top()->expiration());
    }
}


TimerHeap::~TimerHeap() {
    timerfdChannel_.disableAll();
    timerfdChannel_.remove();
    ::close(timerfd_);
    std::cout << timeDelay_.second << std::endl;
    std::cout << static_cast<double>(timeDelay_.first) / (1000 * 1000) /  timeDelay_.second << std::endl;
}

bool TimerHeap::push(std::shared_ptr<Timer> timer) {
    timerHeap_.push(timer);
    if (timerHeap_.top() != timer) {
        return false;
    }
    return true;
}
