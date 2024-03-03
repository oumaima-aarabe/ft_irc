#include "../headers/Server.hpp"

void ft_timeBot(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() > 0) {
        server.sendReply(ERR_NEEDMOREPARAMS(server.hostname, client.nickname, "TIME"), client.fds.fd);
        return ;
    }
    std::time_t now = std::time(NULL);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string current_time(buffer);
    server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostNames[client.fds.fd], client.nickname, client.realname), client.nickname,"Current time is: " + current_time), client.fds.fd);
}