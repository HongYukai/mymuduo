//
// Created by 12096 on 2022/9/13.
//

#pragma once
#include <vector>
#include <string>

class Buffer {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize)
        , readerIndex_(kCheapPrepend)
        , writeIndex_(kCheapPrepend)
    {

    }

    size_t readableBytes() const {
        return writeIndex_ - readerIndex_;
    };

    size_t writeableBytes() const {
        return buffer_.size() - writeIndex_;
    }

    size_t prependBytes() const {
        return readerIndex_;
    }
    // 缓冲区中可读数据的起始地址
    const char* peek() const {
        return begin() + readerIndex_;
    }
    void retrieve(size_t len) {
        if (len < readableBytes()) {
            readerIndex_ += len;
        }
        else {
            retrieveAll();
        }
    }
    void retrieveAll() {
        readerIndex_ = writeIndex_ = kCheapPrepend;
    }
    std::string retrieveAsString(size_t len) {
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }
    std::string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }
    void makeSpace(size_t len) {
        if (writeableBytes() + prependBytes() < len + kCheapPrepend) {
            buffer_.resize(writeIndex_ + len);
        }
        else {
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_, begin() + writeIndex_, begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writeIndex_ = kCheapPrepend + readable;
        }
    }
    void ensureWriteableBytes(size_t len) {
        if (writeableBytes() < len) {
            makeSpace(len);
        }
    }
    void append(const char* data, size_t len) {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        writeIndex_ += len;
    }
    char* beginWrite() {
        return begin() + writeIndex_;
    }
    const char* beginWrite() const {
        return begin() + writeIndex_;
    }
    ssize_t readFd(int fd, int* savedErrno);  // ssize_t : signed size_t
    ssize_t writeFd(int fd, int* saveErrno);
private:
    char* begin() {
        return &*buffer_.begin();
    }
    const char* begin() const {
        return &*buffer_.begin();
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writeIndex_;
};
