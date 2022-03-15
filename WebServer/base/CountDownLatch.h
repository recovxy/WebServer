#pragma once
#include "Mutex.h"
#include "noncopyable.h"
#include "Condition.h"

//作用：保证某个线程在等待另外一些线程完成各自工作之后，再继续执行
class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_; //初始值为线程数量
};