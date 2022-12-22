//
// Created by 12096 on 2022/9/6.
//

#pragma once

/*
 * 该类被派生类继承后，派生类可以正常构造和析构，但不能拷贝赋值和拷贝构造，因为默认的复制构造函数会调用父类的复制构造函数
 * （除非派生类自己定义了拷贝赋值和拷贝构造函数）
 */
class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
