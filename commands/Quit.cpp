#include "../headers/Server.hpp"

void ft_quit(commandInfo& cmd, Server& server, Client& client) 
{
    for (std::vector<Channel*>::iterator it = client.channels_joined.begin(); it != client.channels_joined.end(); it++)
    {
        if (!(*it)->isJoined(client.nickname))
        {
            if ((*it)->isInvited(client.nickname))
                (*it)->removeInvite(client);
            continue;
        }
        if ((*it)->getAllClientsList().size() > 1)
        {
            (*it)->removeClient(client);
            if ((*it)->isOpe(client.nickname) && (*it)->getOpeList().size() == 1)
            {
                (*it)->addOpe((*it)->getAllClientsList()[0].nickname);
                (*it)->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostNames[client.fds.fd], (*it)->getAllClientsList()[0].nickname, (*it)->getAllClientsList()[0].realname), ((*it)->getName() + " +o " + ((*it)->getAllClientsList()[0].nickname))), false);
            }
            (*it)->removeOpe(client.nickname);
            (*it)->broadcastMessage(&client, RPL_PART(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), (*it)->getName(), ""), false);
        }
        else
        {
            (*it)->removeClient(client);
            server.removeFromChannels(*it);
        }
    }
    client.quitAllChannels();
    toUpper(cmd.cmnd_name);
    if (cmd.cmnd_name == "QUIT")
    {
        server.sendReply(RPL_CUSTOM_QUIT(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname),  (!cmd.cmnd_args.empty() ? (":Quit: " + cmd.cmnd_args[0]) : "Quit ")), client.fds.fd);
        server.removeClientFromServer(client);
        Logger::info(":Quit: " + (!cmd.cmnd_args.empty() ? (cmd.cmnd_args[0]) : ""));
        Logger::info("Connection closed");
    }
}
