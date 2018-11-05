//
// Created by marks on 10/26/18.
//

#ifndef SERVER_CLIENTMANAGER_H
#define SERVER_CLIENTMANAGER_H
#include <map>
#include <string>
#include "Channel.h"
#include "ChatServer.h"

class ClientManager {
    private:
        std::map<std::string, bool> connections;


    public:
        ClientManager();
        void addConnection(std::string userName, std::string channelName);
        void getStatus(std::string userName);
        void logout(std::string userName);
        void leave(std::string userName, std::string channel);


    /*channel map
     *
     *
     *
     */

};


#endif //SERVER_CLIENTMANAGER_H
