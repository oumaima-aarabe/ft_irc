#include "server.hpp"

Server::Server(unsigned int port, std::string password){
  char hostnam[256];
  if (gethostname(hostnam, sizeof(hostnam)) == -1){
    Logger::error("Error getting hostname");
    return;
  }
  this->port = port;
  this->password = password;
  this->hostname = hostnam;
}

Server::~Server(){

}

