cmake_minimum_required(VERSION 3.9)

project (HADController)
SET(CMAKE_BUILD_TYPE RELEASE)

ADD_LIBRARY(EposCmd
	Engine.cpp
)

target_link_libraries(EposCmd -lEposCmd)
target_link_libraries(EposCmd -L/opt/software/epos/lib/)


#include_directories(source)
#set(SOURCES src/mainapp.cpp src/Student.cpp)
file(GLOB SOURCES "source/*.hpp")


#target_link_libraries(-lEposCmd)

add_executable(HADController main.cpp)
#add_executable(HADController ${SOURCES})


target_link_libraries(HADController EposCmd)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#install (TARGETS HADContoller DESTINATION bin)
#install (FILES source.cpp DESTINATION include)



