#include "../server.hpp"

void ft_quit(commandInfo& cmd, Server& server, Client& client) 
{

    for (std::vector<Channel>::iterator it = client.channels_joined.begin(); it != client.channels_joined.end(); it++) 
    {
        if (it->getAllClientsList().size() > 1)
        {
            it->removeClient(client);
            if (it->isOpe(client.nickname) && it->getOpeList().size() == 1)
            {
                it->removeOpe(client.nickname);
                it->addOpe(it->getAllClientsList()[0].nickname);
            }
            client.removeChannel(*it);
        }
        else
        {
            it->removeClient(client);
            server.removeFromChannels(*it);
        }

    }
    client.quitAllChannels();
    server.removeClientFromServer(client);
}