//
// Created by marks on 10/26/18.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H
#include <string>
#include "tcpUserSocket.h"
#include <vector>

const int USER = 0;
const int CHANOP = 1;
const int SYSOP = 2;
const int ADMIN = 3;

class User {
    private:
        std::string userName = "";
        std::string passWord = "";
        std::string nick = "";
        std::string hostName = "";
        std::string serverName = "";
        cs457::tcpUserSocket *scktPointer;
        int level;
        bool active = false;
        bool banStatus;
        std::vector<std::string> owner;


    public:
        User();
        User(std::string userName, std::string passWord, bool banStatus, cs457::tcpUserSocket *sckt);
        std::string getUserName();
        void setUserName(std::string userName);
        std::string getNick();
        void setNick(std::string nick);
        std::string getPassWord();
        void setPassWord(std::string passWord);
        void setActive();
        void setInactive();
        bool getActive();
        int getLevel();
        void setLevel(int level);
        void sendMessage(std::string msg);
        void setSocket(cs457::tcpUserSocket *sckt);
        bool getBanStatus();
        void ban();
        void unBan();
        void sendMsg(std::string msg);
};


#endif //SERVER_USER_H
