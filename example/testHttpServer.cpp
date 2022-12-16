//
// Created by 12096 on 2022/12/8.
//

#include "../HttpServer.h"

<<<<<<< HEAD
void testHttpCallback(const HttpRequest& httpRequest, HttpResponse* httpResponse) {
    httpResponse->setStatusCode(HttpResponse::OK);
    httpResponse->setStatusMessage("OK");
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
}

int main(int argc, char* argv[]) {
    EventLoop loop;
    HttpServer httpServer(&loop, InetAddress(std::stoi(argv[1])), "hongws", TcpServer::kReusePort);
    httpServer.setThreadNum(3);
    httpServer.setHttpCallback(testHttpCallback);
=======
int main() {
    EventLoop loop;
    HttpServer httpServer(&loop, InetAddress(9999, "192.168.5.141"), "hongyk's http server", TcpServer::kReusePort);
>>>>>>> 621da876cdd7a7a2342272d5ce339afcfe6d5b01
    httpServer.start();
    loop.loop();
}