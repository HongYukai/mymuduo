//
// Created by 12096 on 2022/9/17.
//

#include <mymuduo/TcpServer.h>
#include <mymuduo/Logger.h>

class EchoServer {
private:
    EventLoop *loop_;
    TcpServer server_;
    void onConnection(const TcpConnectionPtr &conn) {
        if (conn->connected()) {
            LOG_INFO("conn UP: %s", conn->peerAddress().toIpPort().c_str());
        }
        else {
            LOG_INFO("conn DOWN: %s", conn->peerAddress().toIpPort().c_str());
        }
    }
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, TimeStamp time) {
        std::string msg = buf->retrieveAllAsString();
        conn->send(msg);
        conn->shutdown();
    }

public:
    EchoServer(EventLoop *loop, const InetAddress &addr, const std::string &name)
        : server_(loop, addr, name)
        , loop_(loop)
        {
            server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
            server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            server_.setThreadNum(3);
        }
    void start() {
        server_.start();
    }
};

void cb() {
    std::cout << "hyk is so handsome!" << std::endl;
}

int main() {
    EventLoop loop;
    InetAddress addr(8989);
    EchoServer server(&loop, addr, "EchoServer-01");
    server.start();
    loop.runEvery(3, 3, cb);
    loop.loop();
}



