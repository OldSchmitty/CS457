#include <iostream>
#include <string> 
#include <tuple> 
#include <thread>
#include <memory> 
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include <map>
#include "Server.h"
#include <condition_variable>
#include <mutex>
#include "Protocols.h"
#include <vector>
#include "OutputManager.h"

using namespace std;
bool ready = true;
mutex mtx;
const int PORT = 0;
const int CONFIG_FILE = 1;
const int DBPATH = 2;

int numUsers = 0;

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket, Server &server, OutputManager &out){
    User *user = new User();
    user->setSocket(clientSocket.get());
    user->setNick(clientSocket.get()->getUniqueIdentifier());
    std::string msg;
    ssize_t val;
    bool cont =true;

    while (cont)
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (val > 0) {
            vector<string> sVect = Protocols::split(msg, '\000');

            for (int i = 0; i < sVect.size(); i++) {
                string endString = sVect[i].substr(sVect[i].size() - 2);
                if (sVect[i].substr(sVect[i].size() - 2) == "\r\n") {
                    sVect[i] = sVect[i].substr(0, sVect[i].size() - 2);
                    cout << "RECIEVED FROM " + clientSocket->getUniqueIdentifier()+" "+user->getUserName() + ":" + sVect[i]<<endl;
                    vector<string> outMessageV = Protocols::parseMessage(sVect[i], user, server, clientSocket.get());
                    if (outMessageV[0] == "DIE"){
                        cout<<"SERVER IS BEING SHUT DOWN BY USER "+user->getNick()<<endl;
                        server.shutDown();
                        ready = false;
                    }
                    else if (!outMessageV[0].empty()) {
                        out.addToQueue(outMessageV[0], outMessageV[1], outMessageV[2]);
                    }
                }
            }
        }else{
            cont = false;
        }
    }
    server.disconnect(user->getUserName());
    clientSocket.get()->closeSocket();
    mtx.lock();
    numUsers--;
    mtx.unlock();
    return 1;
}

int backgroundListener(string parameters[], cs457::tcpServerSocket &mysocket, Server& server){
    map <string, unique_ptr<thread>> threadM;
    OutputManager out(&server);
    out.start();
    cout<<"Listening on port "<<parameters[PORT]<<endl;
    while (ready)
    {
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val;
        tie(clientSocket,val) = mysocket.acceptSocket();
        if (val > 0) {
            clientSocket.get()->sendString(server.getBanner());
            unique_ptr<thread> t = make_unique<thread>(cclient, clientSocket, std::ref(server), std::ref(out));
            threadM[clientSocket->getUniqueIdentifier()] = std::move(t);
            mtx.lock();
            numUsers++;
            mtx.unlock();
        }
    }
    server.shutDown();
    for (auto item = threadM.begin(); item != threadM.end(); item++){
        item->second->join();
    }
    out.stop();
    return 1;
}


int main(int argc, char * argv[]) {
    string parameters[3] = {"2000","",string(argv[0])+"/"};
    vector<string> addPorts;
    if (argc %2 == 0){
        cout<<"Error: missing value or argument.\n";
    }
    else{
        for( int i = 1; i < argc-1; i+=2) {
            string arg(argv[i]);
            if (arg == "-port"){
                parameters[PORT] = string(argv[i+1]);
            }
            else if (arg == "-configuration"){
                parameters[CONFIG_FILE] = string(argv[i+1]);
            }
            else if (arg == "-db"){
                parameters[DBPATH] = string(argv[i+1]);
            }
        }
    }
    if (parameters[CONFIG_FILE] == "") {
        parameters[CONFIG_FILE] = string(argv[0]) + "conf/chatserver.conf";
    }
    std::ifstream is;
    is.open(parameters[CONFIG_FILE]);
    std::string input;
    while (getline(is, input)) {
        std::vector<std::string> sVect = Protocols::split(input, '\t');
        if (sVect.size() >= 2) {
            if (sVect[0] == "-port") {
                parameters[PORT] = sVect[1];
            } else if (sVect[0] == "dbpath" && parameters[DBPATH] == "") {
                parameters[DBPATH] = sVect[1];
            } else if (sVect[0] == "additional_ports") {
                addPorts = Protocols::split(sVect[1], ',');
            }
        }
    }
    is.close();

    cs457::tcpServerSocket mysocket(stoul(parameters[PORT]));

    mysocket.bindSocket();
    mysocket.listenSocket();
    Server server(parameters[DBPATH]);
    server.setInfoString("Version: 1.0\n\tOwner: Mark Smith");
    unique_ptr<thread> bgThread = make_unique<thread>(backgroundListener, parameters, std::ref(mysocket), std::ref(server));
    input = "";
    while(input != "EXIT"){
        cin>>input;
        if (input== "EXIT"){
            ready = false;
            bgThread->detach();
            mysocket.safeClose();
        }
        else if (input == "help"){
            cout<<"Current Supported Commands are:\n"
            <<"help -- shows this menu\n"
            <<"connections -- shows how many users are connected\n"
            <<"userlist -- outputs lists of users currectly active"
            <<"EXIT -- shuts down the server"<<endl;
        }
        else if (input == "userlist"){
           server.showUsers();
        }
        else if (input == "connections"){
            mtx.lock();
            cout<<"There are "<<numUsers<<" users currently connected"<<endl;
            mtx.unlock();
        }
    }
    return 0;
}