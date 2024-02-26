#include "../server.hpp"

//e.g.:    PART    <channel>{,<channel>}    [<reason>]
void ft_part(commandInfo& cmd, Server& server, Client& client) {
    if (!cmd.cmnd_args.size())
	{
		server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	std::vector<std::string> channelsToLeave = split(cmd.cmnd_args[0], ",");
	for (size_t i = 0; i < channelsToLeave.size(); i++)
	{
		if (!Channel::isValidChannelName(channelsToLeave[i]))
		{
			server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		std::vector<Channel>::iterator channel = server.getChannelByName(channelsToLeave[i]);
		if (channel == server.channels.end()) //channel does't exist in server
		{
			server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		if (!channel->isJoined(client.nickname))
		{
			server.sendReply(ERR_NOTONCHANNEL(std::string("*"), client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		std::string channelName = channel->getName();
		if (channel->getAllClientsList().size() > 1)
		{
			channel->removeClient(client);
            client.removeChannel(*channel);
			server.sendReply(RPL_PART(setPrefix(server.hostname, client.nickname, client.username, ""), channelName, (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")), client.fds.fd);
			channel->broadcastMessage(&client, RPL_PART(setPrefix(server.hostname, client.nickname, client.username, ""), channel->getName(), (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")));
		}
		else // this client is the last member in the channel
		{
			channel->removeClient(client);
            client.removeChannel(*channel);
            server.removeFromChannels(*channel);
			server.sendReply(RPL_PART(setPrefix(server.hostname, client.nickname, client.username, ""), channelName, (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")), client.fds.fd);
		}
	}
}