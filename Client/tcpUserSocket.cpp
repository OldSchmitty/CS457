#include "tcpUserSocket.h"
#include <memory> 
#include <cstring> 
#include <mutex> 
#include <netinet/in.h>
#include <iostream>

using namespace std; 

cs457::tcpUserSocket::tcpUserSocket() {};

cs457::tcpUserSocket::tcpUserSocket(int sckt){
    setSocket(sckt);
}

void cs457::tcpUserSocket::setSocket(int sckt)  
{ 
    userSocket=sckt;
}

cs457::tcpUserSocket::tcpUserSocket(const char* sAddress, uint16_t port){
    if ((userSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
    }

    memset(&userAddress, '0', sizeof(userAddress));
    userAddress.sin_family = AF_INET;
    userAddress.sin_port = htons(port);
    if(inet_pton(AF_INET, sAddress, &userAddress.sin_addr)<=0){
        std::cout<<"error connecting";
    }
    if (connect(userSocket, (struct sockaddr *)&userAddress, sizeof(userAddress)) < 0) {
        std::cout<<"\nConnection Failed \n";
    }
    clientAddressIPv4 = sAddress;
    clientPortIPv4 = port;

}

socklen_t cs457::tcpUserSocket::getLenghtPointer()
{
    socklen_t len = sizeof(userAddress);  
    return len;
}

int cs457::tcpUserSocket::getSocket()
{
    return userSocket;
}

int cs457::tcpUserSocket::closeSocket()
{
    shutdown(userSocket, SHUT_RDWR);
    return close(userSocket);
}

void cs457::tcpUserSocket::setUserInfoIPv4(string address, uint16_t port)
{
    clientAddressIPv4 = address;
    clientPortIPv4 = port; 
}

std::tuple<string,ssize_t> cs457::tcpUserSocket::recvString(int bufferSize, bool useMutex)
{
    char stringBuffer[bufferSize];
    memset(stringBuffer, 0, sizeof(stringBuffer));    //change made here. Zeros out buffer.

    ssize_t recvMsgSize;

    if (useMutex)
    {
        lock_guard<mutex> lock(recvMutex);
        recvMsgSize = recv(userSocket, stringBuffer, bufferSize, 0); 
    }    
    else
    {
        recvMsgSize = recv(userSocket, stringBuffer, bufferSize, 0); 
    }
    string msg = "";
    int start = 0;
    for(int i = 0; i<recvMsgSize; i++){
        if (stringBuffer[i] == '\r')
            if(i < recvMsgSize-1 && stringBuffer[i+1] == '\n') {
                msg += string(&stringBuffer[start], i + 2 - start);
                start = i + 2;
                i++;
            }
    }
    return make_tuple(string(stringBuffer),recvMsgSize);     
};
        

ssize_t cs457::tcpUserSocket::sendString(const string & data, bool useMutex)
{
    //https://stackoverflow.com/questions/7352099/stdstring-to-char
    if (data.size() == 0) return 0;                 
    int bufferSize = data.size()+1; 
    vector<char> stringBuffer(data.c_str(), data.c_str() + data.size() + 1u);

    ssize_t rval; 
    if (useMutex)
    {
       lock_guard<mutex> lock(sendMutex);
       rval = send(userSocket, &stringBuffer[0], bufferSize, 0);
    }
    else
    {
       rval = send(userSocket, &stringBuffer[0], bufferSize, 0);     
    }

    return rval;
}

string cs457::tcpUserSocket::getUniqueIdentifier()
{ 
    //this unique identifier is arbitrary 
    //and it may not be useful for chat program (see other classes)
    //it has not been tested to be unique but for now, we don't need it. 
     string identifier  = "[" + clientAddressIPv4 + "," + to_string(clientPortIPv4) + "]"; 
     return identifier; 
};

struct sockaddr * cs457::tcpUserSocket::getAddressPointer()
{
    return ((struct sockaddr *) &userAddress);
};