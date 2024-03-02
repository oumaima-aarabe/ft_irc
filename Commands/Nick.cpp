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
// e.g.: NICK NEW_NICK
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
	std::string oldNick = client.nickname;
	client.setNickname(cmd.cmnd_args[0]);
	
	std::vector<Channel*> channels = server.channels;
	server.sendReply(RPL_NICKCHANGE(oldNick, cmd.cmnd_args[0]), client.fds.fd);
	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i]->isJoined(oldNick)) {
			channels[i]->getClient(oldNick).nickname = cmd.cmnd_args[0];
			channels[i]->broadcastMessage(&client, RPL_NICKCHANGE(oldNick, client.nickname), false);
		}
	}
}

	// if (!client.nickname.empty())
	// {
	// 	std::string oldNick = client.nickname;
	// 	client.nickname = cmd.cmnd_args[0];
	// 	std::vector<Channel*> channels = server.channels;
	// 	server.sendReply(RPL_NICKCHANGE(oldNick, client.nickname), client.fds.fd);
	// 	for (size_t i = 0; i < channels.size(); i++)
	// 	{
	// 		if (channels[i]->isJoined(oldNick))
	// 		{
	// 			channels[i]->broadcastMessage(&client, RPL_NICKCHANGE(oldNick, client.nickname), false);
	// 			channels[i]->updateNick(oldNick, client.nickname, (channels[i])->getAllClientsListRef());
	// 			channels[i]->updateNick(oldNick, client.nickname, (channels[i])->getOpeListRef());
	// 		}
	// 		if (channels[i]->isInvited(oldNick))
	// 		{
	// 			channels[i]->updateNick(oldNick, client.nickname, (channels[i])->getInviteListRef());
	// 		}
	// 	}
	// }