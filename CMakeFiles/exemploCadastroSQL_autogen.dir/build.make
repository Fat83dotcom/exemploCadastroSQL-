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
CMAKE_SOURCE_DIR = /home/fernando/exemploCadastroSQL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fernando/exemploCadastroSQL

# Utility rule file for exemploCadastroSQL_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/exemploCadastroSQL_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/exemploCadastroSQL_autogen.dir/progress.make

CMakeFiles/exemploCadastroSQL_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/fernando/exemploCadastroSQL/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target exemploCadastroSQL"
	/usr/bin/cmake -E cmake_autogen /home/fernando/exemploCadastroSQL/CMakeFiles/exemploCadastroSQL_autogen.dir/AutogenInfo.json ""

exemploCadastroSQL_autogen: CMakeFiles/exemploCadastroSQL_autogen
exemploCadastroSQL_autogen: CMakeFiles/exemploCadastroSQL_autogen.dir/build.make
.PHONY : exemploCadastroSQL_autogen

# Rule to build all files generated by this target.
CMakeFiles/exemploCadastroSQL_autogen.dir/build: exemploCadastroSQL_autogen
.PHONY : CMakeFiles/exemploCadastroSQL_autogen.dir/build

CMakeFiles/exemploCadastroSQL_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exemploCadastroSQL_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exemploCadastroSQL_autogen.dir/clean

CMakeFiles/exemploCadastroSQL_autogen.dir/depend:
	cd /home/fernando/exemploCadastroSQL && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fernando/exemploCadastroSQL /home/fernando/exemploCadastroSQL /home/fernando/exemploCadastroSQL /home/fernando/exemploCadastroSQL /home/fernando/exemploCadastroSQL/CMakeFiles/exemploCadastroSQL_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/exemploCadastroSQL_autogen.dir/depend

