#include <string>
#include <vector>
#include "Client.hpp"

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::vector<std::string> split(std::string & str, std::string & delimiter) {
    std::vector<std::string> ret;
    my_trim_(str, ' ');
    size_t found = str.find(delimiter);
    if (found == std::string::npos)
        return ret;
    size_t i = 0;
    while (found != std::string::npos){
        std::string str = str.substr(i, found);
        my_trim_(str, ' ');
        ret.push_back(str);
        i = found;
        str = str.substr(found);
        my_trim_(str, ' ');
        found = str.find(delimiter);
    }
    if (!str.empty()){
        ret.push_back(str);
    }
    return (ret);
}

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
    std::cout << "BAD COMMAND" << std::endl;
    return (-1);
  }
  return (0);
}