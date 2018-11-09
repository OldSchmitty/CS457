//
// Created by marks on 10/26/18.
//

#include <sys/user.h>
#include "ChatServer.h"
#include <fstream>
#include <iostream>
#include "Protocols.h"

ChatServer::ChatServer(){

}

ChatServer::ChatServer(std::string filePath){
    this->filePath = filePath;
    loadChannelFile(filePath+"channels");
    loadUserFile(filePath+"users");
    loadBannedUsersFile(filePath+"banusers.txt");
    loadBannerFile(filePath+"banner.txt");
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
    return "SERVER: Welcome to channel "+channel+ "\nDescription: "+channelMap[channel].getDescription()+"\r\n";
}

void ChatServer::shutDown() {
    outputFiles();
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()) {
            it->second.sendMessage("SERVER: THE SERVER IS SHUTTING DOWN.");
        }
    }
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()) {
            it->second.disconnect();
        }
    }
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
        if (sVect.size() == 6){
            std::string userName = sVect[0];
            std::string nick = sVect[1];
            std::string password = sVect[2];
            if (password == "@")
                password = "";
            int level = stoi(sVect[3]);
            std::string banned = sVect[4];
            std::string realName = sVect[5];
            if (realName == "@")
                realName = "";
            User user;
            user.setNick(nick);
            user.setUserName(userName);
            user.setPassWord(password);
            user.setLevel(level);
            user.setRealName(realName);
            if (banned == "true"){
                user.ban();
            }
            userInfo[nick] = user;
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
    userInfo[user.getNick()] = user;
}

void ChatServer::showUsers() {
    for (auto it= userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()){
            std::cout<<it->second.getUserName()<<std::endl;
        }
    }
}

std::string ChatServer::getBanner() {
    return this->banner;
}

void ChatServer::setBanner(std::string banner){this->banner = banner;}

void ChatServer::loadBannedUsersFile(std::string fileName){
    std::ifstream is;
    is.open(fileName);
    std::string input;
    while(getline(is, input)) {
        this->bannedUsers.push_back(input);
    }
}
void ChatServer::loadBannerFile(std::string bannerFile){
    std::ifstream is;
    is.open(bannerFile);
    std::string banner;
    std::string input;
    while(getline(is, input)) {
        banner += input;
    }
    this->setBanner(banner);
}

void ChatServer::outputFiles(){
    std::ofstream of;
    of.open(filePath+"channels");
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
        of << it->first + "\t" + it->second.getDescription() + "\t" << it->second.getPassWord() + "\t"<<it->second.getChanOps() +"\n";
    }
    of.close();
    of.open(filePath+"users");
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        of << it->second.getUserName()+"\t"<<it->first+"\t";
        if (it->second.getPassWord() != "")
            of<<it->second.getPassWord()+"\t";
        else
            of<<"@\t";
        if (it->second.getBanStatus())
            of << "true\t";
        else
            of << "false\t";
        if (it->second.getRealName() == "")
            of << "@\n";
        else
            of << it->second.getRealName()<<"\n";
    }
    of.close();
    of.open(filePath+"banusers.txt");
    for (int i = 0; i < bannedUsers.size(); i++){
        of << bannedUsers[i]<<"\n";
    }
    of.close();
    of.open(filePath+"banner.txt");
    of << banner+"\n";
    of.close();
}

