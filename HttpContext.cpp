//
// Created by 12096 on 2022/12/7.
//

#include "HttpContext.h"

bool HttpContext::parseRequest(Buffer *buffer, TimeStamp receiveTime) {
    bool ok = true;
    bool hasMore = true;
    while (hasMore) {
        if (httpRequestParseState_ == EXPECT_LINE) {
            const char* crlf = buffer->findCRLF();
            if (crlf) {
                ok = processRequestLine(buffer->peek(), crlf);
                if (ok) {
                    httpRequest_.setReceiveTime(receiveTime);
                    buffer->retrieveUntil(crlf + 2);
                    httpRequestParseState_ = EXPECT_HEADER;
                }
                else {
                    hasMore = false;
                }
            }
            else {
                hasMore = false;
            }
        }
        else if (httpRequestParseState_ == EXPECT_HEADER) {
            const char* crlf = buffer->findCRLF();
            if (crlf) {
                const char* colon = std::find(buffer->peek(), crlf, ':');
                if (crlf != colon) {
                    httpRequest_.addHeader(buffer->peek(), colon, crlf);
                }
                else {
                    // FIXME
                    httpRequestParseState_ = GOT_ALL;
                    hasMore = false;
                }
                buffer->retrieveUntil(crlf + 2);
            }
            else {
                hasMore = false;
            }
        }
        else if (httpRequestParseState_ == EXPECT_BODY) {
            // FIXME
<<<<<<< HEAD
=======
            hasMore = false;  // just for safety
        }
        else {  // httpRequestParseState_ == GOT_ALL
            // FIXME
            hasMore = false;  // just for safety
>>>>>>> 8cdbf3f71c984f3a6a3726ff79e7cba1b1ad1ad4
        }
    }
    return ok;
}

bool HttpContext::processRequestLine(const char *start, const char *end) {
    bool success = false;
    const char* space = std::find(start, end, ' ');
    if (space != end && httpRequest_.setMethod(start, space)) {
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end) {
            const char* question = std::find(start, space, '?');
            if (question != space) {
                httpRequest_.setPath(start, question);
                httpRequest_.setQuery(question, space);
            }
            else {
                httpRequest_.setPath(start, space);
            }
            start = space + 1;
            if (std::string(start, end) == "HTTP/1.0") {
                httpRequest_.setVersion(HttpRequest::HTTP10);
                success = true;
            }
            else if (std::string(start, end) == "HTTP/1.1") {
                httpRequest_.setVersion(HttpRequest::HTTP11);
                success = true;
            }
            else {
                success = false;
            }
        }
    }
    return success;
}
