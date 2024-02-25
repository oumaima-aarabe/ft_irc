#include "server.hpp"

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cout << "ERROR : ./exec port password" << std::endl;
    return (1);
  }
  std::string port = av[1];
  std::string pass = av[2];
  int i = 0;
  if (!port.empty() || !pass.empty())
  {
    while (port[i]) {
      if (!isdigit(port[i]))
      {
        std::cout << "ERROR : port must be a number" << std::endl;
        return (1);
      }
      i++;
    }
    Server server(std::atoi(port.c_str()), pass);
    server.create_server();
    server.waiting_for_connections();
    return (0);
  }
    std::cout << "ERROR: empty parameter\n";
    return (1);  
}
