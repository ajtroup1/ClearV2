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
CMAKE_SOURCE_DIR = "/home/adamjtroup/shenanigans and hijinx/ClearV2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/adamjtroup/shenanigans and hijinx/ClearV2/build"

# Include any dependencies generated for this target.
include CMakeFiles/clear.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/clear.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clear.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clear.dir/flags.make

CMakeFiles/clear.dir/src/main.cpp.o: CMakeFiles/clear.dir/flags.make
CMakeFiles/clear.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/clear.dir/src/main.cpp.o: CMakeFiles/clear.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/adamjtroup/shenanigans and hijinx/ClearV2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clear.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/clear.dir/src/main.cpp.o -MF CMakeFiles/clear.dir/src/main.cpp.o.d -o CMakeFiles/clear.dir/src/main.cpp.o -c "/home/adamjtroup/shenanigans and hijinx/ClearV2/src/main.cpp"

CMakeFiles/clear.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clear.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/adamjtroup/shenanigans and hijinx/ClearV2/src/main.cpp" > CMakeFiles/clear.dir/src/main.cpp.i

CMakeFiles/clear.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clear.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/adamjtroup/shenanigans and hijinx/ClearV2/src/main.cpp" -o CMakeFiles/clear.dir/src/main.cpp.s

# Object files for target clear
clear_OBJECTS = \
"CMakeFiles/clear.dir/src/main.cpp.o"

# External object files for target clear
clear_EXTERNAL_OBJECTS =

clear: CMakeFiles/clear.dir/src/main.cpp.o
clear: CMakeFiles/clear.dir/build.make
clear: CMakeFiles/clear.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/adamjtroup/shenanigans and hijinx/ClearV2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable clear"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clear.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clear.dir/build: clear
.PHONY : CMakeFiles/clear.dir/build

CMakeFiles/clear.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clear.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clear.dir/clean

CMakeFiles/clear.dir/depend:
	cd "/home/adamjtroup/shenanigans and hijinx/ClearV2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/adamjtroup/shenanigans and hijinx/ClearV2" "/home/adamjtroup/shenanigans and hijinx/ClearV2" "/home/adamjtroup/shenanigans and hijinx/ClearV2/build" "/home/adamjtroup/shenanigans and hijinx/ClearV2/build" "/home/adamjtroup/shenanigans and hijinx/ClearV2/build/CMakeFiles/clear.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/clear.dir/depend

