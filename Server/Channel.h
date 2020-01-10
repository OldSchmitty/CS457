//
// Created by marks on 10/26/18.
//

#ifndef SERVER_CHANNEL_H
#define SERVER_CHANNEL_H
#include <string>
#include <map>
#include <fstream>
#include "User.h"
#include <map>

class Channel {
    private:
        std::map<std::string, User> userMap;
        std::string channelName;
        std::string passWord;
        std::string description;
	bool userChannel;
        std::map<std::string, bool> channelProps;
        std::map<std::string, bool> chanOps;
        std::map<std::string, bool> invites;

    public:
        Channel();
        Channel(std::string name, std::string passWord, std::string description, bool userChannel);
        bool addUser(std::string nick, User &user);
        void removeUser(std::string nick);
        bool checkUser(std::string nick);
        void setPassWord(std::string passWord);
        std::string getPassWord();
        void setProp(std::string prop, bool var);
        void remove();
        std::string getName();
        void sendMsg(std::string msg);
        void setOp(std::string userName);
        std::string getDescription();
        void setDescription(std::string description);
        std::string getChanOps();
        bool inviteUser(std::string userName, std::string inviteName);
        bool isInvited(std::string userName);
        bool isOP(std::string username);
	bool getUserChannel();
	std::string getUsers();
	void updateUsers();
};


#endif //SERVER_CHANNEL_H
