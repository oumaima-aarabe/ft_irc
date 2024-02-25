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
        Channel *ex_channel = server.getChannelByName(channels[i].name);
        if (ex_channel)
        {
            //check if user is already in the channel
            if (ex_channel->isJoined(client.nickname))
                return;

            //check if channel is invite only 
            if (ex_channel->hasMode(CHANNEL_MODE_INVITE_ONLY))
            {
                //check if user is in the invite list
                if (!ex_channel->isInvited(client.nickname))
                {
                    server.sendReply(ERR_INVITEONLYCHAN(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
                    //remove for invite list
                    return;
                }
            }
            //check if channel uses keys
            if (ex_channel->hasMode(CHANNEL_MODE_KEY))
            {
                //if the key is correct
                if (ex_channel->getPassword() == channels[i].key)
                {
                    ex_channel->addClient(client);
                    //reply in case channel is maximized
                    //prefix reply
                    server.sendReply(setPrefix(server.hostname, client.nickname, client.username, client.buffer), client.fds.fd); //should use server fd?
                    //display modes
                    server.sendReply(RPL_NAMREPLY(std::string("*"), client.nickname, std::string("="), ex_channel->getName(), ex_channel->listClients()), client.fds.fd);
                    server.sendReply(RPL_ENDOFNAMES(std::string("*"), client.nickname, ex_channel->getName()),client.fds.fd);
                }
                else //wrong key reply
                    server.sendReply(ERR_BADCHANNELKEY(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
            }
            else
            {
                ex_channel->addClient(client);
                //reply in case channel is maximized
                //joned reply
                server.sendReply(RPL_NAMREPLY(std::string("*"), client.nickname, std::string("="), ex_channel->getName(), ex_channel->listClients()), client.fds.fd);
                server.sendReply(RPL_ENDOFNAMES(std::string("*"), client.nickname, ex_channel->getName()),client.fds.fd);
            }
        }
        else //new channel
        {
            Channel new_channel = Channel(channels[i].name, "");
            new_channel.addClient(client);
            //reply in case channel is maximized //return;
            new_channel.addOpe(client.nickname);
            server.addToChannels(new_channel);
            //reply msgs
            server.sendReply(RPL_NAMREPLY(std::string("*"), client.nickname, std::string("="), new_channel.getName(), new_channel.listClients()), client.fds.fd);
            server.sendReply(RPL_ENDOFNAMES(std::string("*"), client.nickname, new_channel.getName()),client.fds.fd);
        }
        //JOIN 0
    }
}
