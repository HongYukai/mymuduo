//
// Created by 12096 on 2022/12/7.
//

#ifndef MYMUDUO_HTTPREQUEST_H
#define MYMUDUO_HTTPREQUEST_H

#include "Timestamp.h"
#include <map>
#include <cassert>

void ltrim(std::string& s);

void rtrim(std::string& s);

void trim(std::string& s);

class HttpRequest{
public:
    enum Method {
        INVALID, GET, POST,
    };
    enum Version {
        UNKNOWN, HTTP10, HTTP11,
    };
    HttpRequest() : method_(INVALID), version_(UNKNOWN) {}
    void setVersion(Version version) {
        version_ = version;
    }
    Version version() const {
        return version_;
    }
    bool setMethod(const char* start, const char* end) {
        assert(method_ == INVALID);
        std::string m(start, end);
        if (m == "GET") {
            method_ = GET;
        }
        else if (m == "POST") {
            method_ = POST;
        }
        return method_ != INVALID;
    }
    Method method() const {
        return method_;
    }
    const char* methodString() {
        const char* result = "INVALID";
        switch (method_) {
            case GET:
                result = "GET";
                break;
            case POST:
                result = "POST";
            default:
                break;
        }
        return result;
    }
    void setPath(const char* start, const char* end) {
        path_.assign(start, end);
    }
    const std::string& path() const {
        return path_;
    }
    void setQuery(const char* start, const char* end) {
        query_.assign(start, end);
    }
    const std::string& query() const {
        return query_;
    }
    void setReceiveTime(TimeStamp reveiceTime) {
        receiveTime_ = reveiceTime;
    }
    TimeStamp receiveTime() const {
        return receiveTime_;
    }
    void addHeader(const char* start, const char* colon, const char* end) {
        std::string field(start, colon);
        std::string value(colon + 1, end);
        trim(field);
        trim(value);
        headers_[field] = value;
    }
    std::string getHeader (const std::string& field) const{
        auto it = headers_.find(field);
        if (it == headers_.end()) {
            return std::string();
        }
        return it->second;
    }
    const std::map<std::string, std::string>& headers() const {
        return headers_;
    }
    void swap(HttpRequest& that) {
        std::swap(this->method_, that.method_);
        std::swap(this->version_, that.version_);
        path_.swap(that.path_);
        query_.swap(that.query_);
        receiveTime_.swap(that.receiveTime_);
        headers_.swap(that.headers_);
    }

private:
    Version version_;
    Method method_;
    std::string path_;
    std::string query_;
    TimeStamp receiveTime_;
    std::map<std::string, std::string> headers_;

};

#endif //MYMUDUO_HTTPREQUEST_H
