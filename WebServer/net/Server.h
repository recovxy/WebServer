#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "HttpData.h"
#include "noncopyable.h"

class Server : public noncopyable
{
public:
    explicit Server(int port);
    ~Server();

    void start();

private:
    bool started_;
    int listenFd_;
    int port_;
    static const int MAXFDS = 100000;

    HttpData httpData;


private:
    void accept_request(int connfd);
};
