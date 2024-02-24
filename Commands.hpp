#pragma once

#include "client.hpp"
#include "Channel.hpp"
#include "Replies.hpp"
#include "Utils.hpp"

typedef struct
{
	std::string					cmnd_name;
	std::vector<std::string>	cmnd_args;
} commandInfo;

class Client;

//commands
commandInfo parseCmndBuffer(const std::string &cmndBuffer);
void ft_join(commandInfo& cmd, Client& client);
void ft_part(commandInfo& cmd, Client& client);
void ft_privMsg(commandInfo& cmd, Client& client);
void ft_quit(commandInfo& cmd, Client& client);
void ft_topic(commandInfo& cmd, Client& client);
void ft_invite(commandInfo &cmd, Client &client);
void ft_kick(commandInfo& cmd, Client &client);
void ft_mode(commandInfo& cmd, Client &client);
void ft_notice(commandInfo& cmd, Client& client);
