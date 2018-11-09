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
            user->sendMsg("Error: Nick is is use, please try another one");
        }
    }else if(sVect[0] == "USER"){
        if (sVect.size() >= 2 && !user->getPassWord().empty() && !user->getNick().empty()){
            User* current = server.getUser(user->getNick());
            if (current != nullptr){
                if (current->getBanStatus()){
                    user->sendMsg("SERVER: your account is currently banned.");
                }
                else if (user->getPassWord() == current->getPassWord()){
                    current->setSocket(sckt);
                    user = current;
                    user->setActive();
                    if(sVect.size() > 2) {
                        user->setRealName(sVect[2]);
                    }
                    outV[0] = "SERVER: Successfully Logged in, nick is "+user->getNick();
                    outV[2] = user->getNick();
                }else{
                    outV[0] = "SERVER: Incorect Username or Password";
                    outV[2] = user->getNick();
                }
            }
            else{
                user->sendMsg("SERVER: That username is not registered.");
                user->setPassWord("");
                user->setNick("");
            }
        }
    }else if(sVect[0] == "PRIVMSG"){
        if (sVect.size() >= 3){
            for (int i = 2; i < sVect.size(); i++){
                if (i>2){
                    outV[0]+=" ";
                }
                outV[0]+= sVect[i];
            }
            if (server.hasChannel(sVect[1])){
                outV[1] = sVect[1];
                outV[0] = user->getNick() + ": "+ outV[0];
            }
            else if(server.getUser(sVect[1]) != nullptr){
                if (server.getUser(sVect[1])->getAway()){
                    outV[2] = user->getNick();
                    outV[0] = "User "+ server.getUser(sVect[1])->getNick() + " is away. "+server.getUser(sVect[1])->getAwayMessage();
                }
                else{
                    outV[2]= sVect[1];
                    outV[0] = user->getNick()+": "+ sVect[0];
                }
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
            user->sendMsg("SERVER: Please sign in or choose a nick to join a channel.");
        }
    }else if (sVect[0] == "AWAY") {
        if (sVect.size() > 1) {
            user->setAway(true, sVect[1]);
        }
        else {
            user->setAway(true);
        }
    }else if (sVect[0] == "DIE"){
        if (user->getLevel() == 3)
            outV[0] = "DIE";
    }else if (sVect[0] == "INFO"){
        user->sendMsg("SERVER: "+server.getInfoString());
    }else if (sVect[0] == "HELP"){
        user->sendMsg("SERVER: GETTING HELP INFORMATION --\n"+server.getHelpString());
    }else if (sVect[0] == "INVITE" && sVect.size() == 3 && user->getActive()){
        if (server.invite(user->getNick(), sVect[1], sVect[2])){
            outV[0] = "SERVER: "+user->getNick()+" has invited you to join the channel "+sVect[2];
            outV[2] = sVect[1];
        }
        else{
            outV[0] = "SERVER: Not able to invite to channel "+sVect[2];
            outV[2] = user->getNick();
        }
    }else if (sVect[0] == "ISON"){
        std::string reply= "SERVER: Active Users - ";
        for (int i = 1; i < sVect.size(); i++){
            if (server.getUser(sVect[i]) != nullptr && server.getUser(sVect[i])->getActive()){
                reply += " "+sVect[i];
            }
        }
        user->sendMsg(reply);
    }else if(sVect[0] == "KICK" && sVect.size() >= 3){
        server.kick(user->getNick(), sVect[1], sVect[2]);
        outV[0] = "SERVER: You have been kicked by "+user->getNick();
        if (sVect.size() == 4){
            outV[0] += " Reason -- "+sVect[3];
        }
        outV[2] = sVect[2];
    }else if(sVect[0] == "KILL" and sVect.size() == 2){
        server.kill(user->getNick(), sVect[1]);
    }else if (sVect[0] == "KNOCK" && sVect.size() == 2 && user->getActive()){
        outV[0] = "SERVER: "+user->getNick()+" is requesting an invite for the channel.";
        outV[1] = sVect[1];
    }else if(sVect[0] == "LIST"){
        user->sendMsg("Current channles are:"+server.getChannels());
    }else if(sVect[0] == "NOTICE"){
        if (sVect.size() >= 3){
            for (int i = 2; i < sVect.size(); i++){
                if (i>2){
                    outV[0]+=" ";
                }
                outV[0]+= sVect[i];
            }
            if(server.getUser(sVect[1]) != nullptr && user->getLevel() > 1){
                outV[2]= sVect[1];
                outV[0] = "NOTICE: "+user->getNick()+": "+ outV[0];
            }
        }
    }else if (sVect[0] == "PART" && sVect.size() >= 2){
        for (int i = 1; i< sVect.size(); i++){
            user->sendMsg("SERVER: "+sVect[i]+ " is going to miss you! (you left the channel)");
            server.part(user->getNick(), sVect[i]);
        }
    }else if (sVect[0] == "PING"){
        user->sendMsg("PONG");
    }else if(sVect[0] == "RULES"){
        user->sendMsg("SERVER: Current Rules -- "+server.getRules());
    }else if (sVect[0] == "SETNAME" && sVect.size() == 2){
        user->setRealName(sVect[1]);
        user->sendMsg("SERVER: real name has been set to "+user->getRealName());
    }else if (sVect[0] == "TIME"){
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        user->sendMsg("SERVER: THe current server time is :" + std::string(buf));
    }else if(sVect[0] == "WALLOPS"){
        for (int i = 1; i < sVect.size(); i++){
            if (i>1){
                outV[0]+=" ";
            }
            outV[0]+= sVect[i];
        }
        server.wallOPs(user->getNick(), outV[0]);
    }



    return outV;
}

std::string Protocols::getAction(std::string msg) {
    return "";
}
