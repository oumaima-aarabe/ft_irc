#include "../server.hpp"

std::vector<ChannelJoin> split_args(commandInfo &cmd)
{
    std::vector<std::string> channels = split(cmd.cmnd_args[0], ",");
    std::vector<std::string> keys;
    if (cmd.cmnd_args.size() > 1)
        keys = split(cmd.cmnd_args[1], ",");
    while (channels.size() < keys.size())
        keys.pop_back();
    while (channels.size() > keys.size())
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

bool joinReply(Server &server, Client &client, Channel channel, bool new)
{
    int is_joined = channel.addClient(client);
    if (!is_joined)
        return (false);
    else if (is_joined < 0)
    {
        //channels maximized msg;
        return (false);
    }
    if (new)
        channel.addOpe(client.nickname);
    server.sendReply(setPrefix(server.hostname, client.nickname, client.username, client.buffer), client.fds.fd);
    server.sendReply(RPL_NAMREPLY(std::string("*"), client.nickname, std::string("="), channel.getName(), channel.listClients()), client.fds.fd);
    server.sendReply(RPL_ENDOFNAMES(std::string("*"), client.nickname, channel.getName()), client.fds.fd);
}

void ft_join(commandInfo &cmd, Server &server, Client &client)
{
    if (cmd.cmnd_args.size() < 1)
    {
        server.sendReply(ERR_NEEDMOREPARAMS(std::string("*"), client.nickname, cmd.cmnd_name), client.fds.fd);
        return;
    }
    std::vector<ChannelJoin> channels = split_args(cmd);
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (!Channel::isValidChannelName(channels[i].name))
        {
            server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname, channels[i].name), client.fds.fd);
            return;
        }
        std::vector<Channel>::iterator ex_channel = server.getChannelByName(channels[i].name);

        if (ex_channel == server.channels.end()) { // channel doesnt exit in server 
            Logger::debug("User [" + client.nickname + "] is creating a new channel [" + channels[i].name + "] with key [" + channels[i].key + "] and joining it.");
            Channel new_channel = Channel(channels[i].name, "");
            joinReply(server, client, new_channel, true);
            server.channels.push_back(new_channel);
            return;
        }

        if (ex_channel->isJoined(client.nickname)) {
            Logger::debug("User [" + client.nickname + "] is already in the channel [" + channels[i].name + "].");
            server.sendReply(ERR_USERONCHANNEL(std::string("*"), client.nickname, client.username, ex_channel->getName()), client.fds.fd);
            return;
        }


        // reply in case channel is maximized ERR_CHANNELISFULL and return
            
            
        // check if channel uses keys
        if (ex_channel->hasKey() && (ex_channel->getPassword() != channels[i].key)) { // wrong key reply
            Logger::debug("User [" + client.nickname + "] is trying to join the channel [" + channels[i].name + "] with the wrong key [" + channels[i].key + "].");
            server.sendReply(ERR_BADCHANNELKEY(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
            return ;
        }
        // check if channel is invite only
        if (ex_channel->isInviteOnly()) {
            // check if user is in the invite list
            if (!ex_channel->isInvited(client.nickname))
            {
                Logger::debug("User [" + client.nickname + "] is trying to join the invite only channel [" + channels[i].name + "].");
                server.sendReply(ERR_INVITEONLYCHAN(std::string("*"), client.nickname, ex_channel->getName()), client.fds.fd);
                return;
            }
            ex_channel->removeInvite(client);
        }
        Logger::debug("User [" + client.nickname + "] is joining the channel [" + channels[i].name + "].");
        joinReply(server, client, *ex_channel, false);
    }
    // case JOIN 0
}
