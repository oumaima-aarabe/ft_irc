#include "client.hpp"

int Server::if_nick_exist(std::string value){
  std::map<int, Client>::iterator it;
  for (it = users.begin();it != users.end();it++)
  {
    if (it->second.nickname == value)
      return (1);
  }
  for (it = connections.begin();it != connections.end();it++)
  {
    if (it->second.nickname == value)
      return (1);
  }
  return (0);
}

int  Server::parse_nick(Client &client, std::string value){
  std::string err;
  std::string nickname = value.substr(0, value.find(' '));

  if (!client.nickname.empty()){
    return 0;
  }
  if (!client.password.empty()){
    std::vector<std::string> ret = split(value, " ");
    if (ret.size() == 0){
      // err =  ":* 431 * :No nickname given\n";
      err = ERR_NONICKNAMEGIVEN(std::string("*"));
      send(client.fds.fd, err.c_str(), err.size() +1, 0); 
      return (0);
    }
    else if (ret.size() != 1)
    {
      err =  ":* 461 * :Not enough parameters\n";
      send(client.fds.fd, err.c_str(), err.size() + 1, 0);
      return (0);
    }
    else if (if_nick_exist(value)) {
      err = ERR_NICKNAMEINUSE(std::string("*"), std::string("*"));
      send(client.fds.fd, err.c_str(), err.size() + 1, 0);
      return (0);
    } else {
      client.nickname = nickname;
    }
  }
  else
  {
    std::string message_error = ":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n";
    send(client.fds.fd, message_error.c_str(), message_error.size() + 1, 0);
    return (-1);
  }
  if (!client.username.empty()){
    users[client.fds.fd] = Client(client);
    WelcomeMessage(client);
  }
  return (0);
}

