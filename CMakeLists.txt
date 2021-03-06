cmake_minimum_required(VERSION 3.7)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

project(SmartBET)
PROJECT(${PROJECT_NAME} C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl -lmyhtml -ansi -pedantic")

######################################Include everuthing src##################################
file(GLOB_RECURSE Proj_SOURCES "src/*.cpp" "src/*.c" "include/*.c" "include/*.cpp")
file(GLOB_RECURSE Proj_HEADERS "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

set( Proj_INCLUDE_DIRS "include/")
foreach (_headerFile ${Proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Proj_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Proj_INCLUDE_DIRS)
include_directories(${Proj_INCLUDE_DIRS})
link_directories(${Proj_INCLUDE_DIRS})
##############################################################################################

add_executable (${PROJECT_NAME} ${Proj_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Proj_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} myhtml)

# Copy executable to 'build/Exe' directory
set(EXE_NAME "${PROJECT_NAME}")
add_custom_command(TARGET "${EXE_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/build/Release/${EXE_NAME} ${PROJECT_SOURCE_DIR}/build/Exe/${EXE_NAME}
    COMMENT "Objcopy: Copying executables to output directory: 'build/Exe'" )

# Copy linked libraries to output directory