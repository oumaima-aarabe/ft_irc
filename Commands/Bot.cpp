#include "../server.hpp"

void ft_diceBot(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() != 1) {
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "To roll the dice, add an int param between 1 and 6!"), client.fds.fd);
        return ;
    }
    if (cmd.cmnd_args[0].find_first_not_of("0123456789") != std::string::npos) {
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "Only numbers please! : [1..6]"), client.fds.fd);
        return;
    }
    
    int guessedNumber = to_int(cmd.cmnd_args[0]);
    
    if (guessedNumber < 1 || guessedNumber > 6) {
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "Guessed number is out of range, can only be [1...6]"), client.fds.fd);
        return;
    }
    
    int diceRoll = rand() % 6 + 1;  // Rolling a dice
    
    server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "----- Welcome to Guess&Dice -----"), client.fds.fd);
    server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "🎲 Rolling Dice..."), client.fds.fd);
    if (guessedNumber == diceRoll) {
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "🎉 Congratulations, You guessed it right!"), client.fds.fd);
    } else {
        server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname, "😔 Wrong guess! The correct number was: " + to_string(diceRoll)), client.fds.fd);
    }
}

void ft_timeBot(commandInfo& cmd, Server& server, Client& client) {
    if (cmd.cmnd_args.size() > 0) {
        server.sendReply(ERR_NEEDMOREPARAMS(server.hostname, client.nickname, "TIME"), client.fds.fd);
        return ;
    }
    std::time_t now = std::time(NULL);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string current_time(buffer);
    server.sendReply(RPL_CUSTOM_NOTICE(setPrefix(server.hostname, client.nickname, client.realname), client.nickname,"Current time is: " + current_time), client.fds.fd);
}