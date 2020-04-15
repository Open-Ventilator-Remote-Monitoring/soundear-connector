# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/enkoder/git/soundear-connector/se3connector

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/enkoder/git/soundear-connector/se3connector

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/enkoder/git/soundear-connector/se3connector/CMakeFiles /home/enkoder/git/soundear-connector/se3connector/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/enkoder/git/soundear-connector/se3connector/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named se3connector

# Build rule for target.
se3connector: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 se3connector
.PHONY : se3connector

# fast build rule for target.
se3connector/fast:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/build
.PHONY : se3connector/fast

Helpers/helpers.o: Helpers/helpers.cpp.o

.PHONY : Helpers/helpers.o

# target to build an object file
Helpers/helpers.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/helpers.cpp.o
.PHONY : Helpers/helpers.cpp.o

Helpers/helpers.i: Helpers/helpers.cpp.i

.PHONY : Helpers/helpers.i

# target to preprocess a source file
Helpers/helpers.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/helpers.cpp.i
.PHONY : Helpers/helpers.cpp.i

Helpers/helpers.s: Helpers/helpers.cpp.s

.PHONY : Helpers/helpers.s

# target to generate assembly for a file
Helpers/helpers.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/helpers.cpp.s
.PHONY : Helpers/helpers.cpp.s

Helpers/leds.o: Helpers/leds.cpp.o

.PHONY : Helpers/leds.o

# target to build an object file
Helpers/leds.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leds.cpp.o
.PHONY : Helpers/leds.cpp.o

Helpers/leds.i: Helpers/leds.cpp.i

.PHONY : Helpers/leds.i

# target to preprocess a source file
Helpers/leds.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leds.cpp.i
.PHONY : Helpers/leds.cpp.i

Helpers/leds.s: Helpers/leds.cpp.s

.PHONY : Helpers/leds.s

# target to generate assembly for a file
Helpers/leds.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leds.cpp.s
.PHONY : Helpers/leds.cpp.s

Helpers/leq.o: Helpers/leq.cpp.o

.PHONY : Helpers/leq.o

# target to build an object file
Helpers/leq.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leq.cpp.o
.PHONY : Helpers/leq.cpp.o

Helpers/leq.i: Helpers/leq.cpp.i

.PHONY : Helpers/leq.i

# target to preprocess a source file
Helpers/leq.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leq.cpp.i
.PHONY : Helpers/leq.cpp.i

Helpers/leq.s: Helpers/leq.cpp.s

.PHONY : Helpers/leq.s

# target to generate assembly for a file
Helpers/leq.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Helpers/leq.cpp.s
.PHONY : Helpers/leq.cpp.s

Sockets/filesocket.o: Sockets/filesocket.cpp.o

.PHONY : Sockets/filesocket.o

# target to build an object file
Sockets/filesocket.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/filesocket.cpp.o
.PHONY : Sockets/filesocket.cpp.o

Sockets/filesocket.i: Sockets/filesocket.cpp.i

.PHONY : Sockets/filesocket.i

# target to preprocess a source file
Sockets/filesocket.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/filesocket.cpp.i
.PHONY : Sockets/filesocket.cpp.i

Sockets/filesocket.s: Sockets/filesocket.cpp.s

.PHONY : Sockets/filesocket.s

# target to generate assembly for a file
Sockets/filesocket.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/filesocket.cpp.s
.PHONY : Sockets/filesocket.cpp.s

Sockets/metersocket.o: Sockets/metersocket.cpp.o

.PHONY : Sockets/metersocket.o

# target to build an object file
Sockets/metersocket.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/metersocket.cpp.o
.PHONY : Sockets/metersocket.cpp.o

Sockets/metersocket.i: Sockets/metersocket.cpp.i

.PHONY : Sockets/metersocket.i

# target to preprocess a source file
Sockets/metersocket.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/metersocket.cpp.i
.PHONY : Sockets/metersocket.cpp.i

Sockets/metersocket.s: Sockets/metersocket.cpp.s

.PHONY : Sockets/metersocket.s

# target to generate assembly for a file
Sockets/metersocket.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/Sockets/metersocket.cpp.s
.PHONY : Sockets/metersocket.cpp.s

common.o: common.cpp.o

.PHONY : common.o

# target to build an object file
common.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/common.cpp.o
.PHONY : common.cpp.o

common.i: common.cpp.i

.PHONY : common.i

# target to preprocess a source file
common.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/common.cpp.i
.PHONY : common.cpp.i

common.s: common.cpp.s

.PHONY : common.s

# target to generate assembly for a file
common.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/common.cpp.s
.PHONY : common.cpp.s

hidapi/hid-libusb.o: hidapi/hid-libusb.c.o

.PHONY : hidapi/hid-libusb.o

# target to build an object file
hidapi/hid-libusb.c.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/hidapi/hid-libusb.c.o
.PHONY : hidapi/hid-libusb.c.o

hidapi/hid-libusb.i: hidapi/hid-libusb.c.i

.PHONY : hidapi/hid-libusb.i

# target to preprocess a source file
hidapi/hid-libusb.c.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/hidapi/hid-libusb.c.i
.PHONY : hidapi/hid-libusb.c.i

hidapi/hid-libusb.s: hidapi/hid-libusb.c.s

.PHONY : hidapi/hid-libusb.s

# target to generate assembly for a file
hidapi/hid-libusb.c.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/hidapi/hid-libusb.c.s
.PHONY : hidapi/hid-libusb.c.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/main.cpp.s
.PHONY : main.cpp.s

se3device.o: se3device.cpp.o

.PHONY : se3device.o

# target to build an object file
se3device.cpp.o:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/se3device.cpp.o
.PHONY : se3device.cpp.o

se3device.i: se3device.cpp.i

.PHONY : se3device.i

# target to preprocess a source file
se3device.cpp.i:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/se3device.cpp.i
.PHONY : se3device.cpp.i

se3device.s: se3device.cpp.s

.PHONY : se3device.s

# target to generate assembly for a file
se3device.cpp.s:
	$(MAKE) -f CMakeFiles/se3connector.dir/build.make CMakeFiles/se3connector.dir/se3device.cpp.s
.PHONY : se3device.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... se3connector"
	@echo "... Helpers/helpers.o"
	@echo "... Helpers/helpers.i"
	@echo "... Helpers/helpers.s"
	@echo "... Helpers/leds.o"
	@echo "... Helpers/leds.i"
	@echo "... Helpers/leds.s"
	@echo "... Helpers/leq.o"
	@echo "... Helpers/leq.i"
	@echo "... Helpers/leq.s"
	@echo "... Sockets/filesocket.o"
	@echo "... Sockets/filesocket.i"
	@echo "... Sockets/filesocket.s"
	@echo "... Sockets/metersocket.o"
	@echo "... Sockets/metersocket.i"
	@echo "... Sockets/metersocket.s"
	@echo "... common.o"
	@echo "... common.i"
	@echo "... common.s"
	@echo "... hidapi/hid-libusb.o"
	@echo "... hidapi/hid-libusb.i"
	@echo "... hidapi/hid-libusb.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... se3device.o"
	@echo "... se3device.i"
	@echo "... se3device.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
