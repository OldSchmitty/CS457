cmake_minimum_required(VERSION 3.12)
project(ChatClient)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)
add_executable(Client
        tcpUserSocket.cpp
        tcpUserSocket.cpp
        ClientDriver.cpp Protocols.cpp Protocols.h)
