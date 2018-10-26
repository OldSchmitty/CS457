#include <iostream>
#include <string> 
#include <tuple> 
#include <thread> 
#include <vector> 
#include <memory> 
#include "tcpUserSocket.h"
#include "tcpServerSocket.h"
#include <map>

using namespace std;

bool ready = true; 

int cclient(shared_ptr<cs457::tcpUserSocket> clientSocket,int id)
{
    cout << "Waiting for message from Client Thread" << id << std::endl;
    string msg;
    ssize_t val;
    bool cont =true ;  
    while (cont) 
    {
        tie(msg,val) = clientSocket.get()->recvString();
        if (msg.substr(0,4) != "EXIT" && val != 0) {

            //TODO: Check channels, send message out to appropriate channel connection.
            cout << "[SERVER] The client is sending message " << msg << " -- With value return = " << val << endl;
            string s = "[SERVER REPLY] The client is sending message:" + msg + "\n";
            thread childT1(&cs457::tcpUserSocket::sendString, clientSocket.get(), s, true);
            childT1.join();

            if (msg.substr(0, 6) == "SERVER") {
                thread childTExit(&cs457::tcpUserSocket::sendString, clientSocket.get(), "GOODBYE EVERYONE", false);
                thread childTExit2(&cs457::tcpUserSocket::sendString, clientSocket.get(), "\n", false);
                ready = false;
                cont = false;
                childTExit.join();
                childTExit2.join();
            } else {
                cout << "waiting for another message" << endl;
            }
        }else{
            cont = false;
        }
    }

    clientSocket.get()->sendString("goodbye");
    clientSocket.get()->closeSocket(); 
    return 1; 
}

int main(int argc, char * argv[])
{
    cout << "Initializing Socket" << std::endl; 
    cs457::tcpServerSocket mysocket(2000);
    cout << "Binding Socket" << std::endl; 
    mysocket.bindSocket(); 
    cout << "Listening Socket" << std::endl; 
    mysocket.listenSocket(); 
    cout << "Waiting to Accept Socket" << std::endl;
    int id = 0; 
    map<unique_ptr<thread>> map;
  
    while (ready)
    { 
        shared_ptr<cs457::tcpUserSocket> clientSocket;
        int val; 
        tie(clientSocket,val) = mysocket.acceptSocket();
        cout << "value for accept is " << val << std::endl; 
        cout << "Socket Accepted" << std::endl; 
        unique_ptr<thread> t = make_unique<thread>(cclient,clientSocket,id); 
        threadList.push_back(std::move(t)); 
        
        id++; //not the best way to go about it. 
       // threadList.push_back(t);
        
    }

    for (auto& t: threadList)
    {
        t.get()->join();
    }

    return 0;
}