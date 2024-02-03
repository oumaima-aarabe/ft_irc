#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <vector>
#include <map>

#define BACKLOG 10

class Server{
  private:
    int socket_fd;
    struct sockaddr_in ip4addr;
    unsigned int port;
    std::string password;
    std::vector<struct pollfd> fds;
    int current_size;
};