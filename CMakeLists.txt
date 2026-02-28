cmake_minimum_required(VERSION 3.16)
project(P2PChat VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(P2PChat
    src/main.cpp
    src/NetworkManager.cpp
    src/ChatWindow.cpp
    include/NetworkManager.h
    include/ChatWindow.h
)

target_link_libraries(P2PChat PRIVATE Qt6::Widgets Qt6::Network)
target_include_directories(P2PChat PRIVATE include)