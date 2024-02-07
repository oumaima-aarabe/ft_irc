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
  if (!client.password.empty()){
    std::vector<std::string> ret = split_user(value, ' ');
    if (ret.size() != 4){
      std::cout << "error : must be 4\n" ;//send()
      return (-1);
    }
    else if (if_user_exist(ret[0])) {
      std::cout << "error: reregister\n";//SEND()
      return (-1);
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
    //welcome message send()
    // users.insert({client.fds.fd, Client(client)});
    users[client.fds.fd] = Client(client);
    // users[client.fds.fd] = Client(client.fds, client.username, client.nickname, client.password, client.buffer);
  }
  return (0);
}