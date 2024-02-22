#include "client.hpp"

Client::Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer){
  this->fds = fds;
  this->username = username;
  this->nickname = nickname; 
  this->password = password;
  this->buffer = buffer;
}

Client::Client(){
  this->fds.fd = -1;
  this->username = "";
  this->nickname = "";
  this->password = "";
  this->buffer = "";
}

Client::~Client(){
  close(this->fds.fd);
  this->fds.fd = -1;
  std::cout << "Client disconnected" << std::endl;
}