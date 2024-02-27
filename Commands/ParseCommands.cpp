#include "../server.hpp"

typedef void (*CommandHandlerFunc)(commandInfo &, Server&, Client&); // function pointer typedef

bool isValidCommand(std::map<std::string, CommandHandlerFunc> commandHandlerMap, const std::string &cmdName)
{
	return commandHandlerMap.count(cmdName);
}

void Server::executeCommands(const std::vector<std::string> cmndBuffer, int clientFd) {
  Client client = users[clientFd];
	std::map<std::string, CommandHandlerFunc> commandHandlerMap;
	commandHandlerMap["PONG"] = ft_pong; //to ignore
	commandHandlerMap["JOIN"] = ft_join;
	commandHandlerMap["MODE"] = ft_mode;
	commandHandlerMap["PART"] = ft_part;
	commandHandlerMap["PRIVMSG"] = ft_privMsg;
	commandHandlerMap["QUIT"] = ft_quit; //requires registration
	commandHandlerMap["KICK"] = ft_kick; 
	commandHandlerMap["TOPIC"] = ft_topic;
	commandHandlerMap["INVITE"] = ft_invite;
	commandHandlerMap["NICK"] = ft_nick;
	commandHandlerMap["NOTICE"] = ft_notice;
  // loop through multiple commands sent by client in quick succession, which might be received and buffered by the server as a single string separated by '\n'
  for (size_t i = 0; i < cmndBuffer.size(); i++) 
	{
		commandInfo cmdInfo = parseCmndBuffer(cmndBuffer[i]);
		toUpper(cmdInfo.cmnd_name);
		if (!isValidCommand(commandHandlerMap, cmdInfo.cmnd_name))
		{
			sendReply(ERR_UNKNOWNCOMMAND(cmdInfo.cmnd_name), clientFd);
			continue;
		}
		client.buffer = cmndBuffer[i];
		commandHandlerMap[cmdInfo.cmnd_name](cmdInfo, *this, client); // execute the CommandHandlerFunc corresponding to the command name
	}
}

commandInfo parseCmndBuffer(const std::string &commandMessage) {
    commandInfo command;

    if (commandMessage.empty())
        return command;

    size_t trailingPartStartPos = commandMessage.find(" :");

    std::string middlePart = commandMessage.substr(0, trailingPartStartPos);

    std::vector<std::string> middleParams = split(middlePart, " ");
    if (!middleParams.empty()) {
        command.cmnd_name = middleParams[0];
        command.cmnd_args.insert(command.cmnd_args.end(), middleParams.begin() + 1, middleParams.end());
    }
    if (trailingPartStartPos != std::string::npos) { //if colon found
        std::string trailingPart = commandMessage.substr(trailingPartStartPos + 2); // +2 to skip the colon and the space before it
        command.cmnd_args.push_back(trailingPart);
    }
    return command;
}
