#include "client.hpp"

Client::Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer){
  this->fds = fds;
  this->username = username;
  this->nickname = nickname;
  this->password = password;
  this->buffer = buffer;
}

Client::Client(const Client &client): fds(client.fds), username(client.username), nickname(client.nickname), password(client.password), buffer(client.buffer){
}

Client::~Client(){

}