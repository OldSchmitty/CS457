//
// Created by Mark Smith on 10/24/18.
//
#include <iostream>
#include <string>
#include <thread>
#include <regex>
#include <fstream>
#include "tcpUserSocket.h"
#include "Protocols.h"
using namespace std;
const int PORT = 0;
const int HOST_NAME = 1;
const int USER_NAME = 2;
const int CONFIG_FILE = 3;
const int TEST_FILE = 4;
const int LOG_FILE = 5;

bool stop = false;

int listener(shared_ptr<cs457::tcpUserSocket> userSocket){
    string msg;
    regex reg("\r\n");
    int val;
    while(!stop){
        tie(msg,val) = userSocket.get()->recvString();
        vector<string> sVect = Protocols::split(msg,'\000');
        if (val <= 0) {
            stop = true;
            if (val == 0)
                cout<<"The server has closed the connection."<<endl;
        }
        else if(msg != "")
            cout<<std::regex_replace(msg, reg, "")<<endl;
    }
    return 1;
}


int main(int argc, char* argv[]){
    bool debug = false;
    bool logging = false;
    string parameters[6] = {"50000","127.0.0.1","","/home/marks/School/CS457/Client/conf/chatclient.conf","",""};
    string channel = "";
    if (argc %2 == 0){
        cout<<"Error: missing value or argument.\n";
    }
    else{
        for( int i = 1; i < argc-1; i+=2) {
            //cout<<argv[i]<<"\t"<<argv[i+1]<<"\n";
            string arg(argv[i]);
            if (arg == "-p"){
                //cout<<"setting port: "<<argv[i+1]<<"\n";
                parameters[PORT] = string(argv[i+1]);
            }
            else if (arg == "-h") {
                parameters[HOST_NAME] = string(argv[i + 1]);
                //cout << "setting host name: " << argv[i + 1] << "\n";
            }
            else if (arg == "-u"){
                parameters[USER_NAME] = string(argv[i+1]);
                //cout<<"setting user name: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-c"){
                parameters[CONFIG_FILE] = string(argv[i+1]);
                //cout<<"setting config file: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-t"){
                parameters[TEST_FILE] = string(argv[i+1]);
               // cout<<"setting test file: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-L") {
                parameters[LOG_FILE] = string(argv[i + 1]);
                //cout << "setting log file: "<<argv[i+1]<<"\n";
            }
        }
        int serverSocketd;
        if (parameters[CONFIG_FILE] == "")
            parameters[CONFIG_FILE] = string(argv[0]) + "/conf/chatclient.conf";
        std::ifstream is;
        is.open(parameters[CONFIG_FILE]);
        std::string input = "";
        while(getline(is, input)) {
            std::vector<std::string> sVect = Protocols::split(input, '\t');
            if (sVect.size() >= 2){
                if (sVect[0] == "last_server_used"){

                }
                if (sVect[0] == "port"){
                    if (parameters[PORT] != "")
                        parameters[PORT] = sVect[1];
                }
                if (sVect[0] == "default_debug_mode"){
                    if (sVect[1] == "true")
                        debug = true;
                }
                if (sVect[0] == "log"){
                    if (sVect[1] == "true")
                        logging = true;
                }
                if (sVect[0] == "default_log_file"){
                    if (parameters[LOG_FILE] == "")
                        parameters[LOG_FILE] = sVect[1];
                }

            }
        }
        is.close();
        shared_ptr<cs457::tcpUserSocket> userSocket = make_shared<cs457::tcpUserSocket>(parameters[HOST_NAME].c_str(),stoul(parameters[PORT]));
        std::thread childT1(&listener, userSocket);
        string msg;
        if (parameters[TEST_FILE] != ""){
            is.open(parameters[TEST_FILE]);
            while(getline(is, msg))
            {
                if (channel == "" && msg.find("/") == -1){
                    cout<<"Not connected to a channel, only commands starting in / can be used"<<endl;
                }
                else {
                    msg = Protocols::makeMessage(msg, channel);
                    userSocket->sendString(msg);
                }
            }
            sleep(50);
        }
        else {
            getline(cin, msg);
            while (!stop) {
                if (channel == "" && msg.find("/") == -1) {
                    cout << "Not connected to a channel, only commands starting in / can be used" << endl;
                } else {
                    msg = Protocols::makeMessage(msg, channel);
                    if(msg == "QUIT"){
                        stop = true;
                    }
                    else {
                        userSocket->sendString(msg);
                    }
                }
                if(!stop)
                    getline(cin, msg);
            }
        }
        stop = true;
        userSocket->closeSocket();
        childT1.join();
    }
    return 0;
}

