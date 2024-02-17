#pragma once

#include "Client.hpp"

class Client;

enum ChannelMode {
   CHANNEL_MODE_INVITE_ONLY = 0, //+i
   CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY = 1, //+t
   CHANNEL_MODE_KEY = 2, //+k
   CHANNEL_MODE_OPERATOR = 3, //+o  //user mode
   CHANNEL_MODE_USER_LIMIT = 4, //+l
};

class Channel {
    private:
        std::string _name;
        std::string _password;
        int _channel_limit;
        std::string _topic;
        std::vector<ChannelMode> _modes;
        std::vector<Client> inviteList;
        std::vector<Client> clientList;
        std::vector<Client> opeList;

    public:
        Channel(std::string &name, std::string &password); 
        // Setters
        void setTopic(std::string &newTopic);
        void setPassword(std::string &passwd);
        void setChannel_limit(int limit);
        // Getters
        std::string &getPassword(void) const;
        std::string &getName(void) const;
        std::string &getTopic(void) const;
        std::vector<ChannelMode> getModes(void) const;
        int getChannelLimit(void) const;
        std::vector<Client> getClientList(void) const;
        std::vector<Client> getOpeList(void) const;
        std::vector<Client> getInviteList(void) const;
        // Client stuff
		void join(Client client);
        bool isJoined(Client client);
        bool isOpe(Client client);
        bool isInvited(Client client);
        bool isInviteOnly(void);
        void addOpe(Client client);
        void removeOpe(Client client);
        void kick(Client client);
        void invite(Client client);
        void removeInvite(Client client);
        // Utils
        bool hasMode(ChannelMode mode);
        void addMode(ChannelMode mode);
        void removeMode(ChannelMode mode);
        void broadcastMessage(std::string message);
        bool hasKey(void);
        void removeKey(void);
};