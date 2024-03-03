#include "../headers/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password) : _name(name), _password(password), _channel_limit(MAX_CLIENTS_PER_CHANNEL)
{
    _modes.push_back(std::make_pair(CHANNEL_MODE_INVITE_ONLY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_KEY, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_OPERATOR, 0));
    _modes.push_back(std::make_pair(CHANNEL_MODE_USER_LIMIT, 0));
    updateStringModes();
}
  
// Setters
void Channel::setTopic(std::string const &newTopic)
{
    _topic = newTopic;
}

void Channel::setPassword(std::string const &passwd)
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

std::vector<Client> &Channel::getAllClientsList(void)
{
    return (allClientsList);
}

std::vector<Client> &Channel::getOpeList(void)
{
    return (opeList);
}

std::vector<Client> &Channel::getInviteList(void)
{
    return (inviteList);
}

// --------------
// Client stuff
// --------------

int Channel::addClient(Client const &client)
{
    if (allClientsList.size() >= (size_t)_channel_limit)
        return (-2);
    if (!isJoined(client.nickname))
    {
        allClientsList.push_back(client);
        return (1);
    }
    return 0;
}
std::string Channel::listClients()
{
    std::string list_users;
    for (size_t i = 0; i < allClientsList.size(); i++)
    {
        if (isOpe(allClientsList[i].nickname))
            list_users += "@";
        list_users += allClientsList[i].nickname;
        list_users += " ";
    }
    return list_users;
}

bool Channel::isJoined(std::string const &nickname)
{
    for (size_t i = 0; i < allClientsList.size(); i++) 
    {
        if (allClientsList[i].nickname == nickname)
            return true;
    }
    return false;
}

bool Channel::isOpe(std::string const &nickname)
{
    for (size_t i = 0; i < opeList.size(); i++) 
    {
        if (opeList[i].nickname == nickname)
            return true;
    }
    return false;
}

bool Channel::isInvited(std::string const &nickname)
{
    for (size_t i = 0; i < inviteList.size(); i++) 
    {
        if (inviteList[i].nickname == nickname)
            return true;
    }
    return false;
}

void Channel::addOpe(std::string const &nickname)
{
    if (isJoined(nickname) && isOpe(nickname) == false) {
        for (size_t i = 0; i < allClientsList.size(); i++)
        {
            if (allClientsList[i].nickname == nickname)
            {
                opeList.push_back(allClientsList[i]);
                return;
            }
        }
    }
}

void Channel::removeOpe(std::string const &nickname)
{
    for (std::vector<Client >::iterator it = opeList.begin(); it != opeList.end(); it++)
    {
        if (it->nickname == nickname)
        {
            opeList.erase(it);
            return;
        }
    }
}

void Channel::removeClient(Client const &client)
{
    for (std::vector<Client>::iterator it = allClientsList.begin(); it != allClientsList.end(); it++)
    {
        if (it->fds.fd == client.fds.fd)
        {
            allClientsList.erase(it);
            return;
        }
    }
}

void Channel::invite(Client const &client)
{
    if (!isJoined(client.nickname))
        this->inviteList.push_back(client);
}

void Channel::removeInvite(Client const &client) {
    for (std::vector<Client>:: iterator it = inviteList.begin(); it != inviteList.end(); it++) {
        if (it->nickname == client.nickname) {
            inviteList.erase(it);
            return ;
        }
    }
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
    _stringModes = "";
    bool limitFlag = false;
    bool keyFlag = false;
    for (std::vector<std::pair<ChannelMode, int> >::const_iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        char identifier = getModeIdentifier(it->first);
        if (identifier && identifier != 'o' && it->second == 1) //'o' is a user mode, can't be displayed in channel modes set.
        {
            _stringModes += identifier;
            if (identifier == 'l')
                limitFlag = true;
            else if (identifier == 'k')
                keyFlag = true;           
        }
    }
    if (_stringModes.size() > 0 && keyFlag)
        _stringModes += " " + _password;
    if (_stringModes.size() > 0 && limitFlag)
        _stringModes += " " + to_string(_channel_limit);
    else if (_stringModes.size() == 0)
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

void Channel::broadcastMessage(Client *sender, std::string message, bool opeOnly)
{
    std::vector<Client > clients;
    if (opeOnly)
        clients = this->getOpeList();
    else
        clients = this->getAllClientsList();
    for (size_t i = 0; i < clients.size(); i++) {
        if (sender && sender->fds.fd && clients[i].fds.fd == sender->fds.fd)
            continue ;
        send(clients[i].fds.fd , message.c_str(), message.size(), 0);
    }
}
        void Channel::updateNick(std::string oldNick, std::string newNick, std::vector<Client> &clients)
        {
            for (size_t i = 0; i < clients.size(); i++)
            {
                if (clients[i].nickname == oldNick)
                    clients[i].nickname = newNick;
            }
        }
void Server::broadcastMessage(Client *sender, std::string message, std::vector<Client> chared_channels)
{
    std::vector<Client>::iterator it;
    for (it = chared_channels.begin();it != chared_channels.end();it++)
    {
        if (sender && sender->fds.fd && it->fds.fd == sender->fds.fd)
            continue;
        send(it->fds.fd, message.c_str(), message.size(), 0);
    }
}

bool Channel::isValidChannelName(const std::string &name)
{
	if (name[0] != '#')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ',' || name[i] == ' ')
			return false;
	}
	return true;
}

void Server::addToChannels(Channel *channel) 
{
    channels.push_back(channel);
}

void Server::removeFromChannels(Channel *channel) 
{
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->getName() == channel->getName())
        {
            Channel *tmp = *it;
            channels.erase(it);
            delete tmp;
            return;
        }
    }
}

std::vector<Channel*>::iterator Server::getChannelByName(const std::string &name)
{
  for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
    if ((*it)->getName() == name)
      return it;
  }
  return channels.end();
}

Channel::~Channel()
{
    allClientsList.clear();
    opeList.clear();
    inviteList.clear();
}
