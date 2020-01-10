cmake_minimum_required(VERSION 3.12)
project(Server)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)

add_executable(ChatServer
        ChatServer.cpp
        tcpUserSocket.h
        tcpUserSocket.cpp
        tcpServerSocket.h
        tcpServerSocket.cpp
        cmake-build-debug/CMakeFiles/Channel.cpp Channel.h Protocols.h Server.cpp Server.h User.cpp User.h OutputManager.cpp OutputManager.h Protocols.cpp)