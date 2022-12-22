//
// Created by 12096 on 2022/12/7.
//

#ifndef MYMUDUO_HTTPSERVER_H
#define MYMUDUO_HTTPSERVER_H

#include "TcpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Logger.h"
#include "HttpContext.h"

class HttpServer{
    using HttpCallback = std::function<void(const HttpRequest&, HttpResponse*)>;
public:
    HttpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, TcpServer::Option option);
    void setHttpCallback(const HttpCallback &httpCallback);
    void setThreadNum(int threadNum);
    void start();
private:
    HttpCallback httpCallback_;
    TcpServer tcpServer_;
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime);
    void onRequest(const TcpConnectionPtr& conn, const HttpRequest& httpRequest);

};

#endif //MYMUDUO_HTTPSERVER_H
