# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chris/CLionProjects/c4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chris/CLionProjects/c4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/c4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/c4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/c4.dir/flags.make

CMakeFiles/c4.dir/src/main.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/c4.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/main.cpp.o -c /Users/chris/CLionProjects/c4/src/main.cpp

CMakeFiles/c4.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/main.cpp > CMakeFiles/c4.dir/src/main.cpp.i

CMakeFiles/c4.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/main.cpp -o CMakeFiles/c4.dir/src/main.cpp.s

# Object files for target c4
c4_OBJECTS = \
"CMakeFiles/c4.dir/src/main.cpp.o"

# External object files for target c4
c4_EXTERNAL_OBJECTS =

c4: CMakeFiles/c4.dir/src/main.cpp.o
c4: CMakeFiles/c4.dir/build.make
c4: CMakeFiles/c4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable c4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/c4.dir/build: c4

.PHONY : CMakeFiles/c4.dir/build

CMakeFiles/c4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/c4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/c4.dir/clean

CMakeFiles/c4.dir/depend:
	cd /Users/chris/CLionProjects/c4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chris/CLionProjects/c4 /Users/chris/CLionProjects/c4 /Users/chris/CLionProjects/c4/cmake-build-debug /Users/chris/CLionProjects/c4/cmake-build-debug /Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles/c4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/c4.dir/depend

