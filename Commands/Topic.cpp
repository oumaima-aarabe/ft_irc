#include "../server.hpp"

// TOPIC   <channel>   [ <topic> ]
void ft_topic(commandInfo& cmd, Server& server, Client& client) {
    if (!cmd.cmnd_args.size())
	{
		server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	if (!Channel::isValidChannelName(cmd.cmnd_args[0]))
	{
		server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	std::vector<Channel>::iterator channel = server.getChannelByName(cmd.cmnd_args[0]);
	if (channel == server.channels.end())
	{
		server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[0]), client.fds.fd);
		return ;
	}
	if (!channel->isJoined(client.nickname))
	{
		server.sendReply(ERR_NOTONCHANNEL(std::string("*"), client.nickname, channel->getName()), client.fds.fd);
		return ;
	}
	if (channel->hasMode(CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY) && !channel->isOpe(client.nickname))
	{
		server.sendReply(ERR_CHANOPRIVSNEEDED(std::string("*"), client.nickname, channel->getName()), client.fds.fd);
		return ;
	}
	if (cmd.cmnd_args.size() > 1) // in case of   "TOPIC <channel> :"   or   "TOPIC <channel> :<topic>"
		channel->setTopic(cmd.cmnd_args[1]);
	else // in case of    "TOPIC <channel>"    display the channel topic
	{
		server.sendReply(RPL_TOPIC(std::string("*"), client.nickname, channel->getName(), channel->getTopic()), client.fds.fd);
		return ;
	}
	if (channel->getTopic().empty()) // in case of "TOPIC <channel> :" delete the channels topic (empty string)
		channel->broadcastMessage(&client, RPL_NOTOPIC(std::string("*"), client.nickname, channel->getName()), false);
	else
		channel->broadcastMessage(&client, RPL_TOPIC(std::string("*"), client.nickname, channel->getName(), channel->getTopic()), false);
}