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

# Utility rule file for ExperimentalMemCheck.

# Include any custom commands dependencies for this target.
include tests/CMakeFiles/ExperimentalMemCheck.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/ExperimentalMemCheck.dir/progress.make

tests/CMakeFiles/ExperimentalMemCheck:
	cd /home/apoorva64/SecTrans2/cmake-build-debug/tests && /usr/bin/ctest -D ExperimentalMemCheck

ExperimentalMemCheck: tests/CMakeFiles/ExperimentalMemCheck
ExperimentalMemCheck: tests/CMakeFiles/ExperimentalMemCheck.dir/build.make
.PHONY : ExperimentalMemCheck

# Rule to build all files generated by this target.
tests/CMakeFiles/ExperimentalMemCheck.dir/build: ExperimentalMemCheck
.PHONY : tests/CMakeFiles/ExperimentalMemCheck.dir/build

tests/CMakeFiles/ExperimentalMemCheck.dir/clean:
	cd /home/apoorva64/SecTrans2/cmake-build-debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalMemCheck.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/ExperimentalMemCheck.dir/clean

tests/CMakeFiles/ExperimentalMemCheck.dir/depend:
	cd /home/apoorva64/SecTrans2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apoorva64/SecTrans2 /home/apoorva64/SecTrans2/tests /home/apoorva64/SecTrans2/cmake-build-debug /home/apoorva64/SecTrans2/cmake-build-debug/tests /home/apoorva64/SecTrans2/cmake-build-debug/tests/CMakeFiles/ExperimentalMemCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/ExperimentalMemCheck.dir/depend
