#include "client.hpp"

Client::Client(struct pollfd fds, std::string username, std::string nickname, std::string password, std::string buffer){
  this->fds = fds;
  this->username = username;
  this->nickname = nickname; 
  this->password = password;
  this->buffer = buffer;
}

Client::Client(){
}

Client::~Client(){
}

void  Client::addChannel(Channel &channel)
{
    channels_joined.push_back(channel);
}

void  Client::removeChannel(Channel &channel)
{
    for (std::vector<Channel>::iterator it = channels_joined.begin(); it != channels_joined.end(); it++)
    {
      if (it->getName() == channel.getName())
      {
        channels_joined.erase(it);
        return;
      }
    }
}

void  Client::quitAllChannels()
{
    channels_joined.clear();
}

std::map<int, Client>::iterator Server::getClientByNickname(const std::string &nickName) {
  for (std::map<int, Client>::iterator it = this->users.begin(); it != this->users.end(); it++)
	{
		if (it->second.nickname == nickName)
			return (it);
	}
	return (this->users.end());
}

void Server::removeClientFromServer(Client &client)
{
  std::map<int, Client>::iterator it2 = users.find(client.fds.fd);
  if (it2 != users.end())
  {
    std::vector<struct pollfd>::iterator it = this->fds.begin();
    while (it != this->fds.end())
    {
      if (it->fd == client.fds.fd)
      {
        it = this->fds.erase(it);
        break;
      }
      it++;
    }
    close(it2->second.fds.fd);
      users.erase(it2);
  }
  it2 = connections.find(client.fds.fd);
  if (it2 != connections.end())
    connections.erase(it2);
}
