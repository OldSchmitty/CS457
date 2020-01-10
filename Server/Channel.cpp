//
// Created by marks on 10/26/18.
//

#include "Channel.h"
#include <iostream>

Channel::Channel() {
    channelProps["inviteOnly"] = false;
    channelProps["anyoneInvite"] = true;
    channelProps["outSideMessages"] = true;
}

Channel::Channel(std::string name, std::string passWord, std::string description, bool userChannel) {
    this->channelName = name;
    this->passWord = passWord;
    this->description = description;
    channelProps["inviteOnly"] = false;
    channelProps["anyoneInvite"] = true;
    channelProps["outSideMessages"] = true;
    this->userChannel = userChannel;
}

void Channel::setProp(std::string prop, bool var){
    channelProps[prop] = var;
}

bool Channel::addUser(std::string nick, User &user){
    if (channelProps["inviteOnly"]){
        if (isInvited(nick)) {
            userMap[nick] = user;
            updateUsers();
            return true;
        }
    }
    else if (userMap.count(nick) == 0) {
        userMap[nick] = user;
        updateUsers();
        return true;

    }
    else{
        updateUsers();
        return true;
    }
   return false;
}

void Channel::removeUser(std::string nick) {
    if (userMap.count(nick) == 1){
        userMap.erase(nick);
	updateUsers();
    }
}

bool Channel::checkUser(std::string nick) {
    return userMap.count(nick);
}

void Channel::remove(){}


std::string Channel::getName() {return this->channelName;}

void Channel::sendMsg(std::string msg) {
    if (msg.find("USERSINCHANNEL:") == 0){
	
    }
    else if(userChannel == true){
        msg = std::string("#")+channelName+":"+msg;
    }
    else{
        msg = std::string("##")+channelName+":"+msg;
    }
    for (auto it = userMap.begin(); it != userMap.end(); it++){
        it->second.sendMsg(msg);
    }
}

void Channel::setOp(std::string userName) {
    this->chanOps[userName] = true;
}

void Channel::setPassWord(std::string passWord) {this->passWord = passWord;}
std::string Channel::getPassWord() {return this->passWord;}

void Channel::setDescription(std::string description) {
    this->description = description;
}

std::string Channel::getDescription() {return this->description;}

std::string Channel::getChanOps() {
    std::string rtn;
    int i = 0;
    for (auto it = chanOps.begin(); it != chanOps.end(); it++){
        if (it != chanOps.begin() ){
            rtn +="\t";
        }
        rtn+=it->first;
    }
    return rtn;
}

bool Channel::inviteUser(std::string userName, std::string inviteName) {
    if (!channelProps["inviteOnly"]){
        return true;
    }
    else if (chanOps.count(userName) || invites.count(userName)){
        invites[inviteName] = true;
        return true;
    }
    return false;
}

bool Channel::isInvited(std::string userName) {
    if (invites.count(userName) || chanOps.count(userName))
        return true;
    return false;
}

bool Channel::isOP(std::string username) {return chanOps.count(username) == 1;}
bool Channel::getUserChannel() {return userChannel;}

std::string Channel::getUsers(){
    std::string rtn="";
    for (auto it = userMap.begin(); it != userMap.end(); it++){
	if (it != userMap.begin()){
	    rtn+=",";
        }        
	rtn+=it->first;
    }
    return rtn;
}

void Channel::updateUsers(){
    std::string msg = "USERSINCHANNEL:"+channelName+":";
    msg+=getUsers();
    sendMsg(msg);
}
