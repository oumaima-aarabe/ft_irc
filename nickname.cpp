#include "Client.hpp"

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
  std::string nick = client.nickname;
  std::string err;
  std::vector<std::string> ret = split_user(value, ' ');
  if (!client.password.empty()){
    if (value.empty()){
      err =  ":* 431 * :No nickname given\n";
      send(client.fds.fd, err.c_str(), err.size() +1, 0); 
      return (0);
    }
    else if (if_nick_exist(value)) {
      err = ":* 433 * :Nickname is already in use\n";
      send(client.fds.fd, err.c_str(), err.size() + 1, 0);
      return (0);
    }
    else if (ret.size() > 1){
      err = ":* 432 * :Erroneus nickname\n";
      send(client.fds.fd , err.c_str(), err.size() + 1, 0);
      return (0);
    }
    else
    {
      client.nickname = value;
    }
  }
  else
  {
    std::cout << "password not set" << std::endl;
    return (-1);
  }
  if (!client.username.empty()){
    puts("here");
    users[client.fds.fd] = Client(client);
    WelcomeMessage(client);
  }
  return (0);
}

