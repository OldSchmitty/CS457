//
// Created by Mark Smith on 10/24/18.
//
#include<iostream>
#include<string>
#include"tcpUserSocket.h"
using namespace std;
const int PORT = 0;
const int HOST_NAME = 1;
const int USER_NAME = 2;
const int CONFIG_FILE = 3;
const int TEST_FILE = 4;
const int LOG_FILE = 5;




int main(int argc, char* argv[]){
    string parameters[6] = {"2000","","","","",""};
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
        int serverSocketd;
        shared_ptr<cs457::tcpUserSocket> userSocket = make_shared<cs457::tcpUserSocket>("127.0.0.1",2000);
        userSocket->sendString("HELLO THERE!");

    }
    return 0;
}

