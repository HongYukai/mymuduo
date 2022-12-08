<<<<<<< HEAD
//
// Created by 12096 on 2022/9/9.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Logger.h"
#include "Poller.h"
#include "TimerHeap.h"
#include <sys/eventfd.h>
#include <fcntl.h>
#include <unistd.h>

// 一个线程只能创建一个Eventloop
__thread EventLoop *t_loopInThisThread = nullptr;

// 定义默认的Poller IO复用接口的超时时间
const int kPollTimeMs = 10000;

// 创建eventfd, 用于唤醒working loop处理新来的channel
int createEventFd() {
    int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evfd < 0) {
        LOG_FATAL("eventfd error: %d\n", errno);
    }
    return evfd;
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , callingPendingFunctors_(false)
    , threadId_(CurrentThread::tid())
    , poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(createEventFd())
    , wakeupChannel_(new Channel(this, wakeupFd_))
    , currentActiveChannel_(nullptr)
    , timer_(new TimerHeap(this))
{
    LOG_INFO("EventLoop %p created in thread %d\n", this, threadId_);
    if (t_loopInThisThread) {
        LOG_FATAL("Another EventLoop %p exists in this thread %d\n", t_loopInThisThread, threadId_);
    }
    else {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));  // ???
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::handleRead() {   // 唤醒working loop后执行的读回调
    uint64_t one = 1;
    auto n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR("EventLoop::handleRead() reads %ld bytes instead of 8", n);
    }
}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;
    LOG_INFO("EventLoop %p start looping \n", this);
    while (!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for (auto channel : activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        doPendingFunctors();
    }
}

// 1. 在线程中调用quit方法quit自己  2. 在线程中调用quit方法quit其他线程
void EventLoop::quit() {
    quit_ = true;
    // 在某线程中quit其他线程，则需要唤醒其他线程，防止该线程阻塞
    if (!isInLoopThread()) {
        wakeUp();
    }
//    LOG_INFO("loop %p has quited", this);
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) {
        cb();
    }
    else {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb) {
    {
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁原因：可能base线程操作pendingFunctors_时working线程也在操作
        pendingFunctors_.push_back(cb);
    }

    // 唤醒相应的，需要执行上面回调操作的loop的线程
    if (!isInLoopThread() || callingPendingFunctors_) {  // callingPendingFunctors_为true时唤醒wakeupfd原因：通知线程有新任务，可以直接处理
        wakeUp();  // 唤醒loop所在线程
    }
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    return poller_->hasChannel(channel);
}

void EventLoop::wakeUp() {  // 唤醒working loop
    uint64_t one = 1;
    auto n = write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR("EventLoop::wakeup() writes %ld bytes instead of 8", n);
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);  // 加锁原因：可能base线程操作pendingFunctors_时working线程也在操作
        functors.swap(pendingFunctors_);
    }
    for (const Functor &functor : functors) {
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::runAfter(double delay, Functor cb) {
    timer_->addTimerInLoop(delay, false, std::move(cb));
}

void EventLoop::runEvery(double delay, EventLoop::Functor cb) {
    timer_->addTimerInLoop(delay, true, std::move(cb));
}


=======
//
// Created by 12096 on 2022/9/9.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Logger.h"
#include "Poller.h"
#include "TimerHeap.h"
#include <sys/eventfd.h>
#include <fcntl.h>
#include <unistd.h>

// 一个线程只能创建一个Eventloop
__thread EventLoop *t_loopInThisThread = nullptr;

// 定义默认的Poller IO复用接口的超时时间
const int kPollTimeMs = 10000;

// 创建eventfd, 用于唤醒working loop处理新来的channel
int createEventFd() {
    int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evfd < 0) {
        LOG_FATAL("eventfd error: %d\n", errno);
    }
    return evfd;
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , callingPendingFunctors_(false)
    , threadId_(CurrentThread::tid())
    , poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(createEventFd())
    , wakeupChannel_(new Channel(this, wakeupFd_))
    , currentActiveChannel_(nullptr)
    , timer_(new TimerHeap(this))
{
    LOG_INFO("EventLoop %p created in thread %d\n", this, threadId_);
    if (t_loopInThisThread) {
        LOG_FATAL("Another EventLoop %p exists in this thread %d\n", t_loopInThisThread, threadId_);
    }
    else {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));  // ???
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::handleRead() {   // 唤醒working loop后执行的读回调
    uint64_t one = 1;
    auto n = read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR("EventLoop::handleRead() reads %ld bytes instead of 8", n);
    }
}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;
    LOG_INFO("EventLoop %p start looping \n", this);
    while (!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for (auto channel : activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        doPendingFunctors();
    }
}

// 1. 在线程中调用quit方法quit自己  2. 在线程中调用quit方法quit其他线程
void EventLoop::quit() {
    quit_ = true;
    // 在某线程中quit其他线程，则需要唤醒其他线程，防止该线程阻塞
    if (!isInLoopThread()) {
        wakeUp();
    }
//    LOG_INFO("loop %p has quited", this);
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) {
        cb();
    }
    else {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.push_back(cb);
    }

    // 唤醒相应的，需要执行上面回调操作的loop的线程
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeUp();  // 唤醒loop所在线程
    }
}

void EventLoop::updateChannel(Channel *channel) {
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel) {
    return poller_->hasChannel(channel);
}

void EventLoop::wakeUp() {  // 唤醒working loop
    uint64_t one = 1;
    auto n = write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        LOG_ERROR("EventLoop::wakeup() writes %ld bytes instead of 8", n);
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for (const Functor &functor : functors) {
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::runAfter(double delay, Functor cb) {
    timer_->addTimerInLoop(delay, false, std::move(cb));
}

void EventLoop::runEvery(double delay, EventLoop::Functor cb) {
    timer_->addTimerInLoop(delay, true, std::move(cb));
}


>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
