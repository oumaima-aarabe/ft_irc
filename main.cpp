#include "headers/Server.hpp"

int main(int ac, char **av)
{
  signal(SIGPIPE, SIG_IGN);
  if (ac != 3)
  {
    Logger::error("ERROR : ./exec port password");
    return (1);
  }
  if (atoi(av[1]) > 65535|| atoi(av[1]) < 1024){
    Logger::error("ERROR : invalid port value");
    return (1);
  }
  std::string port = av[1];
  std::string pass = av[2];
  int i = 0;
  if (!port.empty() && !pass.empty())
  {
    while (port[i]) {
      if (!isdigit(port[i]))
      {
        Logger::error("ERROR : port must be a number");
        return (1);
      }
      i++;
    }
    Server server(std::atoi(port.c_str()), pass);
    server.create_server();
    server.waiting_for_connections();
    return (0);
  }
    Logger::error("ERROR: empty parameter");
    return (1);  
}
