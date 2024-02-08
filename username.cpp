#include "client.hpp"

std::vector<std::string> Server::split_user(std::string& line, char delimiter) {
    std::vector<std::string> ret;
    my_trim_(line, ' ');
    size_t found = line.find(delimiter);
    if (found == std::string::npos)
      return ret;
    size_t i = 0;
    while (found != std::string::npos){
      std::string str = line.substr(i, found);
      my_trim_(str, ' ');
      ret.push_back(str);
      i = found;
      line = line.substr(found);
      my_trim_(line, ' ');
      found = line.find(delimiter);
    }
    if (!line.empty()){
      ret.push_back(line);
    }
    return (ret);
}

int Server::if_user_exist(std::string value){
  std::map<int, Client>::iterator it;
  for (it = users.begin();it != users.end();it++)
  {
    if (it->second.username == value)
      return (1);
  }
  for (it = connections.begin();it != connections.end();it++)
  {
    if (it->second.username == value)
      return (1);
  }
  return (0);
}

int  Server::parse_user(Client client, std::string value){
  int fd = client.fds.fd;
  std::string clt = std::to_string(fd);
  std::string user = client.username;
  std::string message_error;
  if (!client.password.empty()){
    std::vector<std::string> ret = split_user(value, ' ');
    if (ret.size() != 4){
      message_error = clt + " " + user + " :Not enough parameters";
      send(client.fds.fd, message_error.c_str(), message_error.size() + 1, MSG_OOB);
      // (461);
    }
    else if (if_user_exist(ret[0])) {
      message_error =  clt + " :You may not reregister";
      send(client.fds.fd, message_error.c_str(), message_error.size() + 1, MSG_OOB);
      // (462);
    }
    else{
      client.username = ret[0];
    }
  }
  else
  {
    std::cout << "password not set" << std::endl;
    return (-1);
  }
  if (!client.nickname.empty()){
    users[client.fds.fd] = Client(client);
    WelcomeMessage(client);
  }
  return (0);
}