# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andreas/git/logger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andreas/git/logger/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cpp_sandbox.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/cpp_sandbox.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cpp_sandbox.dir/flags.make

CMakeFiles/cpp_sandbox.dir/src/main.cpp.o: CMakeFiles/cpp_sandbox.dir/flags.make
CMakeFiles/cpp_sandbox.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andreas/git/logger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cpp_sandbox.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cpp_sandbox.dir/src/main.cpp.o -c /home/andreas/git/logger/src/main.cpp

CMakeFiles/cpp_sandbox.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpp_sandbox.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andreas/git/logger/src/main.cpp > CMakeFiles/cpp_sandbox.dir/src/main.cpp.i

CMakeFiles/cpp_sandbox.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpp_sandbox.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andreas/git/logger/src/main.cpp -o CMakeFiles/cpp_sandbox.dir/src/main.cpp.s

# Object files for target cpp_sandbox
cpp_sandbox_OBJECTS = \
"CMakeFiles/cpp_sandbox.dir/src/main.cpp.o"

# External object files for target cpp_sandbox
cpp_sandbox_EXTERNAL_OBJECTS =

cpp_sandbox: CMakeFiles/cpp_sandbox.dir/src/main.cpp.o
cpp_sandbox: CMakeFiles/cpp_sandbox.dir/build.make
cpp_sandbox: CMakeFiles/cpp_sandbox.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andreas/git/logger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cpp_sandbox"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpp_sandbox.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cpp_sandbox.dir/build: cpp_sandbox
.PHONY : CMakeFiles/cpp_sandbox.dir/build

CMakeFiles/cpp_sandbox.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpp_sandbox.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpp_sandbox.dir/clean

CMakeFiles/cpp_sandbox.dir/depend:
	cd /home/andreas/git/logger/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andreas/git/logger /home/andreas/git/logger /home/andreas/git/logger/cmake-build-debug /home/andreas/git/logger/cmake-build-debug /home/andreas/git/logger/cmake-build-debug/CMakeFiles/cpp_sandbox.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpp_sandbox.dir/depend
