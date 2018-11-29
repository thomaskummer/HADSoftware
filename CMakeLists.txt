cmake_minimum_required(VERSION 3.7)

project (HeartbeatController)
SET(CMAKE_BUILD_TYPE RELEASE)


include_directories(include)


add_library(MaxonHeartrateController
	src/main.cpp
    src/CommandLineParser.hpp
	src/InputFileParser.hpp
    src/InteractiveMode.hpp
	src/Factory.hpp
    src/HeartrateController.hpp
    src/MotionMode.hpp
    src/ProfileMode.hpp
    src/InterpolatedPositionMode.hpp
)

target_link_libraries(MaxonHeartrateController -lEposCmd)
target_link_libraries(MaxonHeartrateController -L/opt/software/epos/lib/)


add_executable(HeartbeatController src/main.cpp)
target_link_libraries(HeartbeatController MaxonHeartrateController)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
