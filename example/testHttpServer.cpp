//
// Created by 12096 on 2022/12/8.
//

#include "../HttpServer.h"

int main() {
    EventLoop loop;
    HttpServer httpServer(&loop, InetAddress(9999, "192.168.5.141"), "hongyk's http server", TcpServer::kReusePort);
    httpServer.start();
    loop.loop();
}