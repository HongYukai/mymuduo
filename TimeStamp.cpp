//
// Created by 12096 on 2022/9/8.
//

#include "Timestamp.h"
#include <sys/time.h>

#include <time.h>

TimeStamp::TimeStamp() : microSecondsSinceEpoch_(0) {}

TimeStamp::TimeStamp(int64_t microSecondsSinceEpoch) : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

int64_t TimeStamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

std::string TimeStamp::toString(){
    char buf[128] = {0};
    auto t = time(NULL);
    tm *time = localtime(&t);
    snprintf(buf, 128, "%04d/%02d/%02d %02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
    return buf;
}

void TimeStamp::swap(TimeStamp &that) {
    std::swap(this->microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
}
//
//int main () {
//    std::cout << TimeStamp::now().toString() << std::endl;
//}