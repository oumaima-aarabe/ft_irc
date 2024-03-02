#include "../server.hpp"

void ft_privMsg(commandInfo& cmd, Server& server, Client& client) {

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
        std::vector<Channel*>::iterator channel = server.getChannelByName(cmd.cmnd_args[0]);

        if (channel == server.channels.end()) 
        {
            server.sendReply(ERR_NOSUCHCHANNEL(std::string("*"), client.nickname,cmd.cmnd_args[0]), client.fds.fd);
            return;
        }
        std::string channelName = (*channel)->getName();
        if (!(*channel)->isJoined(client.nickname))
        {
            server.sendReply(ERR_NOTONCHANNEL(std::string("*"), client.nickname, channelName), client.fds.fd);
            return;
        }
        (*channel)->broadcastMessage(&client, RPL_CUSTOM_PRIVMSG(setPrefix(server.hostname, client.nickname, client.realname), channelName, cmd.cmnd_args[1]), opeOnly);
    }
    else
    {
        std::map<int, Client>::iterator Receiver = server.getClientByNickname(cmd.cmnd_args[0]);
        if (Receiver  == server.users.end())
        {
            server.sendReply(ERR_NOSUCHNICK(std::string("*"), cmd.cmnd_args[0]), client.fds.fd);
            return;
        }
        server.sendReply(RPL_CUSTOM_PRIVMSG(setPrefix(server.hostname, client.nickname, client.realname), Receiver->second.nickname, cmd.cmnd_args[1]), Receiver->second.fds.fd);
    }
}