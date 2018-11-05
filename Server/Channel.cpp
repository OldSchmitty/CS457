//
// Created by marks on 10/26/18.
//

#include "Channel.h"

Channel::Channel() {
    channelProps["inviteOnly"] = false;
    channelProps["anyoneInvite"] = true;
    channelProps["outSideMessages"] = true;
}

Channel::Channel(std::string name, std::string passWord, std::string description) {
    this->channelName = channelName;
    this->passWord = passWord;
    this->description = description;
    channelProps["inviteOnly"] = false;
    channelProps["anyoneInvite"] = true;
    channelProps["outSideMessages"] = true;
}

void Channel::setProp(std::string prop, bool var){
    channelProps[prop] = var;
}

bool Channel::addUser(std::string userName, User &user){

    if (userMap.count(userName) == 0){
        userMap[userName] = user;
    }else{
        if (channelProps["inviteOnly"]){
            return false;
        }
    }
    return true;
}

void Channel::removeUser(std::string userName) {
    if (userMap.count(userName) == 1){
        userMap.erase(userName);
    }
}

bool Channel::checkUser(std::string userName) {
    return userMap.count(userName);
}

void Channel::remove(){}


std::string Channel::getName() {return this->channelName;}

void Channel::sendMsg(std::string msg) {
    for (auto it = userMap.begin(); it != userMap.end(); it++){
        it->second.sendMessage(msg);
    }
}

void Channel::setOp(std::string userName) {
    this->chanOps[userName] = true;
}

void Channel::setPassWord(std::string passWord) {this->passWord = passWord;}
std::string Channel::getPassWord() {return this->passWord;}