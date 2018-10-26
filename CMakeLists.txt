cmake_minimum_required(VERSION 3.9)

project (HADController)
SET(CMAKE_BUILD_TYPE RELEASE)


include_directories(include)


add_library(HADLib
	src/main.cpp
	src/Engine.hpp
	src/HADController.hpp
)
target_link_libraries(HADLib -lEposCmd)
target_link_libraries(HADLib -L/opt/software/epos/lib/)


add_executable(HADController src/main.cpp)
target_link_libraries(HADController HADLib)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

