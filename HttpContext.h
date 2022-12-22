//
// Created by 12096 on 2022/12/7.
//

#ifndef MYMUDUO_HTTPCONTEXT_H
#define MYMUDUO_HTTPCONTEXT_H

#include "Buffer.h"
<<<<<<< HEAD
=======
#include "Logger.h"
>>>>>>> 8cdbf3f71c984f3a6a3726ff79e7cba1b1ad1ad4
#include "HttpRequest.h"

class HttpContext{
public:
    enum HttpRequestParseState{
        EXPECT_LINE,
        EXPECT_HEADER,
        EXPECT_BODY,
        GOT_ALL,
    };
    HttpContext() : httpRequestParseState_(EXPECT_LINE){}
    bool parseRequest(Buffer* buffer, TimeStamp receiveTime);
    bool gotAll() const {return httpRequestParseState_ == GOT_ALL;}
    void reset() {
        httpRequestParseState_ = EXPECT_LINE;
        HttpRequest dummy;
        httpRequest_.swap(dummy);
    }
    const HttpRequest& request() const {return httpRequest_;}
    HttpRequest& request() { return httpRequest_; }
private:
    bool processRequestLine(const char* start, const char* end);
    HttpRequest httpRequest_;
    HttpRequestParseState httpRequestParseState_;
};

#endif //MYMUDUO_HTTPCONTEXT_H
