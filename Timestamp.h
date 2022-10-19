//
// Created by 12096 on 2022/9/8.
//

#pragma once

#include <iostream>

class TimeStamp {
public:
    TimeStamp();
    TimeStamp(int64_t microSecondsSinceEpoch);
    static TimeStamp now();
    std::string toString() const;
private:
    int64_t microSecondsSinceEpoch_;
};
