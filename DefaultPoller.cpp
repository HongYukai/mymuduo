<<<<<<< HEAD
//
// Created by 12096 on 2022/9/9.
//

#include "Poller.h"
#include "EpollPoller.h"
#include <stdlib.h>

Poller *Poller::newDefaultPoller(EventLoop *loop) {
    if (::getenv("MUDUO_USE_POLL")) {
        return nullptr;  // 生成poll的实例
    }
    else {
        return new EpollPoller(loop);  // 生成epoll的实例
    }
}

=======
//
// Created by 12096 on 2022/9/9.
//

#include "Poller.h"
#include "EpollPoller.h"
#include <stdlib.h>

Poller *Poller::newDefaultPoller(EventLoop *loop) {
    if (::getenv("MUDUO_USE_POLL")) {
        return nullptr;  // 生成poll的实例
    }
    else {
        return new EpollPoller(loop);  // 生成epoll的实例
    }
}

>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
