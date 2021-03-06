cmake_minimum_required(VERSION 3.7)

project (HeartbeatController)
SET(CMAKE_BUILD_TYPE RELEASE)


include_directories(include)

add_subdirectory(src)

add_library(MaxonHeartrateController
	src/main.cpp
	src/InputParser.hpp
    src/CommandLineParser.hpp
	src/InputFileParser.hpp
    src/InteractiveParser.hpp
	src/GUIThreadParser.hpp
	src/Factory.hpp
    src/HeartrateController.hpp
    src/MotionMode.hpp
    src/ProfileMode.hpp
    src/ProfilePositionMode.hpp
    src/InterpolatedPositionMode.hpp
)

target_link_libraries(MaxonHeartrateController -lEposCmd)
target_link_libraries(MaxonHeartrateController -L/opt/software/epos/lib/)


add_executable(HeartbeatController src/main.cpp)
target_link_libraries(HeartbeatController MaxonHeartrateController)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/heartbeat_gui.py
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/file.py
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/setup.py
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
