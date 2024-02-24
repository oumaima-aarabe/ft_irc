#include "../server.hpp"

void ft_mode(commandInfo& cmd, Server &server, Client &client) {
    if (cmd.cmnd_args.size() < 1)
	{
		server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	if (!Channel::isValidChannelName(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	Channel *channel = server.getChannelByName(cmd.cmnd_args[0]);
	if (!channel)
	{
		server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	if (!channel->isJoined(client))
	{
		server.sendReply(ERR_NOTONCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	if (cmd.cmnd_args.size() == 1) // MODE #channel
	{
		server.sendReply(RPL_CHANNELMODEIS(std::string("*"), client.nickname, channel->getName(), channel->getStringModes()), client.fds.fd);
		return ;
	}
	if (!channel->isOpe(client))
	{
		server.sendReply(ERR_CHANOPRIVSNEEDED(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
    //loop through the arguments and add or remove the modes


}
