#pragma once
#include <pthread.h>
#include <functional>
#include <string>
#include <unistd.h>

#include "noncopyable.h"

class CountDownLatch;

class Thread : noncopyable
{
public:
    typedef std::function<void ()> ThreadFunc;
    explicit Thread(ThreadFunc, const std::string& name = std::string());
    virtual ~Thread();

    void start();
    int join();
    bool started() const {return started_;}
    pid_t tid() const { return tid_; }
    void setDefaultName();

private:
    static void* ThreadRoutine(void* arg);
    virtual void Run() = 0;
    bool started_;
    bool joined_;
    pid_t tid_;
    pthread_t pthreadID_;
    ThreadFunc func_;
    std::string name_;
};