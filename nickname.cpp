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
  // int clt = client.fds.fd;
  std::string nick = client.nickname;
  int len = value.size();
  // std::string cltt = std::to_string(clt);
  // int len = message_error.size();
  std::string err;
  std::cout << client.password << "------------"<<std::endl;
  std::cout << client.username << "------------"<<std::endl;
  if (!client.password.empty()){
    if (value.empty()){
      std::string err =  "431 :Nickname is already in use";
      send(client.fds.fd, err.c_str(), err.size() +1, 0); 
    }
    else if (if_nick_exist(value)) {
      std::cout << "error: already exist\n";//SEND()
      std::string message_error = "431 " + nick + " :Nickname is already in use";
      send(client.fds.fd, message_error.c_str(), len + 1, MSG_OOB);
      // (433);
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
    users[client.fds.fd] = Client(client);
    WelcomeMessage(client);
  }
  return (0);
}

