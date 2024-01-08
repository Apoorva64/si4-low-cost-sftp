# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/apoorva64/SecTrans2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/apoorva64/SecTrans2/cmake-build-debug

# Include any dependencies generated for this target.
include external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/compiler_depend.make

# Include the progress variables for this target.
include external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/progress.make

# Include the compile flags for this target's objects.
include external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/flags.make

external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o: external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/flags.make
external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o: ../external-libs/ThreadPool/ThreadPool.cpp
external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o: external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o -MF CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o.d -o CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o -c /home/apoorva64/SecTrans2/external-libs/ThreadPool/ThreadPool.cpp

external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ThreadPool.dir/ThreadPool.cpp.i"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/apoorva64/SecTrans2/external-libs/ThreadPool/ThreadPool.cpp > CMakeFiles/ThreadPool.dir/ThreadPool.cpp.i

external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ThreadPool.dir/ThreadPool.cpp.s"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/apoorva64/SecTrans2/external-libs/ThreadPool/ThreadPool.cpp -o CMakeFiles/ThreadPool.dir/ThreadPool.cpp.s

# Object files for target ThreadPool
ThreadPool_OBJECTS = \
"CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o"

# External object files for target ThreadPool
ThreadPool_EXTERNAL_OBJECTS =

external-libs/ThreadPool/libThreadPool.a: external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/ThreadPool.cpp.o
external-libs/ThreadPool/libThreadPool.a: external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/build.make
external-libs/ThreadPool/libThreadPool.a: external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libThreadPool.a"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && $(CMAKE_COMMAND) -P CMakeFiles/ThreadPool.dir/cmake_clean_target.cmake
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ThreadPool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/build: external-libs/ThreadPool/libThreadPool.a
.PHONY : external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/build

external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/clean:
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool && $(CMAKE_COMMAND) -P CMakeFiles/ThreadPool.dir/cmake_clean.cmake
.PHONY : external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/clean

external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/depend:
	cd /home/apoorva64/SecTrans2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apoorva64/SecTrans2 /home/apoorva64/SecTrans2/external-libs/ThreadPool /home/apoorva64/SecTrans2/cmake-build-debug /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external-libs/ThreadPool/CMakeFiles/ThreadPool.dir/depend
