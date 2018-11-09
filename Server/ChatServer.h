//
// Created by marks on 10/26/18.
//

#ifndef SERVER_CHATSERVER_H
#define SERVER_CHATSERVER_H
#include <string>
#include "Channel.h"
#include "ClientManager.h"
#include <map>
#include "User.h"
#include "tcpUserSocket.h"

class ChatServer {
    private:
        std::map<std::string, Channel> channelMap;
        std::map<std::string, User> userInfo;
        std::string configFile;
        std::string userFile;
        std::string banner;
        std::vector<std::string> bannedUsers;
        std::string filePath;
        void outputFiles();

    public:
        ChatServer();
        ChatServer(std::string filePath);
        int CreateChannel(std::string const channelName, const std::string passWord, const std::string description);
        int deleteChannel(std::string const channelName);
        void removeUser(std::string userName, std::string ChannelName);
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

};


#endif //SERVER_CHATSERVER_H
