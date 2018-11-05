//
// Created by marks on 10/26/18.
//

#include <sys/user.h>
#include "ChatServer.h"
#include <fstream>
#include "Protocols.h"

ChatServer::ChatServer(){

}

ChatServer::ChatServer(std::string configFile, std::string userFile){
    //if (configFile != "")

    //if (userFile != "")

}


int ChatServer::CreateChannel(std::string const channelName, const std::string passWord, const std::string description){
    Channel channel(channelName, passWord, description);
    channelMap[channelName] = channel;
}
int ChatServer::deleteChannel(std::string const channelName){
    channelMap[channelName].remove();
    channelMap.erase(channelName);
}

void ChatServer::removeUser(std::string userName, std::string ChannelName){

}
bool ChatServer::addUser(std::string userName, std::string passWord, cs457::tcpUserSocket *sckt) {
    if (userInfo.count(userName) == 0) {
        User user(userName, passWord, false, sckt);
        user.setActive();
        this->userInfo[user.getUserName()] = user;
    }else{
        return false;
    }
    return true;
}


void ChatServer::disconnect(std::string userName){
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
        if (it->second.checkUser(userName)){
            it->second.removeUser(userName);
        }

    }
}


std::string ChatServer::joinChannel(std::string userName, std::string channel, std::string passWord){
    bool success = false;
    if (channelMap.count(channel) == 1) {
        if(channelMap[channel].getPassWord() == passWord)
            success = channelMap[channel].addUser(userName, userInfo[userName]);
        else
            return "You entered the wrong password for channel "+channel+"\r\n";
    }
    else{
        return "Channel does not exists\r\n";
    }
    return "You have joined channel "+channel+"\r\n";
}

void ChatServer::shutDown() {
    /*if (configFile == ""){

    }
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
        it->second.
    }*/
}
void ChatServer::banUser(std::string userName, std::string banName) {
    if (userInfo[userName].getLevel() >= SYSOP){
        userInfo[banName].ban();
        for (auto it = channelMap.begin(); it != channelMap.end(); it++){
            if (it->second.checkUser(banName)){
                it->second.removeUser(banName);
            }
        }

    }
}

void ChatServer::sendMsg(std::string msg, std::string channel, std::string userName) {
    if (channel != "" && userName == ""){
        if (this->channelMap.count(channel) == 1)
            channelMap[channel].sendMsg(msg);
    }
    else if (userName != "" and channel == ""){
        if (userInfo.count(userName) == 1)
            userInfo[userName].sendMsg(msg);
    }
}

void ChatServer::loadChannelFile(std::string fileName) {
    std::ifstream is;
    is.open(fileName);
    std::string input;
    while(getline(is, input))
    {
        std::vector<std::string> sVect = Protocols::split(input, '\t');
        if (sVect.size() >= 3){
            std::string channelName = sVect[0];
            std::string description = sVect[1];
            std::string  passWord = sVect[2];
            if (passWord == "@")
                passWord = "";
            Channel channel(channelName, passWord, description);
            for (int i = 3; i<sVect.size(); i++)
            {
                channel.setOp(sVect[i]);
            }
            channelMap[channelName] = channel;
        }
    }
    is.close();
}

void ChatServer::loadUserFile(std::string fileName) {
    std::ifstream is;
    is.open(fileName);
    std::string input;
    while(getline(is, input))
    {
        std::vector<std::string> sVect = Protocols::split(input, '\t');
        if (sVect.size() == 4){
            std::string userName = sVect[0];
            std::string password = sVect[1];
            if (password == "@")
                password = "";
            int level = stoi(sVect[2]);
            std::string banned = sVect[3];
            User user;
            user.setUserName(userName);
            user.setPassWord(password);
            user.setLevel(level);
            if (banned == "true"){
                user.ban();
            }
            userInfo[userName] = user;
        }
    }
    is.close();
}

User* ChatServer::getUser(std::string userName) {
    User* rtn = nullptr;
    if (userInfo.count(userName) == 1){
        rtn =  &userInfo[userName];
    }
    return rtn;
}

bool ChatServer::hasChannel(std::string channelName) {
    return (channelMap.count(channelName) == 1);
}

void ChatServer::addUser(User &user){
    userInfo[user.getUserName()] = user;
}

