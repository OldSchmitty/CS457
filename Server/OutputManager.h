//
// Created by Mark Smith on 10/29/18.
//

#ifndef SERVER_OUTPUTMANAGER_H
#define SERVER_OUTPUTMANAGER_H
#include <string>
#include <mutex>
#include <vector>
#include "ChatServer.h"
#include <queue>
#include <condition_variable>
#include <thread>

class OutputManager {
    class Message{
        public:
            std::string msg;
            std::string channel;
            std::string user;
            Message(std::string msg, std::string channel, std::string user){
                this->msg = msg;
                this->channel = channel;
                this->user = user;
            }
    };

    public:
        void addToQueue(std::string msg, std::string channel, std::string user);
        OutputManager(ChatServer *server);
        void stop();
        void start();
        bool ready = true;
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<Message> messages;
        ChatServer *server;



private:

        static void outputThread(OutputManager &out);
        std::unique_ptr<std::thread> ot;


};


#endif //SERVER_OUTPUTMANAGER_H
