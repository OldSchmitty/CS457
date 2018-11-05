//
// Created by marks on 10/26/18.
//

#ifndef SERVER_PROTOCOLS_H
#define SERVER_PROTOCOLS_H
#include <string>
#include<vector>
#include "User.h"
#include "ChatServer.h"
#include "tcpUserSocket.h"

class Protocols {
    public:
        static std::vector<std::string> parseMessage(std::string msg, User *user, ChatServer &server, cs457::tcpUserSocket *sckt);
        static std::string getAction(std::string msg);
        static std::vector<std::string> split(const std::string& s, char delimiter);
};


#endif //SERVER_PROTOCOLS_H
