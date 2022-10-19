//
// Created by 12096 on 2022/9/10.
//

#pragma once

namespace CurrentThread {
    extern __thread int t_cachedTid;  // __thread代表该变量是线程特有局部变量，extern代表该变量的定义在其他文件中

    void cacheTid();

    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {  // __builtin_expect(EXP, N)意思是：EXP==N的概率很大，以告诉cpu先判断哪个分支
            cacheTid();
        }
        return t_cachedTid;
    }
}
