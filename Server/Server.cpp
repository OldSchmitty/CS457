//
// Created by marks on 10/26/18.
//

#include <sys/user.h>
#include "Server.h"
#include <fstream>
#include <iostream>
#include "Protocols.h"

Server::Server(){

}

Server::Server(std::string filePath){
    this->filePath = filePath;
    loadChannelFile(filePath+"channels");
    loadUserFile(filePath+"users");
    loadBannedUsersFile(filePath+"banusers.txt");
    loadBannerFile(filePath+"banner.txt");
    loadHelpFile(filePath+"help.txt");
    loadRules(filePath+"rules.txt");
}

int Server::createChannel(std::string userName, std::string const channelName, const std::string passWord, const std::string description){
    Channel channel(channelName, passWord, description, true);
    channel.setOp(userName);
    channelMap[channelName] = channel;
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()) {
            it->second.sendMsg("Current channles are:"+getChannels());
        }
    }
    
}
int Server::deleteChannel(std::string const channelName){
    channelMap[channelName].remove();
    channelMap.erase(channelName);
}

void Server::kill(std::string userName, std::string killName){
    if (userInfo.count(userName) && userInfo[userName].getLevel() >=2 ){
        disconnect(killName);
        userInfo[killName].disconnect();
    }
}

bool Server::addUser(std::string userName, std::string passWord, cs457::tcpUserSocket *sckt) {
    if (userInfo.count(userName) == 0) {
        User user(userName, passWord, false, sckt);
        user.setActive();
        this->userInfo[user.getUserName()] = user;
    }else{
        return false;
    }
    return true;
}


void Server::disconnect(std::string userName){
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
        if (it->second.checkUser(userName)){
            it->second.removeUser(userName);
        }
	if(it->second.getUserChannel() && it->second.isOP(userName)){
            deleteChannel(it->first);
        }
    }
}



std::string Server::joinChannel(std::string userName, std::string channel, std::string passWord){
    bool success = false;
    if (channelMap.count(channel) == 1) {
        if(channelMap[channel].getPassWord() == passWord)
            success = channelMap[channel].addUser(userName, userInfo[userName]);
        else
            return "You entered the wrong password for channel "+channel+"\r\n";
    }
    else{
        return "Channel does not exist\r\n";
    }
    return "SERVER: Welcome to channel "+channel+ "\nDescription: "+channelMap[channel].getDescription()+"\r\n";
}

void Server::shutDown() {
    outputFiles();
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()) {
            it->second.sendMsg("SERVER: THE SERVER IS SHUTTING DOWN.");
        }
    }
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()) {
            it->second.disconnect();
        }
    }
}
void Server::banUser(std::string userName, std::string banName) {
    if (userInfo[userName].getLevel() >= SYSOP){
        userInfo[banName].ban();
        for (auto it = channelMap.begin(); it != channelMap.end(); it++){
            if (it->second.checkUser(banName)){
                it->second.removeUser(banName);
            }
        }
        bannedUsers.push_back(userName);

    }
}

void Server::unBanUser(std::string userName, std::string banName) {
    if (userInfo[userName].getLevel() >= SYSOP){
        userInfo[banName].unBan();
        int loc = -1;
        for (int i = 0; i < bannedUsers.size(); i++){
            if (bannedUsers[i] == banName){
                loc = i;
                break;
            }
        }
        if (loc != -1)
            bannedUsers.erase(bannedUsers.begin()+loc);
    }
}

void Server::sendMsg(std::string msg, std::string channel, std::string userName) {
    if (channel != "" && userName == ""){
        if (this->channelMap.count(channel) == 1)
            channelMap[channel].sendMsg(msg);
    }
    else if (userName != "" and channel == ""){
        if (userInfo.count(userName) == 1)
            userInfo[userName].sendMsg(msg);
    }
}

void Server::loadChannelFile(std::string fileName) {
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
            Channel channel(channelName, passWord, description, false);
            for (int i = 3; i<sVect.size(); i++)
            {
                channel.setOp(sVect[i]);
            }
            channelMap[channelName] = channel;
        }
    }
    is.close();
}

void Server::loadUserFile(std::string fileName) {
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

User* Server::getUser(std::string userName) {
    User* rtn = nullptr;
    if (userInfo.count(userName) == 1){
        rtn =  &userInfo[userName];
    }
    return rtn;
}

bool Server::hasChannel(std::string channelName) {
    return (channelMap.count(channelName) == 1);
}

void Server::addUser(User &user){
    userInfo[user.getNick()] = user;
}

void Server::showUsers() {
    for (auto it= userInfo.begin(); it != userInfo.end(); it++){
        if (it->second.getActive()){
            std::cout<<it->second.getUserName()<<std::endl;
        }
    }
}

std::string Server::getBanner() {
    return this->banner;
}

void Server::setBanner(std::string banner){this->banner = banner;}

void Server::loadBannedUsersFile(std::string fileName){
    std::ifstream is;
    is.open(fileName);
    std::string input;
    while(getline(is, input)) {
        this->bannedUsers.push_back(input);
    }
}
void Server::loadBannerFile(std::string bannerFile){
    std::ifstream is;
    is.open(bannerFile);
    std::string banner;
    std::string input;
    while(getline(is, input)) {
        banner += input;
    }
    this->setBanner(banner);
}

void Server::outputFiles(){
    std::ofstream of;
    of.open(filePath+"channels");
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
        if(it->second.getUserChannel()){
            of << it->first + "\t" + it->second.getDescription() + "\t";
            if (it->second.getPassWord() == "")
                of << "@\t";
            else
                of << it->second.getPassWord() + "\t";
            of << it->second.getChanOps() +"\n";
        }
    }
    of.close();
    of.open(filePath+"users");
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
        of << it->second.getUserName()+"\t"<<it->first+"\t";
        if (it->second.getPassWord() != "")
            of<<it->second.getPassWord()+"\t";
        else
            of<<"@\t";
        of << it->second.getLevel()<<"\t";

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

void Server::loadHelpFile(std::string fileName) {
    std::ifstream is;
    is.open(fileName);
    std::string help;
    std::string current;
    while(getline(is, current)){
        help += current +"\n";
    }
    helpString = help;
    is.close();
}

void Server::loadRules(std::string fileName) {
    std::ifstream is;
    is.open(fileName);
    std::string rules;
    std::string current;
    while(getline(is, current)){
        rules += current +"\n";
    }
    this->rules = rules;
    is.close();
}
std::string Server::getRules(){return rules;}
void Server::setHelpString(std::string help){this->helpString = help;}
void Server::setInfoString(std::string info){this->infoString= info;}
std::string Server::getInfoString(){return infoString;}
std::string Server::getHelpString(){return helpString;}
bool Server::invite(std::string userName, std::string inviteName, std::string channelName) {
    bool success =false;
    if (channelMap.count(channelName)){
        success= channelMap[channelName].inviteUser(userName, inviteName);
    }
    return success;
}

void Server::kick(std::string userName, std::string channel, std::string kickName){
    if (channelMap.count(channel) && userInfo.count(userName) && (userInfo[userName].getLevel() >=2 || channelMap[channel].isOP(userName))){
        channelMap[channel].removeUser(kickName);
    }
}

std::string Server::getChannels() {
    std::string rtn;
    for (auto it = channelMap.begin(); it != channelMap.end(); it++){
	if (it != channelMap.begin()){
            rtn += ",";
        }
        if (it->second.getUserChannel()) 
            rtn +="##"+it->first;
        else
            rtn +="#"+it->first;
    }
    return rtn;
}

void Server::part(std::string userName, std::string channel){
    for (auto it = channelMap.begin(); it != channelMap.end(); it ++){
        it->second.removeUser(userName);
    }
}

void Server::wallOPs(std::string userName, std::string msg) {
    std::cout<<"GOTHERE"<<std::endl;
    if(userInfo.count(userName) && userInfo[userName].getLevel() > 1)
    for (auto it = userInfo.begin(); it != userInfo.end(); it++){
	std::cout<<"lvl1"<<std::endl;
        if (it->second.getLevel() > 1 && it->second.getActive()){
            it->second.sendMsg("WALLOPS: "+userName+": "+msg);
	    std::cout<<"lvl2"<<std::endl;
        }
    }
}

std::string Server::getUsers(std::string channelName){
    return channelMap[channelName].getUsers();
}

