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
include external-libs/CLI11/CMakeFiles/CLI11.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include external-libs/CLI11/CMakeFiles/CLI11.dir/compiler_depend.make

# Include the progress variables for this target.
include external-libs/CLI11/CMakeFiles/CLI11.dir/progress.make

# Include the compile flags for this target's objects.
include external-libs/CLI11/CMakeFiles/CLI11.dir/flags.make

# Object files for target CLI11
CLI11_OBJECTS =

# External object files for target CLI11
CLI11_EXTERNAL_OBJECTS =

external-libs/CLI11/libCLI11.a: external-libs/CLI11/CMakeFiles/CLI11.dir/build.make
external-libs/CLI11/libCLI11.a: external-libs/CLI11/CMakeFiles/CLI11.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library libCLI11.a"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/CLI11 && $(CMAKE_COMMAND) -P CMakeFiles/CLI11.dir/cmake_clean_target.cmake
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/CLI11 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CLI11.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external-libs/CLI11/CMakeFiles/CLI11.dir/build: external-libs/CLI11/libCLI11.a
.PHONY : external-libs/CLI11/CMakeFiles/CLI11.dir/build

external-libs/CLI11/CMakeFiles/CLI11.dir/clean:
	cd /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/CLI11 && $(CMAKE_COMMAND) -P CMakeFiles/CLI11.dir/cmake_clean.cmake
.PHONY : external-libs/CLI11/CMakeFiles/CLI11.dir/clean

external-libs/CLI11/CMakeFiles/CLI11.dir/depend:
	cd /home/apoorva64/SecTrans2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apoorva64/SecTrans2 /home/apoorva64/SecTrans2/external-libs/CLI11 /home/apoorva64/SecTrans2/cmake-build-debug /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/CLI11 /home/apoorva64/SecTrans2/cmake-build-debug/external-libs/CLI11/CMakeFiles/CLI11.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external-libs/CLI11/CMakeFiles/CLI11.dir/depend
