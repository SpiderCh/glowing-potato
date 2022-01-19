cmake_minimum_required(VERSION 3.20.3)

project("ccs" VERSION 0.1)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/server.cpp)
add_executable(sender ${CMAKE_CURRENT_LIST_DIR}/sender.cpp)
add_executable(receiver ${CMAKE_CURRENT_LIST_DIR}/receiver.cpp)