//
// Created by 12096 on 2022/9/10.
//

#include "EpollPoller.h"
#include "Logger.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

const int kNew = -1;  // channel未添加到poller中
const int kAdded = 1;  // channel已添加到poller中
const int kDeleted = 2;  // channel从poller中删除

EpollPoller::EpollPoller(EventLoop *loop)
    : Poller(loop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))  // fork+exec后子进程会把父进程中设置的fd全部关闭（子进程默认会继承父进程所有fd）
    , events_(kInitEventListSize)
{
    if (epollfd_ < 0) {
        LOG_FATAL("epoll_create error:%d\n", errno);
    }
}

EpollPoller::~EpollPoller() {
    ::close(epollfd_);
}

TimeStamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels) {
    LOG_INFO("fd total count: %lu\n", channels_.size());
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    TimeStamp now(TimeStamp::now());
    if (numEvents > 0) {
        LOG_INFO("%d events happened\n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numEvents == 0) {
        LOG_INFO("%s timeout!\n", __FUNCTION__ );
    }
    else {
        // 不是被系统调用中断的错误
        if (saveErrno != EINTR) {
            errno = saveErrno;
            LOG_ERROR("EPOLL err! errno: %d\n", errno);
        }
    }

    return now;
}

void EpollPoller::updateChannel(Channel *channel) {
    const int index = channel->getIndex();
    LOG_INFO("fd=%d, events=%d, index=%d\n", channel->getFd(), channel->getEvents(), index);
    if (index == kNew || index == kDeleted) {
        if (index == kNew) {
            auto fd = channel->getFd();
            channels_.insert(std::make_pair(fd, channel));

        }
        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else {
        auto fd = channel->getFd();
        if (channel->isNoneEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDeleted);
            channels_.erase(fd);
        }
        else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EpollPoller::removeChannel(Channel *channel) {
    int fd = channel->getFd();
    int index = channel->getIndex();
    channels_.erase(fd);
    if (index == kAdded) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(kNew);
}

void EpollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const {
    for (int i = 0; i < numEvents; i++) {
        Channel *channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EpollPoller::update(int operation, Channel *channel) {
    epoll_event event;
    memset(&event, 0, sizeof(event));
    int fd = channel->getFd();
    event.events = channel->getEvents();
    event.data.ptr = channel;
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
            LOG_ERROR("epoll_ctl del error: %d\n", errno);
        }
        else {
            LOG_FATAL("epoll_ctl add/mod error: %d\n", errno);
        }
    }
}

