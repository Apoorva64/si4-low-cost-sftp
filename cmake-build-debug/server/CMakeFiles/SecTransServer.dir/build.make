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
include server/CMakeFiles/SecTransServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include server/CMakeFiles/SecTransServer.dir/compiler_depend.make

# Include the progress variables for this target.
include server/CMakeFiles/SecTransServer.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/SecTransServer.dir/flags.make

server/CMakeFiles/SecTransServer.dir/main.cpp.o: server/CMakeFiles/SecTransServer.dir/flags.make
server/CMakeFiles/SecTransServer.dir/main.cpp.o: ../server/main.cpp
server/CMakeFiles/SecTransServer.dir/main.cpp.o: server/CMakeFiles/SecTransServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/SecTransServer.dir/main.cpp.o"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/SecTransServer.dir/main.cpp.o -MF CMakeFiles/SecTransServer.dir/main.cpp.o.d -o CMakeFiles/SecTransServer.dir/main.cpp.o -c /home/apoorva64/SecTrans2/server/main.cpp

server/CMakeFiles/SecTransServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SecTransServer.dir/main.cpp.i"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/apoorva64/SecTrans2/server/main.cpp > CMakeFiles/SecTransServer.dir/main.cpp.i

server/CMakeFiles/SecTransServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SecTransServer.dir/main.cpp.s"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/apoorva64/SecTrans2/server/main.cpp -o CMakeFiles/SecTransServer.dir/main.cpp.s

server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o: server/CMakeFiles/SecTransServer.dir/flags.make
server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o: ../server/server/Server.cpp
server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o: server/CMakeFiles/SecTransServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o -MF CMakeFiles/SecTransServer.dir/server/Server.cpp.o.d -o CMakeFiles/SecTransServer.dir/server/Server.cpp.o -c /home/apoorva64/SecTrans2/server/server/Server.cpp

server/CMakeFiles/SecTransServer.dir/server/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SecTransServer.dir/server/Server.cpp.i"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/apoorva64/SecTrans2/server/server/Server.cpp > CMakeFiles/SecTransServer.dir/server/Server.cpp.i

server/CMakeFiles/SecTransServer.dir/server/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SecTransServer.dir/server/Server.cpp.s"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/apoorva64/SecTrans2/server/server/Server.cpp -o CMakeFiles/SecTransServer.dir/server/Server.cpp.s

# Object files for target SecTransServer
SecTransServer_OBJECTS = \
"CMakeFiles/SecTransServer.dir/main.cpp.o" \
"CMakeFiles/SecTransServer.dir/server/Server.cpp.o"

# External object files for target SecTransServer
SecTransServer_EXTERNAL_OBJECTS =

server/SecTransServer: server/CMakeFiles/SecTransServer.dir/main.cpp.o
server/SecTransServer: server/CMakeFiles/SecTransServer.dir/server/Server.cpp.o
server/SecTransServer: server/CMakeFiles/SecTransServer.dir/build.make
server/SecTransServer: external-libs/libclient/liblibclient.a
server/SecTransServer: external-libs/libserver/liblibserver.a
server/SecTransServer: external-libs/CLI11/libCLI11.a
server/SecTransServer: external-libs/ThreadPool/libThreadPool.a
server/SecTransServer: external-libs/OpenSSL/libOpenSSL.a
server/SecTransServer: common/libcommon.a
server/SecTransServer: external-libs/spdlog/libspdlog.a
server/SecTransServer: external-libs/curlcpp/src/libcurlcpp.a
server/SecTransServer: external-libs/libclient/liblibclient.a
server/SecTransServer: ../external-libs/libclient/libclient.so
server/SecTransServer: external-libs/libserver/liblibserver.a
server/SecTransServer: ../external-libs/libserver/libserver.so
server/SecTransServer: external-libs/OpenSSL/libOpenSSL.a
server/SecTransServer: /usr/lib/x86_64-linux-gnu/libssl.so
server/SecTransServer: /usr/lib/x86_64-linux-gnu/libcrypto.so
server/SecTransServer: /usr/lib/x86_64-linux-gnu/libcurl.so
server/SecTransServer: /usr/lib/x86_64-linux-gnu/libssl.so
server/SecTransServer: /usr/lib/x86_64-linux-gnu/libcrypto.so
server/SecTransServer: server/CMakeFiles/SecTransServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/apoorva64/SecTrans2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable SecTransServer"
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SecTransServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/SecTransServer.dir/build: server/SecTransServer
.PHONY : server/CMakeFiles/SecTransServer.dir/build

server/CMakeFiles/SecTransServer.dir/clean:
	cd /home/apoorva64/SecTrans2/cmake-build-debug/server && $(CMAKE_COMMAND) -P CMakeFiles/SecTransServer.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/SecTransServer.dir/clean

server/CMakeFiles/SecTransServer.dir/depend:
	cd /home/apoorva64/SecTrans2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apoorva64/SecTrans2 /home/apoorva64/SecTrans2/server /home/apoorva64/SecTrans2/cmake-build-debug /home/apoorva64/SecTrans2/cmake-build-debug/server /home/apoorva64/SecTrans2/cmake-build-debug/server/CMakeFiles/SecTransServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/SecTransServer.dir/depend

