#include "Thread.h"
#include <assert.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <iostream>

#include "CurrentThread.h"


pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid));}

void CurrentThread::cacheTid()
{
    if(t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength = 
            snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_; //把用户传给Thread的函数交给ThreadData执行
    pid_t* tid_;
    std::string name_;
    CountDownLatch* latch_;

    ThreadData(const ThreadFunc& func, const std::string& name, pid_t* tid,
        CountDownLatch* latch)
        :func_(func), name_(name), tid_(tid), latch_(latch)
    {}

    void runInThread()
    {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);

        func_(); //运行
        CurrentThread::t_threadName = "finished";
    }
};

//pthread_create只接受  void*(void*) 类型的指针，此函数作为辅助传入pthread_create
void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

Thread::Thread(ThreadFunc func, const std::string& name = std::string())
            :func_(func), 
            name_(name),
            joined_(false),
            pthreadID_(0),
            tid_(0),
            latch_(1)
            {
                setDefaultName();
            }

Thread::~Thread()
{
    if(started_ && !joined_) pthread_detach(pthreadID_);
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    auto data = new ThreadData(func_, name_, &tid_, &latch_);
    if (pthread_create(&pthreadID_, NULL, &startThread, data)) {
        started_ = false;
        delete data; //销毁
    } else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadID_, NULL);
}

void Thread::setDefaultName()
{
    if(name_.empty()){
        char buf[32];
        snprintf(buf, sizeof buf, "Thread");
        name_ = buf;
    }
}