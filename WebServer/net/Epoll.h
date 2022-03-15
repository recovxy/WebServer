#pragma once
#include <sys/epoll.h>
#include "Util.h"
#include "../base/noncopyable.h"


class Epoll : noncopyable
{
public:
    Epoll() {}
    ~Epoll() {}

private:
    epoll_event events[MAX_EVENT_NUMBER];
     

};

