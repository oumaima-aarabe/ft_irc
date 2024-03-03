#include "../server.hpp"

//e.g.:    PART    <channel>{,<channel>}    [<reason>]
void ft_part(commandInfo& cmd, Server& server, Client& client) {
    if (!cmd.cmnd_args.size())
	{
		server.sendReply(ERR_NEEDMOREPARAMS(server.hostname, client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
	std::vector<std::string> channelsToLeave = split(cmd.cmnd_args[0], ",");
	for (size_t i = 0; i < channelsToLeave.size(); i++)
	{
		if (channelsToLeave[i].empty())
			continue ;
		if (!Channel::isValidChannelName(channelsToLeave[i]))
		{
			server.sendReply(ERR_NOSUCHCHANNEL(server.hostname, client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		std::vector<Channel*>::iterator channel = server.getChannelByName(channelsToLeave[i]);
		if (channel == server.channels.end()) //channel does't exist in server
		{
			server.sendReply(ERR_NOSUCHCHANNEL(server.hostname, client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		if (!(*channel)->isJoined(client.nickname))
		{
			server.sendReply(ERR_NOTONCHANNEL(server.hostname, client.nickname, channelsToLeave[i]), client.fds.fd);
			continue ;
		}
		std::string channelName = (*channel)->getName();
		if ((*channel)->getAllClientsList().size() > 1)
		{
			(*channel)->removeClient(client);
			if ((*channel)->isOpe(client.nickname) && (*channel)->getOpeList().size() == 1)
			{
				(*channel)->addOpe((*channel)->getAllClientsList()[0].nickname);
                (*channel)->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostNames[client.fds.fd], (*channel)->getAllClientsList()[0].nickname, (*channel)->getAllClientsList()[0].realname), ((*channel)->getName() + " " + "+" + "o " + ((*channel)->getAllClientsList()[0].nickname))), false);

			}
			(*channel)->removeOpe(client.nickname);
            client.removeChannel(*channel);
			server.sendReply(RPL_PART(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), channelName, (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")), client.fds.fd);
			(*channel)->broadcastMessage(&client, RPL_PART(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), channelName, (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")), false);
		}
		else // this client is the last member in the channel
		{
			(*channel)->removeClient(client);
            client.removeChannel(*channel);
            server.removeFromChannels(*channel);
			server.sendReply(RPL_PART(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), channelName, (cmd.cmnd_args.size() > 1 ? cmd.cmnd_args[1] : "")), client.fds.fd);
		}
	}
}