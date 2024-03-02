#include "../server.hpp"

//e.g.: INVITE NICK #CHANNEL      =>     INVITE is cmd.cmnd_name  ;   cmd.cmnd_args[0] is NICK  ;  cmd.cmnd_args[1] is #CHANNEL
void ft_invite(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() < 2)
	{
		server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	std::vector<Channel*>::iterator channelIter = server.getChannelByName(cmd.cmnd_args[1]);
    if (channelIter == server.channels.end()) //channel doesn't exist on server
	{
		server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, cmd.cmnd_args[1]), client.fds.fd);
		return ;
	}
	if (!(*channelIter)->isJoined(client.nickname)) //the client who sent the invite cmnd should be a channel member
	{
		server.sendReply(ERR_NOTONCHANNEL(std::string("*"), client.nickname, (*channelIter)->getName()), client.fds.fd);
		return ;
	}
	if (!(*channelIter)->isOpe(client.nickname)) // the client who sent the invite cmnd should be an operator
	{
		server.sendReply(ERR_CHANOPRIVSNEEDED(std::string("*"), client.nickname, (*channelIter)->getName()), client.fds.fd);
		return ;
	}
	std::map<int, Client>::iterator target = server.getClientByNickname(cmd.cmnd_args[0]);
	if (target == server.users.end()) //no such client nickname on server
	{
		server.sendReply(ERR_NOSUCHNICK(std::string("*"), client.nickname), client.fds.fd);
		return ;
	}
	if ((*channelIter)->isJoined(target->second.nickname)) //invited client is already in channel
	{
		server.sendReply(ERR_USERONCHANNEL(std::string("*"), target->second.nickname, target->second.username, (*channelIter)->getName()), client.fds.fd);
		return ;
	}
	(*channelIter)->invite(target->second);
	Logger::info("  " + client.nickname + " invited " + target->second.nickname + " to " + (*channelIter)->getName());
    server.sendReply(RPL_INVITING(std::string("*"), client.nickname, cmd.cmnd_args[0], cmd.cmnd_args[1]), client.fds.fd);
	server.sendReply(RPL_CUSTOM_INVITE(setPrefix(server.hostname, client.nickname, client.username), cmd.cmnd_args[0], (*channelIter)->getName()), target->second.fds.fd);
}
