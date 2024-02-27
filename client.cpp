#include "client.hpp"

Client::Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer){
  this->fds = fds;
  this->username = username;
  this->nickname = nickname; 
  this->password = password;
  this->buffer = buffer;
}

Client::Client(){

}

Client::~Client(){
  close(fds.fd);
}