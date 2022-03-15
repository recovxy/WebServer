#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <unistd.h>
#include <string>

#define MAX_EVENT_NUMBER 1024

int socket_bind_listen(int port);
void error_die(const std::string& sc);
int setnonblocking(int fd);
void addEpollFd(int epollfd, int fd);
