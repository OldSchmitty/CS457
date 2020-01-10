cmake_minimum_required(VERSION 3.12)
project(Server)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)

add_executable(ChatServer
        driver.cpp
        tcpUserSocket.h
        tcpUserSocket.cpp
        tcpServerSocket.h
        tcpServerSocket.cpp
        cmake-build-debug/CMakeFiles/main.cpp Channel.cpp Channel.h Protocols.h ChatServer.cpp ChatServer.h User.cpp User.h CClient.cpp CClient.h OutputManager.cpp OutputManager.h Protocols.cpp)