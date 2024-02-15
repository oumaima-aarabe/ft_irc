#pragma once

#include "server.hpp"
#include "Message.hpp"

class Client{
  public :
    struct pollfd fds;
    std::string username;
    std::string nickname;
    std::string password;
    std::string buffer;
    bool _is_authenticated; //better to add
    bool _is_ope; //better to add
    Message * _message; // calls Message constructor that splits buffer into cmnd and params to be getted later

    Client();
    Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer);
    ~Client();
    void message(std::string const message); //send message to client
};

void my_trim_(std::string& s, char delimiter);