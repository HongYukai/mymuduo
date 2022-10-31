//
// Created by 12096 on 2022/9/8.
//

#pragma once

#include <iostream>

class TimeStamp {
public:
    TimeStamp();
    TimeStamp(int64_t microSecondsSinceEpoch);
    static int64_t now();
    static std::string toString();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch_;

};
