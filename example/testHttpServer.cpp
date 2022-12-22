//
// Created by 12096 on 2022/12/8.
//

#include "../HttpServer.h"

void testHttpCallback(const HttpRequest& httpRequest, HttpResponse* httpResponse) {
    httpResponse->setStatusCode(HttpResponse::OK);
    httpResponse->setStatusMessage("OK");
<<<<<<< HEAD
    httpResponse->setCloseConnection(true);
=======
    httpResponse->addHeader("Server", "hongws/1.0.0");
    httpResponse->setBody("<!DOCTYPE html>\n"
                          "<html>\n"
                          "<head>\n"
                          "<title>Welcome to hongws!</title>\n"
                          "<style>\n"
                          "        body {\n"
                          "        width: 35em;\n"
                          "        margin: 0 auto;\n"
                          "        font-family: Tahoma, Verdana, Arial, sans-serif;\n"
                          "}\n"
                          "</style>\n"
                          "</head>\n"
                          "<body>\n"
                          "<h1>Welcome to hongws!</h1>\n"
                          "<p>If you see this page, the hongws web server is successfully installed and\n"
                          "        working. Further configuration is required.</p>\n"
                          "\n"
                          "<p>For online documentation and support please refer to\n"
                          "        <a href=\"http://hongws.org/\">hongws.org</a>.<br/>\n"
                          "Commercial support is available at\n"
                          "        <a href=\"http://hongws.com/\">hongws.com</a>.</p>\n"
                          "\n"
                          "<p><em>Thank you for using hongws.</em></p>\n"
                          "</body>\n"
                          "</html>");
>>>>>>> 8cdbf3f71c984f3a6a3726ff79e7cba1b1ad1ad4
}

int main(int argc, char* argv[]) {
    EventLoop loop;
<<<<<<< HEAD
    HttpServer httpServer(&loop, InetAddress(std::stoi(argv[1])), "hongyk's http server", TcpServer::kReusePort);
    httpServer.setThreadNum(0);
//    httpServer.setHttpCallback(testHttpCallback);
=======
    HttpServer httpServer(&loop, InetAddress(std::stoi(argv[1])), "hongws", TcpServer::kReusePort);
    httpServer.setThreadNum(3);
    httpServer.setHttpCallback(testHttpCallback);
>>>>>>> 8cdbf3f71c984f3a6a3726ff79e7cba1b1ad1ad4
    httpServer.start();
    loop.loop();
}