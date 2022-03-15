#pragma once
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include <errno.h>

#include "Mutex.h"
#include "noncopyable.h"

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock &mutex) : mutex_(mutex)
    {
        pthread_cond_init(&cond_, NULL);
    }
    ~Condition() 
    { 
        pthread_cond_destroy(&cond_); 
    }
    void wait() //等待条满足
    { 
        pthread_cond_wait(&cond_, mutex_.getLock()); 
    }
    void notify() //满足条件时发送通知
    { 
        pthread_cond_signal(&cond_); 
    }
    void notifyAll()
    { 
        pthread_cond_broadcast(&cond_); 
    }
    bool waitForSeconds(int seconds) //等待多少秒
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.getLock(), &abstime);
    }

private:
    MutexLock &mutex_;
    pthread_cond_t cond_;
};