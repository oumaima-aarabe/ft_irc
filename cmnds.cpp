#include "client.hpp"

int  Server::parse_pair(Client &client, std::pair<std::string, std::string> pair)
{
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
    std::cout << "Called NICK command with success to: " << client.nickname << std::endl;
  }
  else if (pair.first != "")
  {
    std::cout << "BAD COMMAND" << std::endl;
    return (-1);
  }
  client.buffer = "";
  return (0);
}