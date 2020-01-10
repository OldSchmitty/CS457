//
// Created by marks on 10/28/18.
//

#ifndef CHATCLIENT_PROTOCOLS_H
#define CHATCLIENT_PROTOCOLS_H


#include <string>
#include <vector>

class Protocols {
    public:
        static std::string makeMessage(std::string str, std::string &channel);
        static std::vector<std::string> split(const std::string& s, char delimiter);
        static std::string commandFactory(std::string command, std::vector<std::string> args, std::string &channel);
};

#endif //CHATCLIENT_PROTOCOLS_H
