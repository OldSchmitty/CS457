//
// Created by marks on 10/26/18.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H
#include <string>
#include "tcpUserSocket.h"
#include <vector>
#include <mutex>

const int USER = 0;
const int CHANOP = 1;
const int SYSOP = 2;
const int ADMIN = 3;

class User {
    private:
        std::string userName = "";
        std::string passWord = "";
        std::string nick = "";
        std::string realName = "";
        cs457::tcpUserSocket *scktPointer;
        int level = 0;
        bool active = false;
        bool banStatus = false;
        mutable std::mutex *mtx = new std::mutex;
        bool away;
        std::string awayMessage;


    public:
        User();
        ~User();
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
        void setSocket(cs457::tcpUserSocket *sckt);
        bool getBanStatus();
        void ban();
        void unBan();
        void sendMsg(std::string msg);
        void setRealName(std::string name);
        std::string getRealName();
        void disconnect();
        void setAway(bool status, std::string msg = "");
        bool getAway();
        std::string getAwayMessage();
	std::vector<std::string> userChannels;
	
};


#endif //SERVER_USER_H
