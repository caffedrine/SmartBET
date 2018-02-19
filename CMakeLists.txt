cmake_minimum_required(VERSION 3.8)
project(SmartBET)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES main.cpp IFetchBets.h CasaPariurilor.cpp CasaPariurilor.h eFortuna.cpp eFortuna.h Http.h)
add_executable(SmartBET ${SOURCE_FILES})