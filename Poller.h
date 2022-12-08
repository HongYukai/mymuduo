//
// Created by 12096 on 2022/9/9.
//

#pragma once

#include "noncopyable.h"
#include "Timestamp.h"

#include <vector>
#include <unordered_map>

class Channel;  // 所用的是该类的指针类型，不用包含头文件
class EventLoop;

class Poller : noncopyable {
public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop *loop);
    virtual ~Poller() = default;
    // 给所有IO复用保留统一接口
    virtual TimeStamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    bool hasChannel(Channel *channel) const;
    // 获取IO复用的具体实现
    static Poller* newDefaultPoller(EventLoop* loop);
protected:
    using ChannelMap = std::unordered_map<int, Channel*>;  // sockfd -> channel*
    ChannelMap channels_;
private:
    EventLoop *ownerLoop_;
};