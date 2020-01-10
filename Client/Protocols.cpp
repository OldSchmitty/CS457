//
// Created by marks on 10/28/18.
//

#include "Protocols.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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

std::string Protocols::makeMessage(std::string str, std::string &channel) {
    if (str.size() > 510)
        std::cout<<"MESSAGE SIZE LIMIT OF 510 CHARACTERS"<<std::endl;
    else if (str.find('/') == 0) {
        std::vector<std::string> sVect = split(str, ' ');
        std::string command = sVect[0].substr(1);
        for (int i = 0; i<command.size(); i++)
            command[i] = tolower(command[i]);
        str = commandFactory(command, sVect, channel);
    } else {
      str ="PRIVMSG "+channel+" "+str+"\r\n";
    }
    return str;
}

std::string Protocols::commandFactory(std::string command, std::vector<std::string> args, std::string &channel){
    std::string rtn;
    if (command == "join") {
        if (args.size() >= 2) {
            rtn = "JOIN " + args[1];
            if (args.size() == 3) {
                rtn += " " + args[3] + "\r\n";
            } else {
                rtn += " @\r\n";
            }
            channel = args[1];
        }
    }
    else if (command == "user" && args.size() >= 2){
        rtn = "USER "+args[1];
        if (args.size() > 2){
            rtn += " "+ args[2];
        }
        rtn+="\r\n";
    }
    else if (command == "pass" && args.size() == 2){
        rtn = "PASS "+args[1]+"\r\n";
    }
    else if (command == "nick" && args.size() == 2){
        rtn = "NICK "+args[1]+"\r\n";
    }
    else if(command == "away"){
        rtn = "AWAY";
        if (args.size() >1)
            rtn+=args[1];
        rtn += "\r\n";
    }
    else if(command == "die"){
        rtn = "DIE\r\n";
    }else if(command == "info"){
        rtn = "INFO\r\n";
    }else if(command == "help"){
        rtn = "HELP\r\n";
    }else if(command == "invite"){
        if (args.size() == 3){
            rtn = "INVITE "+args[1]+" "+args[2]+"\r\n";
        }
    }else if (command == "ison"){
        rtn = "ISON";
        for (int i = 1; i < args.size(); i++) {
            rtn += " " + args[i];
        }
        rtn +="\r\n";
    } else if (command == "kick" && args.size() >= 3){
        rtn = "KICK";
        for (int i = 1; i <args.size(); i++){
            rtn += " " +args[i];
        }
        rtn+="\r\n";
    } else if (command == "kill" && args.size() == 2){
        rtn = "KILL "+args[1]+"\r\n";
    } else if (command == "knock" && args.size() == 2){
        rtn = "KNOCK "+args[1] + "\r\n";
    } else if (command == "list"){
        rtn = "LIST\r\n";
    } else if (command == "notice" and args.size() >= 3){
        rtn = "NOTICE "+args[1];
        for(int i = 2; i < args.size(); i ++){
            rtn+=" "+args[i];
        }
        rtn+="\r\n";
    }else if (command == "part" && args.size() >= 2){
        rtn = "PART";
        for (int i = 1;  i< args.size(); i ++){
            rtn += " "+args[i];
        }
        rtn += "\r\n";
    }else if (command == "ping"){
        rtn = "PING\r\n";
    }else if (command == "quit"){
        rtn = "QUIT";
    }else if (command == "rules"){
        rtn = "RULES\r\n";
    }else if(command == "setname" && args.size() == 2){
        rtn = "SETNAME "+args[1]+"\r\n";
    }else if (command == "time"){
        rtn = "TIME\r\n";
    }else if (command == "wallops"){
        rtn = "WALLOPS";
        for(int i = 1; i < args.size(); i ++){
            rtn+=" "+args[i];
        }
        rtn+="\r\n";
    }
    return rtn;
}