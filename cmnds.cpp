#include "client.hpp"

int  Server::parse_pair(Client client, std::pair<std::string, std::string> pair)
{
  std::cout << "first:<<" << pair.first << ">>" << std::endl; 
  std::cout << "second:<<" << pair.second << ">>" << std::endl;
  if (pair.first == "PASS")
  {
    if (parse_pass(client, pair.second) == -1)
      return (-1);
  }
  else if (pair.first == "USER")
  {
    if (parse_user(client, pair.second) == -1)
      return (-1);
  }
  else if (pair.first == "NICK")
  {
    if (parse_nick(client, pair.second) == -1)
      return (-1);
  }
  else
  {
    std::cout << "BAD COMMAND" << std::endl;
    return (-1);
  }
  return (0);
}