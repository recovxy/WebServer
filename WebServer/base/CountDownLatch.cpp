#pragma once
#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
    : mutex_(), condition_(mutex_), count_(count_)
{}


void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while (count_ > 0) //大于零说明还没轮到自己
    {
        condition_.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    count_--;
    if(count_ == 0) condition_.notifyAll(); 
}