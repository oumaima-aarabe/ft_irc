#pragma once

#include "client.hpp"
#include "Message.hpp"

class Client;

enum ChannelMode {
   CHANNEL_MODE_INVITE_ONLY = 0, //+i
   CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY = 1, //+t
   CHANNEL_MODE_KEY = 2, //+k
   CHANNEL_MODE_OPERATOR = 3, //+o
   CHANNEL_MODE_USER_LIMIT = 4, //+l
};

enum ClientMode {
    CLIENT_MODE_REGULAR = 0,
    CLIENT_MODE_OPERATOR = 1
};

class Channel {
    private:
        std::string _name;
        std::string _password;
        Client * _creator;
        time_t _created_at;
        std::vector<Message *> _messages;
        long long int _user_limit;
        std::string _topic;
        ChannelMode _mode;
        std::vector<ChannelMode> _modes;
        std::vector<Client *> _invite_list;

    public:
        std::vector<Client *> _normal_clients;
        std::vector<Client *> _ope_clients;
        std::vector<std::pair<Client *, ClientMode> > _clients;

    public:
        Channel(std::string &name, std::string &password); 
        // Setters
        void setCreator(Client *creator);
        void setTopic(std::string &topic);
        void setPassword(std::string passwd);
        // Getters
        std::string &getPassword(void);
        std::string &getName(void);
        std::string &getTopic(void);
        Client *getCreator(void);
        time_t getCreatedAt(void);
        ChannelMode getMode(void);
        std::vector<ChannelMode> getModes(void);
        char getIdentifier(ChannelMode _mode);
        std::string getStringModes(void);
        size_t getUserLimit(void);
        void setUserLimit(size_t limit);
        std::vector<Client *> getClients(void) const;
        // Client stuff
        bool joined(Client *client);
        bool isOwner(Client *client);
        bool isOpe(Client *client);
        bool isNormal(Client *client);
        bool isInvited(Client *client);
        bool isInviteOnly(void);
        void addOpe(Client *client);
        void removeOpe(Client *client);
        void kick(Client *client);
        void invite(Client *client);
        void removeInvite(Client *client);
        // Utils
        std::string getModeString(void);
        bool hasMode(ChannelMode mode);
        void addMode(ChannelMode mode);
        void removeMode(ChannelMode mode);
        void removeClientFromChannel(Client *client);
        void broadcastMessage(std::string message);
        bool hasKey(void);
};