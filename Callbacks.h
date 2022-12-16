//
// Created by 12096 on 2022/9/13.
//

#pragma once

#include <memory>
#include <functional>

class Buffer;
class TcpConnection;
class TimeStamp;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void (const TcpConnectionPtr&)>;
using CloseCallback = std::function<void (const TcpConnectionPtr&)>;
using WriteCompleteCallback = std::function<void (const TcpConnectionPtr&)>;
using MessageCallback = std::function<void (const TcpConnectionPtr&, Buffer*, TimeStamp)>;
using HighMarkCallback = std::function<void (const TcpConnectionPtr&, size_t)>;

