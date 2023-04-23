cmake_minimum_required(VERSION 3.24)
project(chudansmarde)

set(CMAKE_CXX_STANDARD 17)

add_executable(chudansmarde main.cpp)


target_link_libraries(chudansmarde PUBLIC wsock32 ws2_32  )