#include "../headers/Server.hpp"

// e.g.: KICK <channel> <nick> [<reason>] - Remove a user from a channel
void ft_kick(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() < 2)
	{
		server.sendReply(ERR_NEEDMOREPARAMS(server.hostname, client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	if (!Channel::isValidChannelName(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_NOSUCHCHANNEL(server.hostname, client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	std::vector<Channel*>::iterator channel = server.getChannelByName(cmd.cmnd_args[0]);
	if (channel == server.channels.end())
	{
		server.sendReply(ERR_NOSUCHCHANNEL(server.hostname, client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	if (!(*channel)->isJoined(client.nickname))
	{
		server.sendReply(ERR_NOTONCHANNEL(server.hostname, client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	if (!(*channel)->isOpe(client.nickname))
	{
		server.sendReply(ERR_CHANOPRIVSNEEDED(server.hostname, client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	std::map<int ,Client>::iterator target = server.getClientByNickname(cmd.cmnd_args[1]);
	if (target == server.users.end())
	{
		server.sendReply(ERR_NOSUCHNICK(server.hostname, cmd.cmnd_args[1]), client.fds.fd);
		return ;
	}
	if (!(*channel)->isJoined(target->second.nickname))
	{
		server.sendReply(ERR_USERNOTINCHANNEL(client.nickname, cmd.cmnd_args[1], cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	(*channel)->removeClient(target->second);
    target->second.removeChannel(*channel);
    if ((*channel)->isOpe(cmd.cmnd_args[1]))
        (*channel)->removeOpe(cmd.cmnd_args[1]);
	if ((*channel)->getAllClientsList().size() == 0)
		server.channels.erase(channel);
	else if ((*channel)->getOpeList().size() == 0)
	{
		(*channel)->addOpe((*channel)->getAllClientsList()[0].nickname);
		(*channel)->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostNames[client.fds.fd], (*channel)->getAllClientsList()[0].nickname, (*channel)->getAllClientsList()[0].realname), ((*channel)->getName() + " +o " +  (*channel)->getAllClientsList()[0].nickname)), false);
	}
	(*channel)->broadcastMessage(NULL, RPL_KICK(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), cmd.cmnd_args[0], cmd.cmnd_args[1], ((cmd.cmnd_args.size() > 2) ? cmd.cmnd_args[2] : "")), false);
	server.sendReply(RPL_KICK(":" + client.nickname, cmd.cmnd_args[0], cmd.cmnd_args[1], ((cmd.cmnd_args.size() > 2) ? cmd.cmnd_args[2] : "")), target->second.fds.fd);
}
