//
// Created by marks on 10/29/18.
//

#include "OutputManager.h"
#include "Protocols.h"

OutputManager::OutputManager(Server *server) {
    this-> server = server;
}

void OutputManager::addToQueue(std::string msg, std::string channel, std::string user) {
    Message message(msg,channel,user);
    std::unique_lock<std::mutex> locker(mtx);
    messages.push(message);
    locker.unlock();
    cv.notify_all();
}

void OutputManager::outputThread(OutputManager &out){
    while (out.ready)
    {
        std::unique_lock<std::mutex> locker(out.mtx);
        out.cv.wait(locker);
        if (out.messages.size() > 0) {
            Message message = out.messages.front();
            out.server->sendMsg(message.msg, message.channel, message.user);
            out.messages.pop();
        }
        locker.unlock();
        out.cv.notify_all();
    }

}

void OutputManager::stop() {
    ready = false;
    cv.notify_all();
    ot->join();
}
void OutputManager::start(){
    ot = std::make_unique<std::thread>(outputThread, std::ref(*this));
}