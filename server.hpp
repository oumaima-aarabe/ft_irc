#pragma once

#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <vector>
#include <map>
#include <string>
#include "client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
#include "Commands/Commands.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include <strstream>

#define BACKLOG 10
#define MAX_CLIENTS_PER_CHANNEL 30
#define MAX_BUFFER_SIZE 512
#define MAX_CONNECTIONS 50
#define MAX_CHANNELS 5

class Client;
class Channel;

class Server{
  public:
    int socket_fd;
    struct sockaddr_in ip4addr;
    unsigned int port;
    std::string password;
    std::vector<struct pollfd> fds;
    int current_size;
    std::string hostname ;

    std::vector<Channel> channels;

    std::map<int ,Client> connections;

    std::map<int ,Client> users; // clients authenticated

    Server(unsigned int port, std::string password);
    ~Server();
    void create_server();
    void waiting_for_connections();
    int is_server_connection();
    int is_client_connection(struct pollfd fds);
    void parse_buffer_nc(Client &client);
    void parse_buffer_limechat(Client &client);
    void my_split_buffer(Client &client, std::string delimiter);
    int  if_nick_exist(std::string value);
    int  if_user_exist(std::string value);
    int  parse_pass(Client &client, std::string value);
    int  parse_nick(Client &client, std::string value);
    int  parse_user(Client &client, std::string value);
    int  parse_pair(Client &client, std::pair<std::string, std::string> pair);
    void WelcomeMessage(Client &client);
    void addToChannels(Channel& channel);
    void removeFromChannels(Channel& channel);
    std::vector<Channel>::iterator getChannelByName(const std::string &name);
    std::map<int, Client>::iterator getClientByNickname(const std::string &nickName);
    void sendReply(const std::string &message, int clientFd);
    void executeCommands(const std::vector<std::string> cmnds, int clientFd);
};