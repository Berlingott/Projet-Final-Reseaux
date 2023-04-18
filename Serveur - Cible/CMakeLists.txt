cmake_minimum_required(VERSION 3.24)
project(Serveur___Cible)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static -mwindows")

add_executable(Serveur___Cible main.cpp Running.cpp Settings.h Settings.cpp StringConvertion.h StringConvertion.cpp Client.h Client.cpp CommandPrompt.h CommandPrompt.cpp utility.h utility.cpp)
