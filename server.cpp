#include "server.hpp"
#include <sys/utsname.h>

// Server::Server(unsigned int port, std::string password){
//   char hostnam[256];
//   if (gethostname(hostnam, sizeof(hostnam)) == -1){
//     Logger::error("Error getting hostname");
//     return;
//   }
//   this->port = port;
//   this->password = password;
//   this->hostname = hostnam;
// }

Server::~Server(){
  for(size_t i = 0; i < fds.size(); i++){
    close (fds[i].fd);
  }
}

Server::Server(unsigned int port, std::string password) {
    struct utsname unameData;
    if (uname(&unameData) == -1) {
        Logger::error("uname failed");
        return;
    }
    this->port = port;
    this->password = password;
    this->hostname = unameData.nodename;
    std::cout << hostname << std::endl;
}
