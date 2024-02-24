#include "Channel.hpp"

Channel::Channel(std::string &name, std::string &password) : _name(name), _password(password), _channel_limit(MAX_CLIENTS_PER_CHANNEL)
{
    _modes.push_back(std::make_pair(CHANNEL_MODE_INVITE_ONLY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_KEY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_OPERATOR, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_USER_LIMIT, 0));
    updateStringModes();
}
  
// Setters
void Channel::setTopic(std::string &newTopic)
{
    _topic = newTopic;
}

void Channel::setPassword(std::string &passwd)
{
    _password = passwd;
}

void Channel::setChannel_limit(int limit)
{
    _channel_limit = limit;
}

// Getter
std::string Channel::getPassword(void) const 
{
    return (_password);
}

std::string Channel::getName(void) const
{
    return (_name);
}

std::string Channel::getTopic(void) const
{
    return (_topic);
}

int Channel::getChannelLimit(void) const
{
    return (_channel_limit);
}

std::vector<std::pair<ChannelMode, int> > Channel::getModes(void) const
{
    return (_modes);
}

std::string Channel::getStringModes(void) const {
    return (_stringModes);
}

std::vector<Client> Channel::getAllClientsList(void) const
{
    return (allClientsList);
}

std::vector<Client> Channel::getOpeList(void) const
{
    return (opeList);
}

std::vector<Client> Channel::getInviteList(void) const
{
    return (inviteList);
}

// --------------
// Client stuff
// --------------

void Channel::addClient(Client client)
{
    if (allClientsList.size() == (unsigned long)_channel_limit)
        return;
    if (isJoined(client) == false)
        return;
    if (isOpe(client) == false)
        opeList.push_back(client);
    if (isInvited(client) == false)
        inviteList.push_back(client);
    if (isJoined(client) == false)
        allClientsList.push_back(client);
}

bool Channel::isJoined(Client client)
{
    for (size_t i = 0; i < allClientsList.size(); i++) 
    {
        if (allClientsList[i].nickname == client.nickname)
            return true;
    }
    return false;
}

bool Channel::isOpe(Client client)
{
    for (size_t i = 0; i < opeList.size(); i++) 
    {
        if (opeList[i].nickname == client.nickname)
            return true;
    }
    return false;
}

bool Channel::isInvited(Client client)
{
    for (size_t i = 0; i < inviteList.size(); i++) 
    {
        if (inviteList[i].nickname == client.nickname)
            return true;
    }
    return false;
}

void Channel::addOpe(Client client)
{
    if (isJoined(client) == false){
        //error message
        return;
    }
    if (isOpe(client) == false)
        opeList.push_back(client);
}

void Channel::removeOpe(Client client)
{
    for (std::vector<Client >::iterator it = opeList.begin(); it != opeList.end(); it++)
    {
        if (it->fds.fd == client.fds.fd)
        {
            opeList.erase(it);
            return;
        }
    }
}

void Channel::kick(Client client)
{
    if (isOpe(client) == true)
        removeOpe(client);
    for (std::vector<Client >::iterator it = allClientsList.begin(); it != allClientsList.end(); it++)
    {
        if (it->fds.fd == client.fds.fd)
        {
            allClientsList.erase(it);
            return;
        }
    }
}

void Channel::invite(Client client)
{
    if (isInvited(client) == false && isJoined(client) == false)
        this->inviteList.push_back(client);
}

// ---------------------
// Channel modes stuff
// ---------------------

char Channel::getModeIdentifier(ChannelMode _mode) const
{
    char identifier = 0;
    switch (_mode)
    {
        case CHANNEL_MODE_INVITE_ONLY:
        identifier = 'i';
        break;
        case CHANNEL_MODE_KEY:
        identifier = 'k';
        break;
        case CHANNEL_MODE_OPERATOR:
        identifier = 'o';
        break;
        case CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY:
        identifier = 't';
        break;
        case CHANNEL_MODE_USER_LIMIT:
        identifier = 'l';
        break;
    }
    return (identifier);
}

void Channel::updateStringModes(void)
{
    _stringModes = " +";
    bool limit = false;
    for (std::vector<std::pair<ChannelMode, int> >::const_iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        char identifier = getModeIdentifier(it->first);
        if (identifier && identifier != 'o' && it->second == 1) { //o is a user mode, can't be displayed in channel modes set.
            _stringModes += identifier;
            if (identifier == 'l')
                limit = true;
        }
    }
    if (_stringModes.size() > 1 && limit == true)
        _stringModes += " " + std::to_string(_channel_limit);
    if (_stringModes.size() == 1)
        _stringModes = " no mode is set";
}

bool Channel::isInviteOnly(void)
{
    return (hasMode(CHANNEL_MODE_INVITE_ONLY));
}

bool Channel::hasMode(ChannelMode mode)
{
    for (std::vector <std::pair <ChannelMode, int> >::iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        if (it->first == mode) {
            if (it->second == 1)
                return true;
            else
                return false;
        }
    }
    return false;
}

void Channel::addMode(ChannelMode mode)
{
    for (std::vector <std::pair <ChannelMode, int> >::iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        if (it->first == mode) {
            if (it->second == 0)
                it->second = 1;
        }
    }
    updateStringModes();
}

void Channel::removeMode(ChannelMode mode)
{
    for (std::vector <std::pair <ChannelMode, int> >::iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        if (it->first == mode) {
            if (it->second == 1)
                it->second = 0;
        }
    }
    updateStringModes();
}

bool Channel::hasKey(void)
{
    return (hasMode(CHANNEL_MODE_KEY));
}

// --------------
// Utils
// --------------

void Channel::broadcastMessage(Client sender, std::string message)
{
    std::vector<Client > clients = this->getAllClientsList();
    for (size_t i = 0; i < clients.size(); i++) {
        if (sender.fds.fd && clients[i].fds.fd == sender.fds.fd)
			continue ;
        send(clients[i].fds.fd , message.c_str(), message.size() + 1, 0);
    }
}

bool Channel::isValidChannelName(const std::string name)
{
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ',' || name[i] == ' ')
			return false;
	}
	return true;
}

void Server::addToChannels(Channel& channel) 
{
    channels.push_back(channel);
}
