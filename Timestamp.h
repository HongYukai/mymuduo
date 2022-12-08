<<<<<<< HEAD
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
    void swap(TimeStamp& that);
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t microSecondsSinceEpoch_;

};
=======
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
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
