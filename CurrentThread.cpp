<<<<<<< HEAD
//
// Created by 12096 on 2022/11/6.
//
__thread int t_cachedTid = 0;
=======
//
// Created by 12096 on 2022/9/10.
//

#include "CurrentThread.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace CurrentThread {
    __thread int t_cachedTid = 0;
    void cacheTid() {
        if (t_cachedTid == 0) {
            t_cachedTid = static_cast<pid_t> (::syscall(SYS_gettid));
        }
    }
}
>>>>>>> c02fd5c823787dd707ea2627ba81e2b84f3866ee
