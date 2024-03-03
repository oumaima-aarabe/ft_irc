#include "../headers/Server.hpp"

void ft_privMsg(commandInfo& cmd, Server& server, Client& client) {

    if (cmd.cmnd_args.size() < 2)
    {
        server.sendReply(ERR_NOTEXTTOSEND(server.hostname, client.nickname), client.fds.fd);
        return;
    }
    cmd.cmnd_args.resize(2);
    if(cmd.cmnd_args[1].length() > 512)
    {
        server.sendReply(ERR_INPUTTOOLONG(server.hostname, client.nickname), client.fds.fd);
        return;
    }

    std::vector<std::string> targets = split(cmd.cmnd_args[0], ",");
    for (size_t i = 0; i < targets.size(); i++)
    {    
        if((targets[i][0] == '%' && targets[i][1] == '#') || targets[i][0] == '#')
        {
            bool opeOnly = false;
            if(targets[i][0] == '%')
            {
                targets[i].erase(0, 1);
                opeOnly = true;
            }
            std::vector<Channel*>::iterator channel = server.getChannelByName(targets[i]);

            if (channel == server.channels.end()) 
            {
                server.sendReply(ERR_NOSUCHCHANNEL(server.hostname, client.nickname,targets[i]), client.fds.fd);
                continue;;
            }
            std::string channelName = (*channel)->getName();
            if (!(*channel)->isJoined(client.nickname))
            {
                server.sendReply(ERR_NOTONCHANNEL(server.hostname, client.nickname, channelName), client.fds.fd);
                continue;;
            }
            (*channel)->broadcastMessage(&client, RPL_CUSTOM_PRIVMSG(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), channelName, cmd.cmnd_args[1]), opeOnly);
        }
        else
        {
            std::map<int, Client>::iterator Receiver = server.getClientByNickname(targets[i]);
            if (Receiver  == server.users.end())
            {
                server.sendReply(ERR_NOSUCHNICK(server.hostname, targets[i]), client.fds.fd);
                continue;;
            }
            server.sendReply(RPL_CUSTOM_PRIVMSG(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), Receiver->second.nickname, cmd.cmnd_args[1]), Receiver->second.fds.fd);
        }
    }
}