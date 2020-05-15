cmake_minimum_required(VERSION 3.10)
project(OoTMM_Tools)

add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)

add_executable(yaz0 yaz0.cpp)
add_executable(unyaz0 unyaz0.cpp)

install(TARGETS yaz0 unyaz0 RUNTIME DESTINATION "bin")
