cmake_minimum_required(VERSION 3.24)
project(MyPNet)

set(CMAKE_CXX_STANDARD 17)

add_library(MyPNet IncludeMe.h Network.h Network.cpp SocketHandle.h PResult.h IPVersion.h Socket.cpp Socket.h SocketOptions.h IPEndpoint.h IPEndpoint.cpp Settings.h Settings.h Packet.h Packet.h Packet.cpp)
if(WIN32)
    target_link_libraries(MyPNet wsock32 ws2_32)
endif()