<<<<<<< HEAD
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

=======
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

>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
