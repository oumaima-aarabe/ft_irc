#include "client.hpp"

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

int  Server::parse_user(Client &client, std::string value){
  std::string user = client.username;
  std::string message_error;

  if (!client.username.empty()){
    return 0;
  }
  if (!client.password.empty()){
    std::vector<std::string> ret = split_space(value);
    if (ret.size() < 4){
      message_error =  ":* 461 * :Not enough parameters\n";
      send(client.fds.fd, message_error.c_str(), message_error.size() + 1, MSG_OOB);
      return (0);
    }
    else if (if_user_exist(ret[0])) {
      message_error = ":* 462 * :You may not reregister\n";
      send(client.fds.fd, message_error.c_str(), message_error.size() + 1, MSG_OOB);
      return (0);
    }
    else{
      client.username = ret[0];
      client.realname = ret[3];
    }
  }
  else
  {
    std::string message_error = ":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n";
    sendReply(message_error.c_str(), client.fds.fd);
    return (-1);
  }
  if (!client.nickname.empty())
  {
     users[client.fds.fd] = Client(client);
      WelcomeMessage(client);
  }
  return (0);
}
 