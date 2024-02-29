#include "../server.hpp"

bool isValidNickChar(char c)
{
	return (!(c == '#' || c == ':' || c == ' '));
}

bool isValidNick(std::string &nickname)
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

bool isAlreadyUsed(commandInfo &cmd, Server &server)
{
	std::map<int, Client> users = server.users;

	for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->second.nickname == cmd.cmnd_args[0])
			return (true);
	}
	return (false);
}

void ft_nick(commandInfo& cmd, Server& server, Client& client) 
{
	if (!cmd.cmnd_args.size())
	{
		server.sendReply(ERR_NONICKNAMEGIVEN(std::string("*")), client.fds.fd);
		return;
	}
	if (!isValidNick(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_ERRONEUSNICKNAME(std::string("*"), cmd.cmnd_args[0]), client.fds.fd);
		return;
	}
	if (isAlreadyUsed(cmd, server))
	{
		server.sendReply(ERR_NICKNAMEINUSE(std::string("*"), cmd.cmnd_args[0]), client.fds.fd);
		return;
	}
	if (!client.nickname.empty())
	{
		std::string oldNick = client.nickname;
		client.nickname =cmd.cmnd_args[0];
		std::vector<Channel> channels = server.channels;
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].isJoined(client.nickname))
				channels[i].broadcastMessage(&client, RPL_NICKCHANGE(oldNick, client.nickname), false);
		}
		server.sendReply(RPL_NICKCHANGE(oldNick, client.nickname), client.fds.fd);
	}
	else
		client.nickname =cmd.cmnd_args[0];
}