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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.1/bin/cmake -E remove -f

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

CMakeFiles/c4.dir/src/InputReader.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/InputReader.cpp.o: ../src/InputReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/c4.dir/src/InputReader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/InputReader.cpp.o -c /Users/chris/CLionProjects/c4/src/InputReader.cpp

CMakeFiles/c4.dir/src/InputReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/InputReader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/InputReader.cpp > CMakeFiles/c4.dir/src/InputReader.cpp.i

CMakeFiles/c4.dir/src/InputReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/InputReader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/InputReader.cpp -o CMakeFiles/c4.dir/src/InputReader.cpp.s

CMakeFiles/c4.dir/src/Lexer.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/Lexer.cpp.o: ../src/Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/c4.dir/src/Lexer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/Lexer.cpp.o -c /Users/chris/CLionProjects/c4/src/Lexer.cpp

CMakeFiles/c4.dir/src/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/Lexer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/Lexer.cpp > CMakeFiles/c4.dir/src/Lexer.cpp.i

CMakeFiles/c4.dir/src/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/Lexer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/Lexer.cpp -o CMakeFiles/c4.dir/src/Lexer.cpp.s

CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o: ../src/StateMachineBuilder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o -c /Users/chris/CLionProjects/c4/src/StateMachineBuilder.cpp

CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/StateMachineBuilder.cpp > CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.i

CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/StateMachineBuilder.cpp -o CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.s

CMakeFiles/c4.dir/src/StreamInputReader.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/StreamInputReader.cpp.o: ../src/StreamInputReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/c4.dir/src/StreamInputReader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/StreamInputReader.cpp.o -c /Users/chris/CLionProjects/c4/src/StreamInputReader.cpp

CMakeFiles/c4.dir/src/StreamInputReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/StreamInputReader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/StreamInputReader.cpp > CMakeFiles/c4.dir/src/StreamInputReader.cpp.i

CMakeFiles/c4.dir/src/StreamInputReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/StreamInputReader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/StreamInputReader.cpp -o CMakeFiles/c4.dir/src/StreamInputReader.cpp.s

CMakeFiles/c4.dir/src/StringInputReader.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/StringInputReader.cpp.o: ../src/StringInputReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/c4.dir/src/StringInputReader.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/StringInputReader.cpp.o -c /Users/chris/CLionProjects/c4/src/StringInputReader.cpp

CMakeFiles/c4.dir/src/StringInputReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/StringInputReader.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/StringInputReader.cpp > CMakeFiles/c4.dir/src/StringInputReader.cpp.i

CMakeFiles/c4.dir/src/StringInputReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/StringInputReader.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/StringInputReader.cpp -o CMakeFiles/c4.dir/src/StringInputReader.cpp.s

CMakeFiles/c4.dir/src/Token.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/Token.cpp.o: ../src/Token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/c4.dir/src/Token.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/Token.cpp.o -c /Users/chris/CLionProjects/c4/src/Token.cpp

CMakeFiles/c4.dir/src/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/Token.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/Token.cpp > CMakeFiles/c4.dir/src/Token.cpp.i

CMakeFiles/c4.dir/src/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/Token.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/Token.cpp -o CMakeFiles/c4.dir/src/Token.cpp.s

CMakeFiles/c4.dir/src/TreeNode.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/TreeNode.cpp.o: ../src/TreeNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/c4.dir/src/TreeNode.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/TreeNode.cpp.o -c /Users/chris/CLionProjects/c4/src/TreeNode.cpp

CMakeFiles/c4.dir/src/TreeNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/TreeNode.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/TreeNode.cpp > CMakeFiles/c4.dir/src/TreeNode.cpp.i

CMakeFiles/c4.dir/src/TreeNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/TreeNode.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/TreeNode.cpp -o CMakeFiles/c4.dir/src/TreeNode.cpp.s

CMakeFiles/c4.dir/src/main.cpp.o: CMakeFiles/c4.dir/flags.make
CMakeFiles/c4.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/c4.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c4.dir/src/main.cpp.o -c /Users/chris/CLionProjects/c4/src/main.cpp

CMakeFiles/c4.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c4.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chris/CLionProjects/c4/src/main.cpp > CMakeFiles/c4.dir/src/main.cpp.i

CMakeFiles/c4.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c4.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chris/CLionProjects/c4/src/main.cpp -o CMakeFiles/c4.dir/src/main.cpp.s

# Object files for target c4
c4_OBJECTS = \
"CMakeFiles/c4.dir/src/InputReader.cpp.o" \
"CMakeFiles/c4.dir/src/Lexer.cpp.o" \
"CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o" \
"CMakeFiles/c4.dir/src/StreamInputReader.cpp.o" \
"CMakeFiles/c4.dir/src/StringInputReader.cpp.o" \
"CMakeFiles/c4.dir/src/Token.cpp.o" \
"CMakeFiles/c4.dir/src/TreeNode.cpp.o" \
"CMakeFiles/c4.dir/src/main.cpp.o"

# External object files for target c4
c4_EXTERNAL_OBJECTS =

c4: CMakeFiles/c4.dir/src/InputReader.cpp.o
c4: CMakeFiles/c4.dir/src/Lexer.cpp.o
c4: CMakeFiles/c4.dir/src/StateMachineBuilder.cpp.o
c4: CMakeFiles/c4.dir/src/StreamInputReader.cpp.o
c4: CMakeFiles/c4.dir/src/StringInputReader.cpp.o
c4: CMakeFiles/c4.dir/src/Token.cpp.o
c4: CMakeFiles/c4.dir/src/TreeNode.cpp.o
c4: CMakeFiles/c4.dir/src/main.cpp.o
c4: CMakeFiles/c4.dir/build.make
c4: CMakeFiles/c4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chris/CLionProjects/c4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable c4"
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

