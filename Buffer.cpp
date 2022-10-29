//
// Created by 12096 on 2022/9/13.
//

#include "Buffer.h"
#include <sys/uio.h>
#include <unistd.h>


// 从fd读取数据到buffer(LT)
ssize_t Buffer::readFd(int fd, int *savedErrno) {
    char extrabuf[65536] = {0};
    struct iovec vec[2];  // 可以将file中的数据读到不同的内存中
    const size_t writeable = writeableBytes();
    vec[0].iov_base = begin() + writeIndex_;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);
    const int iovcnt = writeable < sizeof(extrabuf) ? 2 : 1;
    const ssize_t  n = ::readv(fd, vec, iovcnt);
    if (n < 0) {
        *savedErrno = errno;
    }
    else if (n <= writeable) {
        writeIndex_ += n;
    }
    else {
        writeIndex_ = buffer_.size();
        append(extrabuf, n - writeable);
    }
    return n;
}

ssize_t Buffer::writeFd(int fd, int *saveErrno) {
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0) {
        *saveErrno = errno;
    }
    return n;
}


