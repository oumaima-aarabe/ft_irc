#pragma once

#include "Server.hpp"

class Client{
  public :
    struct pollfd fds;  //>>fds.fd
    std::string username;
    std::string nickname;
    std::string password;
    std::string buffer;

    Client();
    Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer);
    ~Client();
};

void my_trim_(std::string& s, char delimiter);