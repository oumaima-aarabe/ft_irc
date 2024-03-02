#include "../server.hpp"

// void ft_quit(commandInfo& cmd, Server& server, Client& client) 
// {

//     for (std::vector<Channel>::iterator it = server.channels.begin(); it != server.channels.end(); it++)
//     {
//         // if ((*it)->)÷
//         std::cout << "Channel: " << (*it)->getAllClientsList().size() << std::endl;
//         if (!it->isJoined(client.nickname))
//         {
//             if (it->isInvited(client.nickname))
//                 it->removeInvite(client);
//             continue;
//         }
//         if ((*it)->getAllClientsList().size() > 1)
//         {
//             it->removeClient(client);
//             if (it->isOpe(client.nickname) && (*it)->getOpeList().size() == 1)
//             {
//                 it->removeOpe(client.nickname);
//                 it->addOpe((*it)->getAllClientsList()[0].nickname);
// 	            it->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostname, (*it)->getAllClientsList()[0].nickname, (*it)->getAllClientsList()[0].username), ((*it)->getName() + " " + "+" + "o " + ((*it)->getAllClientsList()[0].nickname))), false);
//             }
//             it->broadcastMessage(&client, RPL_PART(setPrefix(server.hostname, client.nickname, client.username), (*it)->getName(), ""), false);
//         }
//         else
//         {
//             it->removeClient(client);
//             server.removeFromChannels(*it);
//         }
//     }
//     client.quitAllChannels();
//     toUpper(cmd.cmnd_name);
//     if (cmd.cmnd_name != "JOIN")
//     {
//         server.sendReply(RPL_CUSTOM_QUIT(setPrefix(server.hostname, client.nickname, client.username),  (!cmd.cmnd_args.empty() ? (":Quit: " + cmd.cmnd_args[0]) : "Quit ")), client.fds.fd);
//         server.removeClientFromServer(client);
//         Logger::info("ERROR :Quit: " + (!cmd.cmnd_args.empty() ? (cmd.cmnd_args[0]) : ""));
//         Logger::info("Connection closed");
//     }
// }
// :pop!~u@qk3i8byd6tfyg.irc QUIT :Quit: bghit
// -  :appah!~u@qk3i8byd6tfyg.irc QUIT Quit
// <-  :pop!~u@qk3i8byd6tfyg.irc QUIT :Quit: bghit
// ERROR :Quit: bghit
// [ircws-tester] Connection closed


void ft_quit(commandInfo& cmd, Server& server, Client& client) 
{
    (void)cmd;
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
                (*it)->removeOpe(client.nickname);
                (*it)->addOpe((*it)->getAllClientsList()[0].nickname);
                (*it)->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostname, (*it)->getAllClientsList()[0].nickname, (*it)->getAllClientsList()[0].username), ((*it)->getName() + " " + "+" + "o " + ((*it)->getAllClientsList()[0].nickname))), false);
            }
            (*it)->broadcastMessage(&client, RPL_PART(setPrefix(server.hostname, client.nickname, client.username), (*it)->getName(), ""), false);
        }
        else
        {
            (*it)->removeClient(client);
            server.removeFromChannels(*it);
        }
    }
    // for (std::vector<Channel*>::iterator it = server.channels.begin(); it != server.channels.end(); it++)
    // {
    //     // if (it->)÷
    //     std::cout << "Channel: " << (*it)->getAllClientsList().size() << std::endl;
    //     if (!(*it)->isJoined(client.nickname))
    //     {
    //         if ((*it)->isInvited(client.nickname))
    //             (*it)->removeInvite(client);
    //         continue;
    //     }
    //     if ((*it)->getAllClientsList().size() > 1)
    //     {
    //         (*it)->removeClient(client);
    //         if ((*it)->isOpe(client.nickname) && (*it)->getOpeList().size() == 1)
    //         {
    //             (*it)->removeOpe(client.nickname);
    //             (*it)->addOpe((*it)->getAllClientsList()[0].nickname);
	//             (*it)->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostname, (*it)->getAllClientsList()[0].nickname, (*it)->getAllClientsList()[0].username), ((*it)->getName() + " " + "+" + "o " + ((*it)->getAllClientsList()[0].nickname))), false);
    //         }
    //         (*it)->broadcastMessage(&client, RPL_PART(setPrefix(server.hostname, client.nickname, client.username), (*it)->getName(), ""), false);
    //     }
    //     else
    //     {
    //         (*it)->removeClient(client);
    //         server.removeFromChannels(*it);
    //     }
    // }
    client.quitAllChannels();
    toUpper(cmd.cmnd_name);
    if (cmd.cmnd_name != "JOIN")
    {
        server.sendReply(RPL_CUSTOM_QUIT(setPrefix(server.hostname, client.nickname, client.username),  (!cmd.cmnd_args.empty() ? (":Quit: " + cmd.cmnd_args[0]) : "Quit ")), client.fds.fd);
        server.removeClientFromServer(client);
        Logger::info("ERROR :Quit: " + (!cmd.cmnd_args.empty() ? (cmd.cmnd_args[0]) : ""));
        Logger::info("Connection closed");
    }
}