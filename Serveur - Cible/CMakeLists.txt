#########
#       CMakeList
#
#
#
#
#
###################################################

cmake_minimum_required(VERSION 3.24) #Version de compilation

project(Serveur___Cible) #Nom du projet

set(CMAKE_CXX_STANDARD 17)

add_library(        #toutes les fichiers externes.
                    #Dans ce cas-ci, uniquement des fichier de la librairie que nous avons créé.
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

set_target_properties(MyPNet PROPERTIES LINKER_LANGUAGE CXX) #Lie le namespace MyPNet au projet - Projet personnel
#target_include_directories( MyPNet PUBLIC ../MyPNet)
add_executable(
        Serveur___Cible main.cpp # Ajoutons tous les fichier du projet pour l'exécution
        Settings.h Settings.cpp
        OperationState.h
        installationAndStart.cpp installationAndStart.h
        Conversion.cpp Conversion.h
        Connexion.cpp Connexion.h
        ConnexionType.h
        RrecptionEnvoie.cpp
        CMD.cpp CMD.h
        Utility.cpp Utility.h)

#todo add cmd file

target_link_libraries(Serveur___Cible PUBLIC MyPNet wsock32 ws2_32 ) #Spécification des librairies externe utilisées

#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static -mwindows") # Permet de ne pas voir de fenêtre d'exécution

