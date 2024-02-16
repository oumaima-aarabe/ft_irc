#include "server.hpp"

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cout << "ERROR : ./exec port password" << std::endl;
    return(0);
  }

  std::string port = av[1];
  std::string pass = av[2];

  if (!port.empty() || !pass.empty()){
    Server server(std::stoi(port), pass);
    server.create_server();
    server.waiting_for_connctions();
  }
  else
    std::cout << "emty parametr\n";

  return -1;
  
}
