//
// Created by marks on 10/29/18.
//
#include <vector>
#include <sstream>
#include <iostream>
#include "Protocols.h"
#include "User.h"

std::vector<std::string> Protocols::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<std::string> Protocols::parseMessage(std::string msg, User *&user, ChatServer &server, cs457::tcpUserSocket *sckt) {
    std::vector<std::string> sVect = split(msg,' ');
    std::vector<std::string> outV = {"","",""};
    if (sVect[0] == "PASS"){
        if (sVect.size() == 2) {
            user->setPassWord(sVect[1]);
        }
    }else if (sVect[0] == "NICK"){
        if (sVect.size() == 2 && !user->getPassWord().empty()) {
            user->setNick(sVect[1]);
        }
        else if (sVect.size() == 2 && user->getPassWord().empty()){
            user->setNick(sVect[1]);
        }
        else{
            outV[0] = "Error: Nick is is use, please try another one";
            outV[2] = user->getNick();
        }
    }else if(sVect[0] == "USER"){
        if (sVect.size() >= 2 && !user->getPassWord().empty() && !user->getNick().empty()){
            User* current = server.getUser(user->getNick());
            if (current != nullptr){
                if (user->getPassWord() == current->getPassWord()){
                    current->setSocket(sckt);
                    user = current;
                    user->setActive();
                    if(sVect.size() > 2) {
                        user->setRealName(sVect[2]);
                    }
                    outV[0] = "SERVER: Successfully Logged in, nick is "+user->getNick();
                    outV[2] = user->getNick();
                }else{
                    //error here
                }
            }
            else{
                outV[0] = "SERVER: That username is not registered.";
                outV[2] = user->getNick();
                user->setPassWord("");
                user->setNick("");
            }
        }
    }else if(sVect[0] == "PRIVMSG"){
        if (sVect.size() >= 3){
            if (server.hasChannel(sVect[1])){
                for (int i = 2; i < sVect.size(); i++){
                    if (i>2){
                        outV[0]+=" ";
                    }
                    outV[0]+= sVect[i];
                }
                outV[1] = sVect[1];
                outV[0] = user->getNick() + ": "+ outV[0]+ "\r\n";
            }
            else if(server.getUser(sVect[1]) != nullptr){
                outV[2]= sVect[1];
                outV[0] = user->getNick()+"; "+ sVect[0];
            }
        }
    }else if (sVect[0] == "JOIN"){
        if (sVect.size() == 3 && user->getActive()){
            if (sVect[2] == "@"){
                sVect[2] = "";
            }
            std::string response = server.joinChannel(user->getNick(), sVect[1], sVect[2]);
            outV[0] = response;
            outV[2] = user->getNick();
        }
        else{
            //error here
        }
    }

    return outV;
}

std::string Protocols::getAction(std::string msg) {
    return "";
}
