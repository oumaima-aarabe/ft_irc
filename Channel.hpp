#pragma once

#include "client.hpp"

class Client;

enum ChannelMode {
   CHANNEL_MODE_INVITE_ONLY = 0, //+i      //doesn't need a parameter
   CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY = 1, //+t     //doesn't need a parameter
   CHANNEL_MODE_KEY = 2, //+k              //'key PARAMETER' undisplayable in channel modes set
   CHANNEL_MODE_OPERATOR = 3, //+o         //user mode undisplayable in channel modes set
   CHANNEL_MODE_USER_LIMIT = 4, //+l
};

class Channel {
    private:
        std::string _name; // channel name
        std::string _password; //channel key
        int _channel_limit; // maximum number of clients that can join the channel
        std::string _topic; // channel topic
        std::string _stringModes; // string representation of the channel modes
        std::vector<std::pair<ChannelMode, int> > _modes; //all known channel modes and their active status
        std::vector<Client> inviteList; //all invited members
        std::vector<Client> allClientsList; //all members of the channel
        std::vector<Client> opeList; //all channel operators

    public:
        Channel(std::string const &name, std::string const &password);
        ~Channel();
        // Setters
        void setTopic(std::string const &newTopic);
        void setPassword(std::string const &passwd);
        void setChannel_limit(int limit);
        // Getters
        std::string getPassword(void) const;
        std::string getName(void) const;
        std::string getTopic(void) const;
        std::vector<std::pair<ChannelMode, int> > getModes(void) const;
        std::string getStringModes(void) const;
        int getChannelLimit(void) const;
        std::vector<Client> getAllClientsList(void) const;
        std::vector<Client> getOpeList(void) const;
        std::vector<Client> getInviteList(void) const;
        // Client stuff
		int addClient(Client client);
		void removeClient(Client client);
        bool isJoined(std::string nickname);
        std::string listClients();
        bool isOpe(std::string nickname);
        bool isInvited(std::string nickname);
        void addOpe(std::string nickname);
        void removeOpe(std::string nickname);
        void invite(Client client);
        void removeInvite(Client client);
        // Channel modes stuff
        char getModeIdentifier(ChannelMode _mode) const;
        void updateStringModes(void);
        bool isInviteOnly(void);
        bool hasMode(ChannelMode mode);
        void addMode(ChannelMode mode);
        void removeMode(ChannelMode mode);
        bool hasKey(void);
        // Utils
        void broadcastMessage(Client *sender, std::string message);
        static bool isValidChannelName(const std::string name);
};