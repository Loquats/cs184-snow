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
include glfw/examples/CMakeFiles/simple.dir/depend.make

# Include the progress variables for this target.
include glfw/examples/CMakeFiles/simple.dir/progress.make

# Include the compile flags for this target's objects.
include glfw/examples/CMakeFiles/simple.dir/flags.make

glfw/examples/simple.app/Contents/Resources/glfw.icns: ../glfw/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content glfw/examples/simple.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/examples/glfw.icns glfw/examples/simple.app/Contents/Resources/glfw.icns

glfw/examples/CMakeFiles/simple.dir/simple.c.o: glfw/examples/CMakeFiles/simple.dir/flags.make
glfw/examples/CMakeFiles/simple.dir/simple.c.o: ../glfw/examples/simple.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw/examples/CMakeFiles/simple.dir/simple.c.o"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/simple.c.o   -c /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/examples/simple.c

glfw/examples/CMakeFiles/simple.dir/simple.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/simple.c.i"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/examples/simple.c > CMakeFiles/simple.dir/simple.c.i

glfw/examples/CMakeFiles/simple.dir/simple.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/simple.c.s"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/examples/simple.c -o CMakeFiles/simple.dir/simple.c.s

glfw/examples/CMakeFiles/simple.dir/simple.c.o.requires:

.PHONY : glfw/examples/CMakeFiles/simple.dir/simple.c.o.requires

glfw/examples/CMakeFiles/simple.dir/simple.c.o.provides: glfw/examples/CMakeFiles/simple.dir/simple.c.o.requires
	$(MAKE) -f glfw/examples/CMakeFiles/simple.dir/build.make glfw/examples/CMakeFiles/simple.dir/simple.c.o.provides.build
.PHONY : glfw/examples/CMakeFiles/simple.dir/simple.c.o.provides

glfw/examples/CMakeFiles/simple.dir/simple.c.o.provides.build: glfw/examples/CMakeFiles/simple.dir/simple.c.o


glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o: glfw/examples/CMakeFiles/simple.dir/flags.make
glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o: ../glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/__/deps/glad.c.o   -c /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c

glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/__/deps/glad.c.i"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c > CMakeFiles/simple.dir/__/deps/glad.c.i

glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/__/deps/glad.c.s"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/deps/glad.c -o CMakeFiles/simple.dir/__/deps/glad.c.s

glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.requires:

.PHONY : glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.requires

glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.provides: glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.requires
	$(MAKE) -f glfw/examples/CMakeFiles/simple.dir/build.make glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.provides.build
.PHONY : glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.provides

glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.provides.build: glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o


# Object files for target simple
simple_OBJECTS = \
"CMakeFiles/simple.dir/simple.c.o" \
"CMakeFiles/simple.dir/__/deps/glad.c.o"

# External object files for target simple
simple_EXTERNAL_OBJECTS =

glfw/examples/simple.app/Contents/MacOS/simple: glfw/examples/CMakeFiles/simple.dir/simple.c.o
glfw/examples/simple.app/Contents/MacOS/simple: glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o
glfw/examples/simple.app/Contents/MacOS/simple: glfw/examples/CMakeFiles/simple.dir/build.make
glfw/examples/simple.app/Contents/MacOS/simple: glfw/src/libglfw3.a
glfw/examples/simple.app/Contents/MacOS/simple: glfw/examples/CMakeFiles/simple.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andy/Dropbox/repos/cs184/cs184-snow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable simple.app/Contents/MacOS/simple"
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw/examples/CMakeFiles/simple.dir/build: glfw/examples/simple.app/Contents/MacOS/simple
glfw/examples/CMakeFiles/simple.dir/build: glfw/examples/simple.app/Contents/Resources/glfw.icns

.PHONY : glfw/examples/CMakeFiles/simple.dir/build

glfw/examples/CMakeFiles/simple.dir/requires: glfw/examples/CMakeFiles/simple.dir/simple.c.o.requires
glfw/examples/CMakeFiles/simple.dir/requires: glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o.requires

.PHONY : glfw/examples/CMakeFiles/simple.dir/requires

glfw/examples/CMakeFiles/simple.dir/clean:
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples && $(CMAKE_COMMAND) -P CMakeFiles/simple.dir/cmake_clean.cmake
.PHONY : glfw/examples/CMakeFiles/simple.dir/clean

glfw/examples/CMakeFiles/simple.dir/depend:
	cd /Users/andy/Dropbox/repos/cs184/cs184-snow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andy/Dropbox/repos/cs184/cs184-snow /Users/andy/Dropbox/repos/cs184/cs184-snow/glfw/examples /Users/andy/Dropbox/repos/cs184/cs184-snow/build /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples /Users/andy/Dropbox/repos/cs184/cs184-snow/build/glfw/examples/CMakeFiles/simple.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw/examples/CMakeFiles/simple.dir/depend

