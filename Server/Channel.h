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
        std::map<std::string, bool> channelProps;
        std::map<std::string, bool> chanOps;

    public:
        Channel();
        Channel(std::string name, std::string passWord, std::string description);
        bool addUser(std::string userName, User &user);
        void removeUser(std::string userName);
        bool checkUser(std::string userName);
        void setPassWord(std::string passWord);
        std::string getPassWord();
        void setProp(std::string prop, bool var);
        void remove();
        std::string getName();
        void sendMsg(std::string msg);
        void setOp(std::string userName);

    /*user map
     *
     * closeChannel()
     * channelCreation
     *
     *
     */
};


#endif //SERVER_CHANNEL_H
