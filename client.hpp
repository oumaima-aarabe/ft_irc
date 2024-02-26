#pragma once

#include "server.hpp"
#include "Channel.hpp"

class Client{
    
  public :
    struct pollfd fds;  //>>fds.fd
    std::string username;
    std::string nickname;
    std::string password;
    std::string buffer;
    std::vector<Channel> channels_joined;

    Client();
    Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer);
    ~Client();

    void addChannel(Channel &channel);
    void removeChannel(Channel &channel);
    void quitAllChannels();
};
