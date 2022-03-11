#pragma once
#include <pthread.h>
#include <exception>
#include "noncopyable.h"


class MutexLock : noncopyable
{
public:
    MutexLock() 
    { 
        if (pthread_mutex_init(&mutex_, NULL) != 0 )
        {
            throw std::exception(); //构造函数没有返回值，如果出错则抛出出异常
        } 
    }

    ~MutexLock() 
    {
        pthread_mutex_destroy(&mutex_);
    }
    bool lock()
    {
        return pthread_mutex_lock(&mutex_) == 0; 
    }
    bool unlock() 
    {
        return pthread_mutex_unlock(&mutex_) == 0;
    }   
    pthread_mutex_t* getLock() { return &mutex_; };

private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard : noncopyable
{
public:
    MutexLockGuard(MutexLock& lock)
    : lock_(lock) 
    {
        lock_.lock();    
    }

    ~MutexLockGuard()
    {
        lock_.unlock();
    }
private:
    MutexLock lock_;
};
