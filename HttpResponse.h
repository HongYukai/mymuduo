//
// Created by 12096 on 2022/12/7.
//

#ifndef MYMUDUO_HTTPRESPONSE_H
#define MYMUDUO_HTTPRESPONSE_H

#include <map>
#include "Buffer.h"

class HttpResponse{
public:
    enum StatusCode {
        UNKNOWN, OK = 200, MOVE_PERMANENTLY = 301, BAD_REQUEST = 400, NOT_FOUND = 404,
    };
    HttpResponse(bool close) : statusCode_(UNKNOWN), closeConnection_(close) {}
    void setStatusCode(StatusCode statusCode) {
        statusCode_ = statusCode;
    }
    void setStatusMessage(const std::string& statusMessage) {
        statusMessage_ = statusMessage;
    }
    void setCloseConnection(const bool close) {
        closeConnection_ = close;
    }
    bool closeConnection() const {
        return closeConnection_;
    }
    void setContentType(const std::string& contentType) {
        addHeader("Content-Type", contentType);
    }
    void addHeader(const std::string& field, const std::string& value) {
        headers_[field] = value;
    }
    void setBody(const std::string body) {
        body_ = body;
    }
    void appendToBuffer(Buffer* buffer) const;

private:
    std::map<std::string, std::string> headers_;
    StatusCode statusCode_;
    std::string statusMessage_;
    std::string body_;
    bool closeConnection_;
};

#endif //MYMUDUO_HTTPRESPONSE_H
