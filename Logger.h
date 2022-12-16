//
// Created by 12096 on 2022/9/6.
//

#pragma once

#include "noncopyable.h"
#include "Logger_Helper.h"

#include <string>

/* ##__VA_ARGS__用于获取可变参数列表 */
#ifdef INFO
#define LOG_INFO(logmsgFormat, ...) \
    do                              \
    {                               \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(INFO);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);            \
    } while (0)
#else
#define LOG_INFO(logmsgFormat, ...)
#endif

#define LOG_ERROR(logmsgFormat, ...) \
    do                              \
    {                               \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(ERROR);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);            \
    } while (0)

#define LOG_FATAL(logmsgFormat, ...) \
    do                              \
    {                               \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(FATAL);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);            \
    } while (0)

#ifdef DEBUG
#define LOG_DEBUG(logmsgFormat, ...) \
    do                              \
    {                               \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(DEBUG);   \
        char buf[1024] = {0};       \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__); \
        logger.log(buf);            \
    } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif

// log level : INFO ERROR FATAL DEBUG
enum LogLevel{
    INFO,
    ERROR,
    FATAL,
    DEBUG,
};

class Logger : noncopyable {
public:
    // 获取日志唯一的实例对象
    static Logger& instance();  // 唯一实例只能通过类来获取，而不能通过对象，因此要把instance方法定义为static
    // 设置日志级别
    void setLogLevel(int level);
    // 写日志
    void log(std::string msg);
private:
    int logLevel_;
    Logger(){}
};