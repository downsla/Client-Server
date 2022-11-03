cmake_minimum_required(VERSION 3.00)
project(clientserver VERSION 0.1.0)

add_executable(server server.cpp)
add_executable(client client.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lboost_system -pthread")