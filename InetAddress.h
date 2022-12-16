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
    explicit InetAddress(uint16_t port, std::string ip = "0.0.0.0");
    explicit InetAddress(sockaddr_in &addr) : addr_(addr) {}
    std::string toIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;
    const sockaddr_in* getAddr() const {return &addr_;}
    void setAddr(const sockaddr_in &addr) {addr_ = addr;}
private:
    sockaddr_in addr_;
};

