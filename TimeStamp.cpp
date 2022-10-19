//
// Created by 12096 on 2022/9/8.
//

#include "Timestamp.h"

#include <time.h>

TimeStamp::TimeStamp() : microSecondsSinceEpoch_(0) {}

TimeStamp::TimeStamp(int64_t microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

TimeStamp TimeStamp::now() {
    return TimeStamp(time(NULL));
}

std::string TimeStamp::toString() const {
    char buf[128] = {0};
    tm *time = localtime(&microSecondsSinceEpoch_);
    snprintf(buf, 128, "%04d/%02d/%02d %02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
    return buf;
}

int main () {
    std::cout << TimeStamp::now().toString() << std::endl;
}