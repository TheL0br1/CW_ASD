# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2023.1.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2023.1.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\CLionProjects\CW_ASD

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CW_ASD.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CW_ASD.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CW_ASD.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CW_ASD.dir/flags.make

CMakeFiles/CW_ASD.dir/main.cpp.obj: CMakeFiles/CW_ASD.dir/flags.make
CMakeFiles/CW_ASD.dir/main.cpp.obj: CMakeFiles/CW_ASD.dir/includes_CXX.rsp
CMakeFiles/CW_ASD.dir/main.cpp.obj: C:/Users/User/CLionProjects/CW_ASD/main.cpp
CMakeFiles/CW_ASD.dir/main.cpp.obj: CMakeFiles/CW_ASD.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CW_ASD.dir/main.cpp.obj"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CW_ASD.dir/main.cpp.obj -MF CMakeFiles\CW_ASD.dir\main.cpp.obj.d -o CMakeFiles\CW_ASD.dir\main.cpp.obj -c C:\Users\User\CLionProjects\CW_ASD\main.cpp

CMakeFiles/CW_ASD.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CW_ASD.dir/main.cpp.i"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\User\CLionProjects\CW_ASD\main.cpp > CMakeFiles\CW_ASD.dir\main.cpp.i

CMakeFiles/CW_ASD.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CW_ASD.dir/main.cpp.s"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\User\CLionProjects\CW_ASD\main.cpp -o CMakeFiles\CW_ASD.dir\main.cpp.s

# Object files for target CW_ASD
CW_ASD_OBJECTS = \
"CMakeFiles/CW_ASD.dir/main.cpp.obj"

# External object files for target CW_ASD
CW_ASD_EXTERNAL_OBJECTS =

CW_ASD.exe: CMakeFiles/CW_ASD.dir/main.cpp.obj
CW_ASD.exe: CMakeFiles/CW_ASD.dir/build.make
CW_ASD.exe: C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML\ 2.5.1/lib/libsfml-graphics-s-d.a
CW_ASD.exe: C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML\ 2.5.1/lib/libsfml-window-s-d.a
CW_ASD.exe: C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML\ 2.5.1/lib/libsfml-system-s-d.a
CW_ASD.exe: C:/Users/User/Desktop/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML\ 2.5.1/lib/libfreetype.a
CW_ASD.exe: CMakeFiles/CW_ASD.dir/linkLibs.rsp
CW_ASD.exe: CMakeFiles/CW_ASD.dir/objects1
CW_ASD.exe: CMakeFiles/CW_ASD.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CW_ASD.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\CW_ASD.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CW_ASD.dir/build: CW_ASD.exe
.PHONY : CMakeFiles/CW_ASD.dir/build

CMakeFiles/CW_ASD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\CW_ASD.dir\cmake_clean.cmake
.PHONY : CMakeFiles/CW_ASD.dir/clean

CMakeFiles/CW_ASD.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\User\CLionProjects\CW_ASD C:\Users\User\CLionProjects\CW_ASD C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug C:\Users\User\CLionProjects\CW_ASD\cmake-build-debug\CMakeFiles\CW_ASD.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CW_ASD.dir/depend

