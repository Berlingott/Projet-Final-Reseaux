cmake_minimum_required(VERSION 3.24)
project(Serveur___Cible)

set(CMAKE_CXX_STANDARD 17)

add_library(
        MyPNet STATIC
        ../MyPNet/IncludeMe.h
        ../MyPNet/Network.h
        ../MyPNet/Network.cpp
        ../MyPNet/IPVersion.h
        ../MyPNet/Socket.h
        ../MyPNet/Socket.cpp
        ../MyPNet/SocketHandle.h
        ../MyPNet/SocketOptions.h
        ../MyPNet/IPEndpoint.cpp
        ../MyPNet/IPEndpoint.h
        ../MyPNet/Packet.cpp
        ../MyPNet/Packet.h
)

set_target_properties(MyPNet PROPERTIES LINKER_LANGUAGE CXX)
#target_include_directories( MyPNet PUBLIC ../MyPNet)
add_executable(Serveur___Cible main.cpp  Client.h Client.cpp PacketType.h Settings.h Settings.cpp OperationState.h installationAndStart.cpp installationAndStart.h ) #todo add cmd file

target_link_libraries(Serveur___Cible PUBLIC MyPNet wsock32 ws2_32 )

#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static -mwindows") todo remettre a la fin pour cacher le terminal

