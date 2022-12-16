//
// Created by 12096 on 2022/9/8.
//

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

class InetAddress{
public:
    InetAddress() {}
<<<<<<< HEAD
    explicit InetAddress(uint16_t port, std::string ip = "0.0.0.0");
=======
    explicit InetAddress(uint16_t port, std::string ip = "127.0.0.1");
>>>>>>> 621da876cdd7a7a2342272d5ce339afcfe6d5b01
    explicit InetAddress(sockaddr_in &addr) : addr_(addr) {}
    std::string toIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;
    const sockaddr_in* getAddr() const {return &addr_;}
    void setAddr(const sockaddr_in &addr) {addr_ = addr;}
private:
    sockaddr_in addr_;
};

