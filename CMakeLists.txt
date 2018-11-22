cmake_minimum_required(VERSION 3.9)

project (HeartbeatController)
SET(CMAKE_BUILD_TYPE RELEASE)


include_directories(include)


add_library(HADLib
	src/main.cpp
	src/Engine.hpp
	src/HADController.hpp
	src/InputFileParser.hpp
	src/Factory.hpp
)
target_link_libraries(HADLib -lEposCmd)
target_link_libraries(HADLib -L/opt/software/epos/lib/)


add_executable(HeartbeatController src/main.cpp)
target_link_libraries(HeartbeatController HADLib)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

