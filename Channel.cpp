#include "Channel.hpp"

Channel::Channel(std::string &name, std::string &password) : _name(name), _password(password), _creator(NULL), _user_limit(MAX_CLIENTS_PER_CHANNEL)
{
    _created_at = time(0);
    if (password.size() > 0)
        addMode(CHANNEL_MODE_KEY);
};
  
// Setters
void Channel::setCreator(Client *creator)
{
    if (_creator)
        throw std::runtime_error("Creator is already assigned");
    _creator = creator;
    _ope_clients.push_back(creator);
};
void Channel::setTopic(std::string &topic)
{
    _topic = topic;
};
void Channel::setPassword(std::string passwd)
{
    _password = passwd;
};
// Getter
std::string &Channel::getPassword(void)
{
    return (_password);
}
std::string &Channel::getName(void)
{
    return (_name);
}
std::string &Channel::getTopic(void)
{
    return (_topic);
}
Client *Channel::getCreator(void)
{
    return (_creator);
}
time_t Channel::getCreatedAt(void)
{
    return (_created_at);
}
ChannelMode Channel::getMode(void)
{
    return (_mode);
}
std::vector<ChannelMode> Channel::getModes(void)
{
    return (_modes);
}
char Channel::getIdentifier(ChannelMode _mode)
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
std::string Channel::getStringModes(void)
{
    std::string stringModes = "";
    for (std::vector<ChannelMode>::const_iterator it = _modes.begin(); it != _modes.end(); ++it)
    {
        char identifier = getIdentifier(*it);
        if (identifier)
        stringModes += getIdentifier(*it);
    }
    return (stringModes);
}
size_t Channel::getUserLimit(void)
{
    return (_user_limit);
}
void Channel::setUserLimit(size_t limit)
{
    _user_limit = limit;
}
std::vector<Client *> Channel::getClients(void) const
{
    std::vector<Client *> clients;
    clients.insert(clients.end(), _normal_clients.begin(), _normal_clients.end());
    clients.insert(clients.end(), _ope_clients.begin(), _ope_clients.end());
    return (clients);
}
// --------------
// Client stuff
// --------------
bool Channel::joined(Client *client)
{
    std::vector<Client *> clients = this->getClients();
    return (std::find(clients.begin(), clients.end(), client) != clients.end());
}
bool Channel::isOwner(Client *client)
{
    return (_creator == client);
}
bool Channel::isOpe(Client *client)
{
    return (std::find(_ope_clients.begin(), _ope_clients.end(), client) != _ope_clients.end());
}
bool Channel::isNormal(Client *client)
{
    return (std::find(_normal_clients.begin(), _normal_clients.end(), client) != _normal_clients.end());
}
bool Channel::isInvited(Client *client)
{
    return (std::find(_invite_list.begin(), _invite_list.end(), client) != _invite_list.end());
}
bool Channel::isInviteOnly(void)
{
    return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_INVITE_ONLY) != _modes.end());
}
void Channel::addOpe(Client *client)
{
    std::vector<Client *>::iterator it =
    std::find(_normal_clients.begin(), _normal_clients.end(), client);
    if (it != _normal_clients.end())
        _normal_clients.erase(it);
    if (std::find(_ope_clients.begin(), _ope_clients.end(), client) == _ope_clients.end())
        _ope_clients.push_back(client);
}
void Channel::removeOpe(Client *client)
{
    std::vector<Client *>::iterator it =
    std::find(_ope_clients.begin(), _ope_clients.end(), client);
    if (it != _ope_clients.end())
        _ope_clients.erase(it);
    if (std::find(_normal_clients.begin(), _normal_clients.end(), client) == _normal_clients.end())
        _normal_clients.push_back(client);
}
void Channel::kick(Client *client)
{
    std::vector<Client *>::iterator it =
    std::find(_normal_clients.begin(), _normal_clients.end(), client);
    if (it != _normal_clients.end())
        _normal_clients.erase(it);
    it = std::find(_ope_clients.begin(), _ope_clients.end(), client);
    if (it != _ope_clients.end())
        _ope_clients.erase(it);
}
void Channel::invite(Client *client)
{
    if (std::find(_invite_list.begin(), _invite_list.end(), client) == _invite_list.end())
        this->_invite_list.push_back(client);
}
void Channel::removeInvite(Client *client)
{
    std::vector<Client *>::iterator it =
    std::find(_invite_list.begin(), _invite_list.end(), client);
    if (it != _invite_list.end())
        _invite_list.erase(it);
}
// --------------
// Utils
// --------------
std::string Channel::getModeString(void)
{
  std::string mode;
  for (size_t i = 0; i < _modes.size(); i++)
  {
    if (_modes[i] == CHANNEL_MODE_INVITE_ONLY)
        mode += "!";
    else if (_modes[i] == CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY)
        mode += "+";
    else if (_modes[i] == CHANNEL_MODE_KEY)
        mode += "!";
  }
  std::cout << "MODE: " << mode << std::endl;
  return (mode);
}
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
void Channel::removeClientFromChannel(Client *client)
{
    std::vector<Client *> related_channels_clients = getClients();
    for (size_t j = 0; j < related_channels_clients.size(); j++)
    {
        if (related_channels_clients[j] == client)
        {
            if (isOpe(related_channels_clients[j])) {
                _ope_clients.erase(std::find(_ope_clients.begin(), _ope_clients.end(), client));
            }
            else
            {
                _normal_clients.erase(std::find(_normal_clients.begin(), _normal_clients.end(), client));
            }
        }
    }
}
void Channel::broadcastMessage(std::string message)
{
    std::vector<Client *> clients = this->getClients();
    for (size_t i = 0; i < clients.size(); i++)
        clients[i]->message(message);
}
bool Channel::hasKey(void)
{
    return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_KEY) != _modes.end());
}
