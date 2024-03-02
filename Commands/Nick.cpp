#include "../server.hpp"

bool Server::isValidNickChar(char c)
{
	return (!(c == '#' || c == ':' || c == ' '));
}

bool Server::isValidNick(std::string &nickname)
{
	if (nickname.empty())
		return (false);
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isValidNickChar(nickname[i]))
			return (false);
	}
	return (true);
}

void ft_nick(commandInfo& cmd, Server& server, Client& client) 
{
	if (!cmd.cmnd_args.size())
	{
		server.sendReply(ERR_NONICKNAMEGIVEN(std::string("*")), client.fds.fd);
		return;
	}
	if (!server.isValidNick(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_ERRONEUSNICKNAME(std::string("*"), cmd.cmnd_args[0]), client.fds.fd);
		return;
	}
	if (server.if_nick_exist(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_NICKNAMEINUSE(std::string("*"), cmd.cmnd_args[0]), client.fds.fd);
		return;
	}
	if (!client.nickname.empty())
	{
		std::string oldNick = client.nickname;
		client.nickname =cmd.cmnd_args[0];
		std::vector<Channel*> channels = server.channels;
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i]->isJoined(client.nickname))
				channels[i]->broadcastMessage(&client, RPL_NICKCHANGE(oldNick, client.nickname), false);
		}
		server.sendReply(RPL_NICKCHANGE(oldNick, client.nickname), client.fds.fd);
	}
	else
		client.nickname =cmd.cmnd_args[0];
}