#include "Thread.h"
#include <assert.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/prctl.h>
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
    ThreadFunc func_;
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

        func_();
        CurrentThread::t_threadName = "finished";
    }
};


Thread::Thread(ThreadFunc func, const std::string& name = std::string())
            :func_(func), 
            name_(name),
            joined_(false),
            pthreadID_(0),
            tid_(0)
            {
                setDefaultName();
            }

Thread::~Thread()
{
    if(started_ && !joined_) pthread_detach(pthreadID_);
}

void Thread::start()
{
    auto data = new ThreadData(func_, name_, &tid_, &latch));
    pthread_create(&pthreadID_, NULL, func_, data);
    started_ = true;

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

pid_t Thread