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
  // Close fds.fd
}

void  Client::addChannel(Channel channel)
{
    channels_joined.push_back(channel);
}

void  Client::removeChannel(Channel channel)
{
    for (std::vector<Channel>::iterator it = channels_joined.begin(); it != channels_joined.end(); it++) {
      if (it->getName() == channel.getName())
        channels_joined.erase(it);
        return;
    }
}

void  Client::quitAllChannels()
{
    channels_joined.clear();
}