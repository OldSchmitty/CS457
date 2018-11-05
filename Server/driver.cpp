#include <iostream>
#include <string> 
#include <tuple> 
#include <thread>
#include <memory> 
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include <map>
#include "ChatServer.h"
#include <condition_variable>
#include <mutex>
#include "CClient.h"
#include "Protocols.h"
#include <vector>
#include "OutputManager.h"

using namespace std;
bool ready = true;
mutex mtx;

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket, ChatServer &server, OutputManager &out){
    User *user = new User();
    user->setSocket(clientSocket.get());
    std::string msg;
    ssize_t val;
    bool cont =true;

    while (cont)
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (val != 0) {
            cout<<"RECIEVED MESSAGE: "+msg<<endl;
            string endString =msg.substr(msg.size()-2);
            if (msg.substr(msg.size()-2) == "\r\n") {
                msg = msg.substr(0, msg.size() - 2);
                vector<string> outMessageV = Protocols::parseMessage(msg, user, server, clientSocket.get());
                if (!outMessageV[0].empty()) {
                    out.addToQueue(outMessageV[0], outMessageV[1], outMessageV[2]);
                }
            }
        }else{
            cont = false;
        }
    }
    server.disconnect(user->getUserName());
    clientSocket.get()->closeSocket();
    return 1;

}


int main(int argc, char * argv[]) {
    cout << "Initializing Socket" << std::endl;
    cs457::tcpServerSocket mysocket(2000);
    cout << "Binding Socket" << std::endl; 
    mysocket.bindSocket(); 
    cout << "Listening Socket" << std::endl; 
    mysocket.listenSocket(); 
    cout << "Waiting to Accept Socket" << std::endl;
    map <string, unique_ptr<thread>> threadM;
    ChatServer server;
    server.loadUserFile("/home/marks/School/CS457/Server/users");
    server.loadChannelFile("/home/marks/School/CS457/Server/channels");
    OutputManager out(&server);
    out.start();
    while (ready)
    { 
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val; 
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl; 
        cout << "Socket Accepted" << std::endl;
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket, std::ref(server), std::ref(out));
        threadM[clientSocket->getUniqueIdentifier()] = std::move(t);
    }

    for (auto item = threadM.begin(); item != threadM.end(); item++){
        item->second->join();
    }
    out.stop();
    //TODO: CLOSE THE SERVER SOCKET AND END PROGRAM SOMEHOW??? Thread for doing commands at server?
    //TODO: full argument parser for all of the commands
    //TODO: actually broadcasting messages into channels
    //TODO: config file support (all channel and user info in one file)
    return 0;
}