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
    std::vector<std::string> ret = split_space(value);
    if (ret.size() == 0){
      err = ERR_NONICKNAMEGIVEN(std::string("*"));
      sendReply(err.c_str(), client.fds.fd); 
      return (0);
    }
    if (ret.size() != 1)
    {
      err =  ":* 461 * :Not enough parameters\n";
      sendReply(err.c_str(), client.fds.fd);
      return (0);
    }
    if (!isValidNick(value))
	  {
	  	sendReply(ERR_ERRONEUSNICKNAME(std::string("*"), value), client.fds.fd);
	  	return (0);
	  }
    if (if_nick_exist(value)) {
      err = ERR_NICKNAMEINUSE(std::string("*"), value);
      sendReply(err.c_str(), client.fds.fd);
      return (0);
    }
      client.nickname = nickname;
  }
  else
  {
    std::string message_error = ":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n";
    sendReply(message_error.c_str(), client.fds.fd);
    return (-1);
  }
  if (!client.username.empty()){
    users[client.fds.fd] = Client(client);
    WelcomeMessage(client);
  }
  return (0);
}

