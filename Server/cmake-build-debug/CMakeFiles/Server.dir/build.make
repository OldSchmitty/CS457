# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /snap/clion/44/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/44/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marks/School/CS457/Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marks/School/CS457/Server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Server.dir/flags.make

CMakeFiles/Server.dir/driver.cpp.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/driver.cpp.o: ../driver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marks/School/CS457/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Server.dir/driver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Server.dir/driver.cpp.o -c /home/marks/School/CS457/Server/driver.cpp

CMakeFiles/Server.dir/driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Server.dir/driver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marks/School/CS457/Server/driver.cpp > CMakeFiles/Server.dir/driver.cpp.i

CMakeFiles/Server.dir/driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Server.dir/driver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marks/School/CS457/Server/driver.cpp -o CMakeFiles/Server.dir/driver.cpp.s

CMakeFiles/Server.dir/tcpUserSocket.cpp.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/tcpUserSocket.cpp.o: ../tcpUserSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marks/School/CS457/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Server.dir/tcpUserSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Server.dir/tcpUserSocket.cpp.o -c /home/marks/School/CS457/Server/tcpUserSocket.cpp

CMakeFiles/Server.dir/tcpUserSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Server.dir/tcpUserSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marks/School/CS457/Server/tcpUserSocket.cpp > CMakeFiles/Server.dir/tcpUserSocket.cpp.i

CMakeFiles/Server.dir/tcpUserSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Server.dir/tcpUserSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marks/School/CS457/Server/tcpUserSocket.cpp -o CMakeFiles/Server.dir/tcpUserSocket.cpp.s

CMakeFiles/Server.dir/tcpServerSocket.cpp.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/tcpServerSocket.cpp.o: ../tcpServerSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marks/School/CS457/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Server.dir/tcpServerSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Server.dir/tcpServerSocket.cpp.o -c /home/marks/School/CS457/Server/tcpServerSocket.cpp

CMakeFiles/Server.dir/tcpServerSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Server.dir/tcpServerSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marks/School/CS457/Server/tcpServerSocket.cpp > CMakeFiles/Server.dir/tcpServerSocket.cpp.i

CMakeFiles/Server.dir/tcpServerSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Server.dir/tcpServerSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marks/School/CS457/Server/tcpServerSocket.cpp -o CMakeFiles/Server.dir/tcpServerSocket.cpp.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/driver.cpp.o" \
"CMakeFiles/Server.dir/tcpUserSocket.cpp.o" \
"CMakeFiles/Server.dir/tcpServerSocket.cpp.o"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

Server: CMakeFiles/Server.dir/driver.cpp.o
Server: CMakeFiles/Server.dir/tcpUserSocket.cpp.o
Server: CMakeFiles/Server.dir/tcpServerSocket.cpp.o
Server: CMakeFiles/Server.dir/build.make
Server: CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marks/School/CS457/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Server.dir/build: Server

.PHONY : CMakeFiles/Server.dir/build

CMakeFiles/Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Server.dir/clean

CMakeFiles/Server.dir/depend:
	cd /home/marks/School/CS457/Server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marks/School/CS457/Server /home/marks/School/CS457/Server /home/marks/School/CS457/Server/cmake-build-debug /home/marks/School/CS457/Server/cmake-build-debug /home/marks/School/CS457/Server/cmake-build-debug/CMakeFiles/Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Server.dir/depend

