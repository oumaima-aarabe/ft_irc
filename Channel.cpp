#include "Channel.hpp"

Channel::Channel(std::string &name, std::string &password) : _name(name), _password(password), _channel_limit(MAX_CLIENTS_PER_CHANNEL)
{
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

std::vector<ChannelMode> Channel::getModes(void) const
{
    return (_modes);
}

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

std::string Channel::getStringModes(void) const
{
    std::string stringModes = "";
    // for (std::vector<ChannelMode>::const_iterator it = _modes.begin(); it != _modes.end(); ++it)
    // {
    //     char identifier = getModeIdentifier(*it);
    //     if (identifier)
    //     stringModes += getModeIdentifier(*it);
    // }
    return (stringModes);
}

int Channel::getChannelLimit(void) const
{
    return (_channel_limit);
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
    if (std::find(allClientsList.begin(), allClientsList.end(), client) == allClientsList.end())
        allClientsList.push_back(client);
}

bool Channel::isJoined(Client client)
{
    std::vector<Client > clients = getAllClientsList();
    return (std::find(clients.begin(), clients.end(), client) != clients.end());
}

bool Channel::isOpe(Client client)
{
    return (std::find(opeList.begin(), opeList.end(), client) != opeList.end());
}

bool Channel::isInvited(Client client)
{
    return (std::find(inviteList.begin(), inviteList.end(), client) != inviteList.end());
}

void Channel::addOpe(Client client)
{
    if (isJoined(client) == false){
        //error message 
        return;
    }
    if (std::find(opeList.begin(), opeList.end(), client) == opeList.end())
        opeList.push_back(client);
}

void Channel::removeOpe(Client client)
{
    std::vector<Client >::iterator it = std::find(opeList.begin(), opeList.end(), client);
    if (it != opeList.end())
        opeList.erase(it);
    if (std::find(allClientsList.begin(), allClientsList.end(), client) == allClientsList.end())
        allClientsList.push_back(client);
}

void Channel::kick(Client client)
{
    std::vector<Client >::iterator it = std::find(allClientsList.begin(), allClientsList.end(), client);
    if (it != allClientsList.end())
        allClientsList.erase(it);
    it = std::find(opeList.begin(), opeList.end(), client);
    if (it != opeList.end())
        opeList.erase(it);
}

void Channel::invite(Client client)
{
    if (std::find(inviteList.begin(), inviteList.end(), client) == inviteList.end())
        this->inviteList.push_back(client);
}

void Channel::removeInvite(Client client)
{
    std::vector<Client >::iterator it =
    std::find(inviteList.begin(), inviteList.end(), client);
    if (it != inviteList.end())
        inviteList.erase(it);
}

// --------------
// Channel modes stuff
// --------------
bool Channel::isInviteOnly(void)
{
    return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_INVITE_ONLY) != _modes.end());
}

// --------------
// Utils
// --------------
bool Channel::hasMode(ChannelMode mode)
{
    return std::find(this->_modes.begin(), this->_modes.end(), mode) != this->_modes.end();
}

void Channel::addMode(ChannelMode mode)
{
    if (!this->hasMode(mode))
        this->_modes.push_back(mode);
}

void Channel::removeMode(ChannelMode mode)
{
    std::vector<ChannelMode>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
    if (it != _modes.end())
        _modes.erase(it);
}

void Channel::broadcastMessage(std::string message)
{
    std::vector<Client > clients = this->getAllClientsList();
    for (size_t i = 0; i < clients.size(); i++)
        send(clients[i].fds.fd , message.c_str(), message.size() + 1, 0);
}

bool Channel::hasKey(void)
{
    return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_KEY) != _modes.end());
}

void Channel::removeKey(void)
{
    std::vector<ChannelMode>::iterator it = std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_KEY);
    if (it != _modes.end())
        _modes.erase(it);
}

void Server::addToChannels(const Channel& channel) 
{
    channels.push_back(channel);
}
