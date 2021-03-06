#pragma once

class noncopyable
{
protected:
    oncopyable() {}n
    ~noncopyable() {}
private:    
    noncopyable(const noncopyable&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};