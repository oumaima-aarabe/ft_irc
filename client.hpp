#pragma once

#include "server.hpp"

class Client{
  public :
    struct pollfd fds;
    std::string username;
    std::string nickname;
    std::string password;
    std::string buffer;

    Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer);
    Client(const Client &client);
    ~Client();
};