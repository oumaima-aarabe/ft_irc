#include "../headers/Client.hpp"
#include "../headers/Utils.hpp"

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
  }
  else
  {
    Logger::error("BAD COMMAND");
    return (-1);
  }
  client.buffer = "";
  return (0);
}

std::pair<std::string, std::string> my_split_pair(const std::string& line, char delimiter) {
    std::pair<std::string, std::string> pair;
    size_t found = line.find(delimiter);
    std::string cmnd = line.substr(0, found);
    if (found == std::string::npos) {
      pair = make_pair(cmnd, "");
      return pair;
    }
    std::string val = line.substr(found+1);
    my_trim_(val, delimiter);
    my_trim_(val, '\r');
    my_trim_(val, '\n');
    my_trim_(cmnd, delimiter);
    pair = make_pair(cmnd, val);
    return pair;
}

void Server::my_split_buffer(Client &client, std::string delimiter) {
    std::pair<std::string, std::string> pair;
    size_t found = client.buffer.find(delimiter);
 
    while (found != std::string::npos)
    {
      std::string rec = client.buffer.substr(0, found);
      my_trim_(rec, ' ');
      pair = my_split_pair(rec, ' ');
      if (parse_pair(client, pair) < 0)
        break;
      //Deliminer xxx xxx\r\n (if limechat)
      found = client.buffer.find(delimiter);
    }
    return ;
}

void Server::parse_buffer_nc(Client &client)
{
  my_split_buffer(client, "\n");
}

void Server::parse_buffer_limechat(Client &client)
{
    my_split_buffer(client, "\r\n");
}
