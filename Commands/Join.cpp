#include "../server.hpp"

std::vector<ChannelJoin> split_args(commandInfo& cmd)
{
    std::vector<std::string> channels = split(cmd.cmnd_args[0], ",");
    std::vector<std::string> keys;
    if (cmd.cmnd_args.size() > 1)
        keys = split(cmd.cmnd_args[1], ",");

    while (channels.size() < keys.size())
        keys.pop_back();
    while(channels.size() > keys.size())
        keys.push_back("");
    std::vector<ChannelJoin> channelJoins;
    for (size_t i = 0; i < channels.size(); ++i) 
    {
        ChannelJoin join;
        join.name = channels[i];
        join.key = keys[i];
        channelJoins.push_back(join);
    }
    return (channelJoins);
}

void joinReply(Server& server, Client& client, Channel channel) {
    channel.addClient(client);
    server.sendReply(setPrefix(server.hostname, client.nickname, client.username, client.buffer), client.fds.fd);
    server.sendReply(RPL_NAMREPLY(std::string("*"), client.nickname, std::string("="), channel.getName(), channel.listClients()), client.fds.fd);
    server.sendReply(RPL_ENDOFNAMES(std::string("*"), client.nickname, channel.getName()),client.fds.fd);
}

void ft_join(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() < 1)
    {
		server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
		return ;
	}
    std::vector<ChannelJoin> channels = split_args(cmd);
    for(size_t i = 0; i < channels.size(); i++)
    {
        if (!Channel::isValidChannelName(channels[i].name))
        {
            server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, channels[i].name), client.fds.fd);
            return ;
        }
        std::vector<Channel>::iterator ex_channel = server.getChannelByName(channels[i].name);
        if (ex_channel != server.channels.end()) //channel exits in server
        {
            //check if user is already in the channel
            if (ex_channel->isJoined(client.nickname))
            {
		        server.sendReply(ERR_USERONCHANNEL(std::string("*"), client.nickname, client.username, ex_channel->getName()), client.fds.fd);
		        return ;
	        }
            //check if channel is invite only
            if (ex_channel->isInviteOnly())
            {
                //check if user is in the invite list
                if (!ex_channel->isInvited(client.nickname))
                {
                    server.sendReply(ERR_INVITEONLYCHAN(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
                    return;
                }
                else {
                    ex_channel->removeInvite(client);  
                    joinReply(server, client, *ex_channel);
                }
            }
            //check if channel uses keys
            if (ex_channel->hasMode(CHANNEL_MODE_KEY))
            {
                if (ex_channel->getPassword() == channels[i].key) //if the key is correct
                {
                    //reply in case channel is maximized ERR_CHANNELISFULL and return

                    joinReply(server, client, *ex_channel);
                }
                else //wrong key reply
                    server.sendReply(ERR_BADCHANNELKEY(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
            }
            else
            {
                //reply in case channel is maximized ERR_CHANNELISFULL and return

                joinReply(server, client, *ex_channel);
            }
        }
        else //new channel
        {
            Channel new_channel = Channel(channels[i].name, "");
            new_channel.addClient(client);
            new_channel.addOpe(client.nickname);
            joinReply(server, client, new_channel);
        }
        //JOIN 0
    }
}
