#include "../server.hpp"

void ft_notice(commandInfo& cmd, Server& server, Client& client) {

    if (cmd.cmnd_args.size() < 1)
    {
        server.sendReply(ERR_NOTEXTTOSEND(std::string("*"), client.nickname), client.fds.fd);
        return;
    }

    cmd.cmnd_args.resize(2);
    if(cmd.cmnd_args[1].length() > 512)
    {
        server.sendReply(ERR_INPUTTOOLONG(std::string("*"), client.nickname), client.fds.fd);
        return;
    }
    if((cmd.cmnd_args[0][0] == '%' && cmd.cmnd_args[0][1] == '#') || cmd.cmnd_args[0][0] == '#')
    {
        bool opeOnly = false;
        if(cmd.cmnd_args[0][0] == '%')
        {
            cmd.cmnd_args[0].erase(0, 1);
            opeOnly = true;
        }
        std::vector<Channel>::iterator channel = server.getChannelByName(cmd.cmnd_args[0]);

        if (channel == server.channels.end()) 
            return;
        std::string channelName = channel->getName();
        if (!channel->isJoined(client.nickname))
            return;
        channel->broadcastMessage(&client, RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.username), channelName, cmd.cmnd_args[1]), opeOnly);
    }
    else
    {
        std::map<int, Client>::iterator Receiver = server.getClientByNickname(cmd.cmnd_args[0]);
        if (Receiver  == server.users.end())
            return;
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.username), Receiver->second.nickname, cmd.cmnd_args[1]), Receiver->second.fds.fd);
    }
}