//
// Created by 12096 on 2022/12/7.
//
#include "HttpServer.h"

void defaultHttpCallback(const HttpRequest& httpRequest, HttpResponse* httpResponse) {
    httpResponse->setStatusCode(HttpResponse::NOT_FOUND);
    httpResponse->setStatusMessage("Not Found");
    httpResponse->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &name,
                       TcpServer::Option option)
                       : tcpServer_(loop, listenAddr, name, option), httpCallback_(defaultHttpCallback){
    tcpServer_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    tcpServer_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void HttpServer::start() {
    LOG_INFO("Http sertver [%s] starts listening on %s", tcpServer_.name().c_str(), tcpServer_.ipPort().c_str());
    tcpServer_.start();
}

void HttpServer::setThreadNum(int threadNum) {
    tcpServer_.setThreadNum(threadNum);
}

void HttpServer::setHttpCallback(const HttpCallback &httpCallback) {
    httpCallback_ = httpCallback;
}

void HttpServer::onConnection(const TcpConnectionPtr &conn) {
    if (conn->connected()) {
        conn->setContext(HttpContext());
    }
}

void HttpServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, TimeStamp receiveTime) {
    HttpContext* httpContext = conn->getContext();
    if (!httpContext->parseRequest(buf, receiveTime)) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }
    if (httpContext->gotAll()) {
        onRequest(conn, httpContext->request());
        httpContext->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr &conn, const HttpRequest &httpRequest) {
    const std::string& connection =httpRequest.getHeader("Connection");
    bool close = connection == "close" || (httpRequest.version() == HttpRequest::HTTP10 && connection != "Keep-Alive");
    HttpResponse httpResponse(close);
    httpCallback_(httpRequest, &httpResponse);
    Buffer buffer;
    httpResponse.appendToBuffer(&buffer);
    conn->send(buffer.retrieveAllAsString());
    if (httpResponse.closeConnection()) {
        conn->shutdown();
    }
}