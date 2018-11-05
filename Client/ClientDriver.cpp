//
// Created by Mark Smith on 10/24/18.
//
#include <iostream>
#include <string>
#include <thread>
#include <regex>
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
        cout<<std::regex_replace(msg, reg, "")<<endl;
    }
    return 1;
}


int main(int argc, char* argv[]){
    string parameters[6] = {"2000","","","","",""};
    string channel = "";
    if (argc %2 == 0){
        cout<<"Error: missing value or argument.\n";
    }
    else{
        for( int i = 1; i < argc-1; i+=2) {
            cout<<argv[i]<<"\t"<<argv[i+1]<<"\n";
            string arg(argv[i]);
            if (arg == "-p"){
                cout<<"setting port: "<<argv[i+1]<<"\n";
                parameters[PORT] = string(argv[i+1]);
            }
            else if (arg == "-h") {
                parameters[HOST_NAME] = string(argv[i + 1]);
                cout << "setting host name: " << argv[i + 1] << "\n";
            }
            else if (arg == "-u"){
                parameters[USER_NAME] = string(argv[i+1]);
                cout<<"setting user name: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-c"){
                parameters[CONFIG_FILE] = string(argv[i+1]);
                cout<<"setting config file: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-t"){
                parameters[TEST_FILE] = string(argv[i+1]);
                cout<<"setting test file: "<<argv[i+1]<<"\n";
            }
            else if (arg == "-L") {
                parameters[LOG_FILE] = string(argv[i + 1]);
                cout << "setting log file: "<<argv[i+1]<<"\n";
            }
        }
        cout<<"Enter password for user "<<parameters[USER_NAME]<<": ";
        string passWord;
        getline(cin, passWord);
        cout<<"Enter nick name: ";
        string nick;
        getline(cin,nick);
        int serverSocketd;
        shared_ptr<cs457::tcpUserSocket> userSocket = make_shared<cs457::tcpUserSocket>("127.0.0.1",2000);
        std::thread childT1(&listener, userSocket);
        string PASS = "PASS "+passWord+"\r\n";
        //sleep(1);
        userSocket->sendString(PASS);
        string NICK = "NICK "+nick+"\r\n";
        //sleep(1);
        userSocket->sendString(NICK);
        string USER = "USER "+parameters[USER_NAME]+"\r\n";
        //sleep(1);
        userSocket->sendString(USER);
        string msg;
        cout<<nick<<": ";
        getline(cin,msg);
        while(msg.find("/EXIT") == -1){
            msg = Protocols::makeMessage(msg, channel);
            userSocket->sendString(msg);
            getline(cin,msg);;
        }
        stop = true;
        userSocket->closeSocket();
        childT1.join();
    }
    return 0;
}

