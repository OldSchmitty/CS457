//
// Created by marks on 10/26/18.
//

#include "User.h"
#include "tcpUserSocket.h"

User::User(){    this->mtx = new std::mutex();}
User::User(std::string userName, std::string passWord, bool banStatus, cs457::tcpUserSocket *sckt){
    this->userName = userName;
    this->passWord = passWord;
    this->scktPointer = sckt;
    this->banStatus = banStatus;
    this->mtx = new std::mutex();
}
User::~User(){
    delete(this->mtx);
}

std::string User::getUserName() {
    return userName;
}

void User::setUserName(std::string userName) {
    this->userName = userName;
}

std::string User::getPassWord(){
    return passWord;
}

void User::setPassWord(std::string passWord) {
    this->passWord = passWord;
}

std::string User::getNick() {
    return this->nick;
}

void User::setNick(std::string nick) {
    this->nick = nick;
}

void User::sendMessage(std::string msg) {
    this->scktPointer->sendString(msg);
}

void User::setSocket(cs457::tcpUserSocket *sckt) {
    this->scktPointer = sckt;
}

void User::setActive() {
    mtx->lock();
    this->active = true;
    mtx->unlock();
}
void User::setInactive() {this->active = false;}
bool User::getActive() {return this->active;}
bool User::getBanStatus(){return this->banStatus;}
void User::ban() {banStatus = true;}
void User::unBan() {banStatus = false;}
int User::getLevel() {return this->level;}
void User::setLevel(int level) {this->level = level;}
void User::sendMsg(std::string msg) {this->scktPointer->sendString(msg);}
void User::setRealName(std::string name) {this->realName = name;}
std::string User::getRealName() {return this->realName;}
void User::disconnect(){
    mtx->lock();
    this->scktPointer->closeSocket();
    active = false;
    mtx->unlock();
}