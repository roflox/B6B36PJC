# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/c/ClionProjects/B6B36PJC/INIT_B

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tests.dir/flags.make

CMakeFiles/tests.dir/factorial.cpp.o: CMakeFiles/tests.dir/flags.make
CMakeFiles/tests.dir/factorial.cpp.o: ../factorial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tests.dir/factorial.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/factorial.cpp.o -c /mnt/c/ClionProjects/B6B36PJC/INIT_B/factorial.cpp

CMakeFiles/tests.dir/factorial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/factorial.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/ClionProjects/B6B36PJC/INIT_B/factorial.cpp > CMakeFiles/tests.dir/factorial.cpp.i

CMakeFiles/tests.dir/factorial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/factorial.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/ClionProjects/B6B36PJC/INIT_B/factorial.cpp -o CMakeFiles/tests.dir/factorial.cpp.s

CMakeFiles/tests.dir/tests-factorial.cpp.o: CMakeFiles/tests.dir/flags.make
CMakeFiles/tests.dir/tests-factorial.cpp.o: ../tests-factorial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tests.dir/tests-factorial.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/tests-factorial.cpp.o -c /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-factorial.cpp

CMakeFiles/tests.dir/tests-factorial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/tests-factorial.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-factorial.cpp > CMakeFiles/tests.dir/tests-factorial.cpp.i

CMakeFiles/tests.dir/tests-factorial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/tests-factorial.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-factorial.cpp -o CMakeFiles/tests.dir/tests-factorial.cpp.s

CMakeFiles/tests.dir/tests-main.cpp.o: CMakeFiles/tests.dir/flags.make
CMakeFiles/tests.dir/tests-main.cpp.o: ../tests-main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/tests.dir/tests-main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/tests-main.cpp.o -c /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-main.cpp

CMakeFiles/tests.dir/tests-main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/tests-main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-main.cpp > CMakeFiles/tests.dir/tests-main.cpp.i

CMakeFiles/tests.dir/tests-main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/tests-main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/ClionProjects/B6B36PJC/INIT_B/tests-main.cpp -o CMakeFiles/tests.dir/tests-main.cpp.s

# Object files for target tests
tests_OBJECTS = \
"CMakeFiles/tests.dir/factorial.cpp.o" \
"CMakeFiles/tests.dir/tests-factorial.cpp.o" \
"CMakeFiles/tests.dir/tests-main.cpp.o"

# External object files for target tests
tests_EXTERNAL_OBJECTS =

tests: CMakeFiles/tests.dir/factorial.cpp.o
tests: CMakeFiles/tests.dir/tests-factorial.cpp.o
tests: CMakeFiles/tests.dir/tests-main.cpp.o
tests: CMakeFiles/tests.dir/build.make
tests: CMakeFiles/tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tests.dir/build: tests

.PHONY : CMakeFiles/tests.dir/build

CMakeFiles/tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tests.dir/clean

CMakeFiles/tests.dir/depend:
	cd /mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/ClionProjects/B6B36PJC/INIT_B /mnt/c/ClionProjects/B6B36PJC/INIT_B /mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug /mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug /mnt/c/ClionProjects/B6B36PJC/INIT_B/cmake-build-debug/CMakeFiles/tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tests.dir/depend

