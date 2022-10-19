//
// Created by 12096 on 2022/9/9.
//

#pragma once
#include "noncopyable.h"
#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>
#include "Timestamp.h"
#include "CurrentThread.h"
class Channel;
class Poller;
class TimerHeap;

class EventLoop : noncopyable{
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();
    // 开启事件循环
    void loop();
    // 退出事件循环
    void quit();
    TimeStamp pollReturnTime() const {return pollReturnTime_;}
    void runAfter(int delay, Functor cb);
    void runEvery(int delay, int interval, Functor cb);
    // 在当前loop执行cb
    void runInLoop(Functor cb);
    // 把cb放入队列，唤醒loop所在线程，执行cb
    void queueInLoop(Functor cb);
    // 唤醒loop所在线程
    void wakeUp();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);
    // 判断Eventloop对象是否在自己的线程里面
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();}
private:
    void handleRead(); // wake up
    void doPendingFunctors(); // 执行回调
    using ChannelList = std::vector<Channel*>;
    std::atomic_bool looping_;
    std::atomic_bool quit_; // 退出loop循环
    std::atomic_bool callingPendingFunctors_;  // 标识当前loop是否有需要执行的回调操作
    std::vector<Functor> pendingFunctors_;  // 存储loop需要执行的所有回调操作
    const pid_t threadId_;
    TimeStamp pollReturnTime_;  // 返回发生事件的channels的时间点
    std::unique_ptr<Poller> poller_;
    int wakeupFd_;  // mainloop通过该成员唤醒working loop，将新用户的channel传递给working loop处理
    std::unique_ptr<Channel> wakeupChannel_;  // 包含wakeupFd_和感兴趣的事件 （统一事件源）
    ChannelList activeChannels_;
    Channel *currentActiveChannel_;
    std::mutex mutex_; // 用于确保vector容器的线程安全
    std::unique_ptr<TimerHeap> timer_;
};
