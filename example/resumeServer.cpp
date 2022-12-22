//
// Created by 12096 on 2022/12/21.
//
#include "../HttpServer.h"
#include <fstream>
#include <cstring>

void file_transfer(const HttpRequest&, HttpResponse* httpResponse);

using HttpCallback = void(const HttpRequest&, HttpResponse* httpResponse);

class ResumeServer {
    friend void file_transfer(const HttpRequest&, HttpResponse* httpResponse);
private:
    HttpServer httpServer_;
    static std::string file;
    static std::string svg[11];
public:
    ResumeServer(EventLoop *loop, int port, const std::string& ip = "0.0.0.0", const std::string& name = "HYK RESUME SERVER", TcpServer::Option option = TcpServer::kReusePort) : httpServer_(loop, InetAddress(port, ip), name, option){}
    void start() {
        httpServer_.start();
    }
    void setHttpCallback(HttpCallback httpCallback) {
        httpServer_.setHttpCallback(httpCallback);
    }
    void setThreadNum(int num) {
        httpServer_.setThreadNum(num);
    }
    static char* extract_file(std::string file_name) {
        std::ifstream is (file_name, std::ifstream::binary);
        if (is) {
            // get length of file:
            is.seekg(0, is.end);
            int length = is.tellg();
            is.seekg(0, is.beg);

            char *buffer = new char[length];

            std::cout << "Reading " << length << " characters... ";
            // read data as a block:
            is.read(buffer, length);

            if (is)
                std::cout << "all characters read successfully. count: " << strlen(buffer) << std::endl;
            else
                std::cout << "error: only " << is.gcount() << " could be read";
            is.close();

            return buffer;
        }
        return nullptr;
    }
};

std::string ResumeServer::file = ResumeServer::extract_file("../files/resume/Resume.html");
std::string ResumeServer::svg[11] = {
        ResumeServer::extract_file("../files/resume/assets/envelope-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/briefcase-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/github-brands.svg")
        ,ResumeServer::extract_file("../files/resume/assets/graduation-cap-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/info-circle-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/info-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/pencil.svg")
        ,ResumeServer::extract_file("../files/resume/assets/phone-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/project-diagram-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/rss-solid.svg")
        ,ResumeServer::extract_file("../files/resume/assets/tools-solid.svg")
};


void file_transfer(const HttpRequest& httpRequest, HttpResponse* httpResponse) {
    auto path = httpRequest.path();
    if (path == "/") {
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::file);
    }
    else if (path == "/assets/envelope-solid.svg") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[0]);
    }
    else if (path == "/assets/briefcase-solid.svg") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[1]);
    }
    else if (path == "/assets/github-brands.svg") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[2]);
    }
    else if (path == "/assets/graduation-cap-solid.svg") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[3]);
    }
    else if (path == "/assets/info-circle-solid.svg") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[4]);
    }
    else if (path == "/assets/info-solid.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[5]);
    }
    else if (path == "/assets/pencil.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[6]);
    }
   else if (path == "/assets/phone-solid.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[7]);
   }
   else if(path == "/assets/project-diagram-solid.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[8]);
   }
   else if (path == "/assets/rss-solid.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[9]);
   }
   else if (path == "/assets/tools-solid.svg") {
            httpResponse->setContentType("image/svg+xml");
            httpResponse->setStatusCode(HttpResponse::OK);
            httpResponse->setStatusMessage("OK");
            httpResponse->setBody(ResumeServer::svg[10]);
   }
    else if (path == "/favicon.ico") {
        httpResponse->setContentType("image/svg+xml");
        httpResponse->setStatusCode(HttpResponse::OK);
        httpResponse->setStatusMessage("OK");
        httpResponse->setBody(ResumeServer::svg[1]);
    }
   else {
       httpResponse->setStatusCode(HttpResponse::NOT_FOUND);
       httpResponse->setStatusMessage("Not Found");
   }

}

int main() {
    EventLoop loop;
    ResumeServer resumeServer(&loop, 9898);
    resumeServer.setThreadNum(3);
    resumeServer.setHttpCallback(file_transfer);
    resumeServer.start();
    loop.loop();
}