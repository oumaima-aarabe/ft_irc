#include "../server.hpp"

void ft_quit(commandInfo& cmd, Server& server, Client& client) 
{
    std::vector<Client> chared_channels;


    for (std::vector<Channel>::iterator it = client.channels_joined.begin(); it != client.channels_joined.end(); it++) 
    {
        for(std::vector<Client >::iterator i = it->getAllClientsList().begin(); i != it->getAllClientsList().end(); i++)
        {
            for(std::vector<Client >::iterator j = chared_channels.begin(); j != chared_channels.end(); j++)
            {
                if (joined[i].nickName == chared_channels[j].nickname)
                    break;
            }
            if (j != chared_channels.end())
                chared_channels.push_back(*it);
            
        }

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