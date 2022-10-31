//
// Created by 12096 on 2022/9/9.
//

#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false)
{

}

Channel::~Channel() {
}

//
void Channel::tie(const std::shared_ptr<void> &obj) {
    tie_ = obj;
    tied_ = true;
}

// 通过Eventloop的Poller模块注册fd的events事件
void Channel::update() {
    loop_->updateChannel(this);
}

// 在Eventloop的Channel列表中删除本Channel
void Channel::remove() {
    loop_->removeChannel(this);
}

void Channel::handleEvent(TimeStamp receiveTime) {
    if (tied_) {
        std::shared_ptr<void>  guard = tie_.lock();
        if (guard) {  // 如果tie_指向的内存没有被销毁
            handleEventWithGuard(receiveTime);
        }
    }
    else {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(TimeStamp receiveTime) {
//    LOG_INFO("channel handleEvent revents:%d\n", revents_);
    if ((revents_ & EPOLLHUP)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }
    if (revents_ & EPOLLERR) {
        if (errorCallback_) {
            errorCallback_();
        }

    }
    if (revents_ & EPOLLIN) {
        if (readCallback_) {
            readCallback_(receiveTime);
        }
    }
    if (revents_ & EPOLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}

