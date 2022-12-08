<<<<<<< HEAD
//
// Created by 12096 on 2022/9/10.
//

#pragma once

#include "Poller.h"
#include "Timestamp.h"
#include <vector>
#include <sys/epoll.h>

class EpollPoller : public Poller {
public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller() override;
    TimeStamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
    static const int kInitEventListSize = 16;
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void update(int operation, Channel *channel);
    int epollfd_;
    using EventList = std::vector<epoll_event>;
    EventList events_;
};
=======
//
// Created by 12096 on 2022/9/10.
//

#pragma once

#include "Poller.h"
#include "Timestamp.h"
#include <vector>
#include <sys/epoll.h>

class EpollPoller : public Poller {
public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller() override;
    TimeStamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;

private:
    static const int kInitEventListSize = 16;
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void update(int operation, Channel *channel);
    int epollfd_;
    using EventList = std::vector<epoll_event>;
    EventList events_;
};
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
