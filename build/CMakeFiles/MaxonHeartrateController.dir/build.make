# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thomas/Dokumente/HADSoftware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thomas/Dokumente/HADSoftware/build

# Include any dependencies generated for this target.
include CMakeFiles/MaxonHeartrateController.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MaxonHeartrateController.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MaxonHeartrateController.dir/flags.make

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o: CMakeFiles/MaxonHeartrateController.dir/flags.make
CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Dokumente/HADSoftware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o -c /home/thomas/Dokumente/HADSoftware/src/main.cpp

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thomas/Dokumente/HADSoftware/src/main.cpp > CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.i

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thomas/Dokumente/HADSoftware/src/main.cpp -o CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.s

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.requires

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.provides: CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/MaxonHeartrateController.dir/build.make CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.provides

CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.provides.build: CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o


# Object files for target MaxonHeartrateController
MaxonHeartrateController_OBJECTS = \
"CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o"

# External object files for target MaxonHeartrateController
MaxonHeartrateController_EXTERNAL_OBJECTS =

libMaxonHeartrateController.a: CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o
libMaxonHeartrateController.a: CMakeFiles/MaxonHeartrateController.dir/build.make
libMaxonHeartrateController.a: CMakeFiles/MaxonHeartrateController.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Dokumente/HADSoftware/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMaxonHeartrateController.a"
	$(CMAKE_COMMAND) -P CMakeFiles/MaxonHeartrateController.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MaxonHeartrateController.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MaxonHeartrateController.dir/build: libMaxonHeartrateController.a

.PHONY : CMakeFiles/MaxonHeartrateController.dir/build

CMakeFiles/MaxonHeartrateController.dir/requires: CMakeFiles/MaxonHeartrateController.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/MaxonHeartrateController.dir/requires

CMakeFiles/MaxonHeartrateController.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MaxonHeartrateController.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MaxonHeartrateController.dir/clean

CMakeFiles/MaxonHeartrateController.dir/depend:
	cd /home/thomas/Dokumente/HADSoftware/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Dokumente/HADSoftware /home/thomas/Dokumente/HADSoftware /home/thomas/Dokumente/HADSoftware/build /home/thomas/Dokumente/HADSoftware/build /home/thomas/Dokumente/HADSoftware/build/CMakeFiles/MaxonHeartrateController.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MaxonHeartrateController.dir/depend

