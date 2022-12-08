<<<<<<< HEAD
//
// Created by 12096 on 2022/10/31.
//

#ifndef MYMUDUO_CLOCK_H
#define MYMUDUO_CLOCK_H

#include <sys/time.h>
#include <iostream>

class Clock{
public:
    Clock() {
        gettimeofday(&t1, 0);
    }
    ~Clock() {
        gettimeofday(&t2, 0);
        std::cout << 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) << std::endl;
    }

private:
    struct timeval t1, t2;
};

#endif //MYMUDUO_CLOCK_H
=======
//
// Created by 12096 on 2022/10/31.
//

#ifndef MYMUDUO_CLOCK_H
#define MYMUDUO_CLOCK_H

#include <sys/time.h>
#include <iostream>

class Clock{
public:
    Clock() {
        gettimeofday(&t1, 0);
    }
    ~Clock() {
        gettimeofday(&t2, 0);
        std::cout << 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) << std::endl;
    }

private:
    struct timeval t1, t2;
};

#endif //MYMUDUO_CLOCK_H
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
