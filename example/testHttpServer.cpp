//
// Created by 12096 on 2022/12/8.
//

#include "../HttpServer.h"

void testHttpCallback(const HttpRequest& httpRequest, HttpResponse* httpResponse) {
    httpResponse->setStatusCode(HttpResponse::OK);
    httpResponse->setStatusMessage("OK");
    httpResponse->setCloseConnection(true);
}

int main(int argc, char* argv[]) {
    EventLoop loop;
    HttpServer httpServer(&loop, InetAddress(std::stoi(argv[1])), "hongyk's http server", TcpServer::kReusePort);
    httpServer.setThreadNum(0);
//    httpServer.setHttpCallback(testHttpCallback);
    httpServer.start();
    loop.loop();
}