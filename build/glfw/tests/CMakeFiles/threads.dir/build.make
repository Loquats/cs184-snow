# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.6.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.6.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/andy/Dropbox/repos/cs184/cs184-snow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/andy/Dropbox/repos/cs184/cs184-snow/build

# Include any dependencies generated for this target.
include glfw/tests/CMakeFiles/threads.dir/depend.make

# Include the progress variables for this target.
include glfw/tests/CMakeFiles/threads.dir/progress.make

# Include the compile flags for this target's objects.
include glfw/tests/CMakeFiles/threads.dir/flags.make

glfw/tests/CMakeFiles/threads.dir/threads.c.o: glfw/tests/CMakeFiles/threads.dir/flags.make
glfw/tests/CMakeFiles/threads.dir/threads.c.o: ../glfw/tests/threads.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw/tests/CMakeFiles/threads.dir/threads.c.o"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/threads.c.o   -c /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/tests/threads.c

glfw/tests/CMakeFiles/threads.dir/threads.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/threads.c.i"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/tests/threads.c > CMakeFiles/threads.dir/threads.c.i

glfw/tests/CMakeFiles/threads.dir/threads.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/threads.c.s"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/tests/threads.c -o CMakeFiles/threads.dir/threads.c.s

glfw/tests/CMakeFiles/threads.dir/threads.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/threads.dir/threads.c.o.requires

glfw/tests/CMakeFiles/threads.dir/threads.c.o.provides: glfw/tests/CMakeFiles/threads.dir/threads.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/threads.dir/build.make glfw/tests/CMakeFiles/threads.dir/threads.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/threads.dir/threads.c.o.provides

glfw/tests/CMakeFiles/threads.dir/threads.c.o.provides.build: glfw/tests/CMakeFiles/threads.dir/threads.c.o


glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o: glfw/tests/CMakeFiles/threads.dir/flags.make
glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o: ../glfw/deps/tinycthread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/__/deps/tinycthread.c.o   -c /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/tinycthread.c

glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/__/deps/tinycthread.c.i"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/tinycthread.c > CMakeFiles/threads.dir/__/deps/tinycthread.c.i

glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/__/deps/tinycthread.c.s"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/tinycthread.c -o CMakeFiles/threads.dir/__/deps/tinycthread.c.s

glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires

glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides: glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/threads.dir/build.make glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides

glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides.build: glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o


glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o: glfw/tests/CMakeFiles/threads.dir/flags.make
glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o: ../glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/__/deps/glad.c.o   -c /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c

glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/__/deps/glad.c.i"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c > CMakeFiles/threads.dir/__/deps/glad.c.i

glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/__/deps/glad.c.s"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c -o CMakeFiles/threads.dir/__/deps/glad.c.s

glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires:

.PHONY : glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires

glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides: glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires
	$(MAKE) -f glfw/tests/CMakeFiles/threads.dir/build.make glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides.build
.PHONY : glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides

glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides.build: glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o


# Object files for target threads
threads_OBJECTS = \
"CMakeFiles/threads.dir/threads.c.o" \
"CMakeFiles/threads.dir/__/deps/tinycthread.c.o" \
"CMakeFiles/threads.dir/__/deps/glad.c.o"

# External object files for target threads
threads_EXTERNAL_OBJECTS =

glfw/tests/threads.app/Contents/MacOS/threads: glfw/tests/CMakeFiles/threads.dir/threads.c.o
glfw/tests/threads.app/Contents/MacOS/threads: glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o
glfw/tests/threads.app/Contents/MacOS/threads: glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o
glfw/tests/threads.app/Contents/MacOS/threads: glfw/tests/CMakeFiles/threads.dir/build.make
glfw/tests/threads.app/Contents/MacOS/threads: glfw/src/libglfw3.a
glfw/tests/threads.app/Contents/MacOS/threads: glfw/tests/CMakeFiles/threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable threads.app/Contents/MacOS/threads"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw/tests/CMakeFiles/threads.dir/build: glfw/tests/threads.app/Contents/MacOS/threads

.PHONY : glfw/tests/CMakeFiles/threads.dir/build

glfw/tests/CMakeFiles/threads.dir/requires: glfw/tests/CMakeFiles/threads.dir/threads.c.o.requires
glfw/tests/CMakeFiles/threads.dir/requires: glfw/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires
glfw/tests/CMakeFiles/threads.dir/requires: glfw/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires

.PHONY : glfw/tests/CMakeFiles/threads.dir/requires

glfw/tests/CMakeFiles/threads.dir/clean:
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/threads.dir/cmake_clean.cmake
.PHONY : glfw/tests/CMakeFiles/threads.dir/clean

glfw/tests/CMakeFiles/threads.dir/depend:
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andy/Dropbox/repos/cs184/cs184-snow /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/tests /Users/andy/Dropbox/repos/cs184/cs184-snow/build /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/tests/CMakeFiles/threads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw/tests/CMakeFiles/threads.dir/depend

