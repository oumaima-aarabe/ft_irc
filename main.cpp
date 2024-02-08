#include "server.hpp"

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cout << "ERROR : ./exec port password" << std::endl;
    return(0);
  }


  Server server(atoi(av[1]), av[2]);

  server.create_server();
  server.waiting_for_connctions();
  
}
