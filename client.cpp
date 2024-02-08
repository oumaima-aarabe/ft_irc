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


// Client& Client::operator=(const Client& other){
//   if (this != &other){
//     fds= other.fds;
//     username = other.username;
//     nickname = other.nickname;
// }     password = other.password;
//     buffer = other.buffer;
//   }
//   return(*this);
// }

// Client::Client(const Client &other){
//   *this = other;
//

Client::~Client(){

}