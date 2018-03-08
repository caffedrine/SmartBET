cmake_minimum_required(VERSION 3.8)
project(SmartBET)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl -lmyhtml -fpermissive")


set(SOURCE_FILES
        main.cpp
        IFetchBets.h
        CasaPariurilor.cpp
        CasaPariurilor.h
        eFortuna.cpp
        eFortuna.h
        Http.h
        util.h util.cpp Betano.cpp Betano.h StanleyBet.cpp StanleyBet.h)

add_executable(SmartBET ${SOURCE_FILES}) # <----------------------------------

# Curl
target_link_libraries(SmartBET curl) # <----------------------------------

# myhtml
target_link_libraries(SmartBET myhtml)
