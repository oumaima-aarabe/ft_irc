#include "../server.hpp"

void ft_quit(commandInfo& cmd, Server& server, Client& client) 
{

    for (std::vector<Channel>::iterator it = client.channels_joined.begin(); it != client.channels_joined.end(); it++) 
    {
         std::cout << client.channels_joined.size() << " === channel joined n";
        if (it->getAllClientsList().size() > 1)
        {
            Logger::error("here size() > 1");
            std::cout << it->getAllClientsList().size() << "  clients size\n";
            it->removeClient(client);
            if (it->isOpe(client.nickname) && it->getOpeList().size() == 1)
            {
                std::cout << it->getOpeList().size() << "  OP size\n";
                it->removeOpe(client.nickname);
                it->addOpe(it->getAllClientsList()[0].nickname);
	            it->broadcastMessage(NULL, RPL_MODE(setPrefix(server.hostname, it->getAllClientsList()[0].nickname, it->getAllClientsList()[0].username), (it->getName() + " " + "+" + "o " + (it->getAllClientsList()[0].nickname))), false);
            }
            it->broadcastMessage(&client, RPL_PART(setPrefix(server.hostname, client.nickname, client.username), it->getName(), ""), false);
        }
        else
        {
            Logger::error("here size() = 1");
            std::cout << it->getAllClientsList().size() << "  clients size\n";
            it->removeClient(client);
            server.removeFromChannels(*it);
        }
    }
    client.quitAllChannels();
    if (cmd.cmnd_name != "JOIN")
    {
        server.sendReply(RPL_CUSTOM_QUIT(setPrefix(server.hostname, client.nickname, client.username),  (!cmd.cmnd_args.empty() ? (":Quit: " + cmd.cmnd_args[0]) : "Quit ")), client.fds.fd);
        server.removeClientFromServer(client);
        Logger::error("ERROR :Quit: " + (!cmd.cmnd_args.empty() ? (cmd.cmnd_args[0]) : ""));
        Logger::info("  Connection closed");
    }
}
// :pop!~u@qk3i8byd6tfyg.irc QUIT :Quit: bghit
// -  :appah!~u@qk3i8byd6tfyg.irc QUIT Quit
// <-  :pop!~u@qk3i8byd6tfyg.irc QUIT :Quit: bghit
// ERROR :Quit: bghit
// [ircws-tester] Connection closed
