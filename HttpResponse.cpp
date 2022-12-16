//
// Created by 12096 on 2022/12/7.
//
#include "HttpResponse.h"
#include <sstream>

void HttpResponse::appendToBuffer(Buffer *buffer) const {
    std::string enter = "\r\n";
    std::stringstream ss;
    //! 响应行
    ss << "HTTP/1.1 " << statusCode_ << " " << statusMessage_ << enter;
    buffer->append(ss.str().c_str(), ss.str().size());
    //! 响应头
<<<<<<< HEAD
    ss.str("");  //! 清空sstream
=======
    ss.clear();
>>>>>>> 621da876cdd7a7a2342272d5ce339afcfe6d5b01
    if (closeConnection_) {
        ss << "Connection: close" << enter;

    }
    else {
        ss << "Content-Length: " << body_.size() << enter
            << "Connection: Keep-Alive" << enter;
    }
    buffer->append(ss.str().c_str(), ss.str().size());
    for (auto it = headers_.begin(); it != headers_.end(); it++) {
<<<<<<< HEAD
        ss.str("");
=======
        ss.clear();
>>>>>>> 621da876cdd7a7a2342272d5ce339afcfe6d5b01
        ss << it->first << ": " << it->second << enter;
        buffer->append(ss.str().c_str(), ss.str().size());
    }
    buffer->append(enter.c_str(), enter.size());
    //! 响应体
    buffer->append(body_.c_str(), body_.size());
<<<<<<< HEAD
}
=======
}
>>>>>>> 621da876cdd7a7a2342272d5ce339afcfe6d5b01
