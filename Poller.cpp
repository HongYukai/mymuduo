//
// Created by 12096 on 2022/9/9.
//

#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
    : ownerLoop_(loop)
{

}

bool Poller::hasChannel(Channel *channel) const {
    auto it = channels_.find(channel->getFd());
    return it != channels_.end() && it->second == channel;
}

