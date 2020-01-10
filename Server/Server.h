//
// Created by marks on 10/26/18.
//

#ifndef SERVER_CHATSERVER_H
#define SERVER_CHATSERVER_H
#include <string>
#include "Channel.h"
#include <map>
#include "User.h"
#include "tcpUserSocket.h"

class Server {
    private:
        std::map<std::string, Channel> channelMap;
        std::map<std::string, User> userInfo;
        std::string configFile;
        std::string userFile;
        std::string banner;
        std::vector<std::string> bannedUsers;
        std::string filePath;
        void outputFiles();
        std::string helpString;
        std::string infoString;
        std::string rules;

    public:
        Server();
        Server(std::string filePath);
        int createChannel(std::string userName, std::string const channelName, const std::string passWord, const std::string description);
        int deleteChannel(std::string const channelName);
        bool addUser(std::string userName, std::string passWord, cs457::tcpUserSocket *socket);
        void addUser(User &user);
        std::string joinChannel(std::string userName, std::string channel, std::string passWord);
        void disconnect(std::string userName);
        void shutDown();
        void banUser(std::string userName, std::string banName);
        void unBanUser(std::string userName, std::string banName);
        void sendMsg(std::string msg, std::string channel, std::string userName);
        void loadChannelFile(std::string fileName);
        void loadUserFile(std::string fileName);
        User* getUser(std::string userName);
        bool hasChannel(std::string channelName);
        void showUsers();
        std::string getBanner();
        void setBanner(std::string banner);
        void loadBannedUsersFile(std::string fileName);
        void loadBannerFile(std::string bannerFile);
        void loadRules(std::string fileName);
        std::string getRules();
        void setHelpString(std::string help);
        void setInfoString(std::string info);
        std::string getInfoString();
        std::string getHelpString();
        void loadHelpFile(std::string fileName);
        bool invite(std::string userName, std::string inviteName, std::string channelName);
        void kick(std::string userName, std::string channel, std::string kickName);
        void kill(std::string userName, std::string killName);
        std::string getChannels();
        void part(std::string userName, std::string channel);
        void wallOPs(std::string userName, std::string msg);
	std::string getUsers(std::string channelName);

};


#endif //SERVER_CHATSERVER_H
