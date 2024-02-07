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

int  Server::parse_nick(Client client, std::string value){
  if (!client.password.empty()){
    if (value.empty()){
      std::cout << "ERROR : 461 PARAMETR ENOUTHG\n";//SEND()
      return (-1);
    }
    else if (if_nick_exist(value)) {
      std::cout << "error: already exist\n";//SEND()
      return (-1);
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
    //welcome message send()
    // users.insert({client.fds.fd, Client(client)});
    users[client.fds.fd] = Client(client);
    // users[client.fds.fd] = Client(client.fds, client.username, client.nickname, client.password, client.buffer);
  }
  return (0);
}

