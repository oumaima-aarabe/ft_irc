#include "server.hpp"

int main(int ac, char **av)
{

//   int current_size = 0;
//   int timeout;
//   int new_sd;
//   struct sockaddr_storage their_addr;
//   std::vector<struct pollfd> fds; 
//   std::string passe;

  if (ac != 3)
  {
    std::cout << "ERROR : ./exec port password" << std::endl;
    return(0);
  }

  Server server(atoi(av[1]), av[2]);

  server.create_server();
  server.waiting_for_connctions();
  
}
