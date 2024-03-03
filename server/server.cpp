#include "../headers/Server.hpp"

Server::Server(unsigned int port, std::string password) {
    this->port = port;
    this->password = password;
    this->hostname = "IRCServer.1337.ma";
    Logger::info("-------Server :   Welcome to " + this->hostname + "-------");
}

Server::~Server(){
  for(size_t i = 0; i < fds.size(); i++){
    close (fds[i].fd);
  }
}
