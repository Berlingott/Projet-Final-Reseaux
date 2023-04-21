cmake_minimum_required(VERSION 3.22)
project(Client___Pirate)
set(CMAKE_CXX_STANDARD 14)


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

add_executable(Client___Pirate main.cpp Connexion.cpp Connexion.h ConnexionType.h)

target_link_libraries(Client___Pirate PUBLIC MyPNet wsock32 ws2_32  )