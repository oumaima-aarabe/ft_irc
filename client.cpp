#include "client.hpp"

Client::Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer){
  this->fds = fds; //>>fds.fd
  this->username = username; //>>
  this->nickname = nickname; //>>
  this->password = password;
  this->buffer = buffer; //>>
  this->_is_authenticated = false; //change it to true when user is authenticated
  this->_is_ope = false; //change it to true when user is an operator
  this->_message = new Message(buffer);
}

Client::Client(){

}

Client::~Client(){
  //clode fd
  //set fd to -1
  //client disconneted message
}