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
CMAKE_SOURCE_DIR = /home/jakob/Documents/gdv/gdv/GDV_Christmas

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jakob/Documents/gdv/gdv/GDV_Christmas/build

# Include any dependencies generated for this target.
include CMakeFiles/exercise00.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/exercise00.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/exercise00.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exercise00.dir/flags.make

CMakeFiles/exercise00.dir/src/main.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/exercise00.dir/src/main.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/exercise00.dir/src/main.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/main.cpp.o -MF CMakeFiles/exercise00.dir/src/main.cpp.o.d -o CMakeFiles/exercise00.dir/src/main.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/main.cpp

CMakeFiles/exercise00.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/main.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/main.cpp > CMakeFiles/exercise00.dir/src/main.cpp.i

CMakeFiles/exercise00.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/main.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/main.cpp -o CMakeFiles/exercise00.dir/src/main.cpp.s

CMakeFiles/exercise00.dir/src/mesh.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/mesh.cpp.o: ../src/mesh.cpp
CMakeFiles/exercise00.dir/src/mesh.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/exercise00.dir/src/mesh.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/mesh.cpp.o -MF CMakeFiles/exercise00.dir/src/mesh.cpp.o.d -o CMakeFiles/exercise00.dir/src/mesh.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/mesh.cpp

CMakeFiles/exercise00.dir/src/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/mesh.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/mesh.cpp > CMakeFiles/exercise00.dir/src/mesh.cpp.i

CMakeFiles/exercise00.dir/src/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/mesh.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/mesh.cpp -o CMakeFiles/exercise00.dir/src/mesh.cpp.s

CMakeFiles/exercise00.dir/src/intersection.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/intersection.cpp.o: ../src/intersection.cpp
CMakeFiles/exercise00.dir/src/intersection.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/exercise00.dir/src/intersection.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/intersection.cpp.o -MF CMakeFiles/exercise00.dir/src/intersection.cpp.o.d -o CMakeFiles/exercise00.dir/src/intersection.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/intersection.cpp

CMakeFiles/exercise00.dir/src/intersection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/intersection.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/intersection.cpp > CMakeFiles/exercise00.dir/src/intersection.cpp.i

CMakeFiles/exercise00.dir/src/intersection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/intersection.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/intersection.cpp -o CMakeFiles/exercise00.dir/src/intersection.cpp.s

CMakeFiles/exercise00.dir/src/bvh.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/bvh.cpp.o: ../src/bvh.cpp
CMakeFiles/exercise00.dir/src/bvh.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/exercise00.dir/src/bvh.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/bvh.cpp.o -MF CMakeFiles/exercise00.dir/src/bvh.cpp.o.d -o CMakeFiles/exercise00.dir/src/bvh.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/bvh.cpp

CMakeFiles/exercise00.dir/src/bvh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/bvh.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/bvh.cpp > CMakeFiles/exercise00.dir/src/bvh.cpp.i

CMakeFiles/exercise00.dir/src/bvh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/bvh.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/bvh.cpp -o CMakeFiles/exercise00.dir/src/bvh.cpp.s

CMakeFiles/exercise00.dir/src/raytracer.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/raytracer.cpp.o: ../src/raytracer.cpp
CMakeFiles/exercise00.dir/src/raytracer.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/exercise00.dir/src/raytracer.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/raytracer.cpp.o -MF CMakeFiles/exercise00.dir/src/raytracer.cpp.o.d -o CMakeFiles/exercise00.dir/src/raytracer.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/raytracer.cpp

CMakeFiles/exercise00.dir/src/raytracer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/raytracer.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/raytracer.cpp > CMakeFiles/exercise00.dir/src/raytracer.cpp.i

CMakeFiles/exercise00.dir/src/raytracer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/raytracer.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/raytracer.cpp -o CMakeFiles/exercise00.dir/src/raytracer.cpp.s

CMakeFiles/exercise00.dir/src/sampler.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/sampler.cpp.o: ../src/sampler.cpp
CMakeFiles/exercise00.dir/src/sampler.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/exercise00.dir/src/sampler.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/sampler.cpp.o -MF CMakeFiles/exercise00.dir/src/sampler.cpp.o.d -o CMakeFiles/exercise00.dir/src/sampler.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/sampler.cpp

CMakeFiles/exercise00.dir/src/sampler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/sampler.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/sampler.cpp > CMakeFiles/exercise00.dir/src/sampler.cpp.i

CMakeFiles/exercise00.dir/src/sampler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/sampler.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/sampler.cpp -o CMakeFiles/exercise00.dir/src/sampler.cpp.s

CMakeFiles/exercise00.dir/src/texture.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/texture.cpp.o: ../src/texture.cpp
CMakeFiles/exercise00.dir/src/texture.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/exercise00.dir/src/texture.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/texture.cpp.o -MF CMakeFiles/exercise00.dir/src/texture.cpp.o.d -o CMakeFiles/exercise00.dir/src/texture.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/texture.cpp

CMakeFiles/exercise00.dir/src/texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/texture.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/texture.cpp > CMakeFiles/exercise00.dir/src/texture.cpp.i

CMakeFiles/exercise00.dir/src/texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/texture.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/texture.cpp -o CMakeFiles/exercise00.dir/src/texture.cpp.s

CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o: ../src/gl/pbr_shader.cpp
CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o -MF CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o.d -o CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/pbr_shader.cpp

CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/pbr_shader.cpp > CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.i

CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/pbr_shader.cpp -o CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.s

CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o: ../src/gl/simple_shader.cpp
CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o -MF CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o.d -o CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/simple_shader.cpp

CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/simple_shader.cpp > CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.i

CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/simple_shader.cpp -o CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.s

CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o: ../src/gl/helper_shader.cpp
CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o -MF CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o.d -o CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/helper_shader.cpp

CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/helper_shader.cpp > CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.i

CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/helper_shader.cpp -o CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.s

CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o: ../src/gl/gl_utils.cpp
CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o -MF CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o.d -o CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_utils.cpp

CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_utils.cpp > CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.i

CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_utils.cpp -o CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.s

CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o: ../src/gl/gl_envmap.cpp
CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o -MF CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o.d -o CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_envmap.cpp

CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_envmap.cpp > CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.i

CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gl/gl_envmap.cpp -o CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.s

CMakeFiles/exercise00.dir/src/gui/gui.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gui/gui.cpp.o: ../src/gui/gui.cpp
CMakeFiles/exercise00.dir/src/gui/gui.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/exercise00.dir/src/gui/gui.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gui/gui.cpp.o -MF CMakeFiles/exercise00.dir/src/gui/gui.cpp.o.d -o CMakeFiles/exercise00.dir/src/gui/gui.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gui.cpp

CMakeFiles/exercise00.dir/src/gui/gui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gui/gui.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gui.cpp > CMakeFiles/exercise00.dir/src/gui/gui.cpp.i

CMakeFiles/exercise00.dir/src/gui/gui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gui/gui.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gui.cpp -o CMakeFiles/exercise00.dir/src/gui/gui.cpp.s

CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o: ../src/gui/camera_controls.cpp
CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o -MF CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o.d -o CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/camera_controls.cpp

CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/camera_controls.cpp > CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.i

CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/camera_controls.cpp -o CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.s

CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o: ../src/gui/gl_view.cpp
CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o -MF CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o.d -o CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gl_view.cpp

CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gl_view.cpp > CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.i

CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/gl_view.cpp -o CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.s

CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o: CMakeFiles/exercise00.dir/flags.make
CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o: ../src/gui/raytracer_view.cpp
CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o: CMakeFiles/exercise00.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o -MF CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o.d -o CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o -c /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/raytracer_view.cpp

CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/raytracer_view.cpp > CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.i

CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jakob/Documents/gdv/gdv/GDV_Christmas/src/gui/raytracer_view.cpp -o CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.s

# Object files for target exercise00
exercise00_OBJECTS = \
"CMakeFiles/exercise00.dir/src/main.cpp.o" \
"CMakeFiles/exercise00.dir/src/mesh.cpp.o" \
"CMakeFiles/exercise00.dir/src/intersection.cpp.o" \
"CMakeFiles/exercise00.dir/src/bvh.cpp.o" \
"CMakeFiles/exercise00.dir/src/raytracer.cpp.o" \
"CMakeFiles/exercise00.dir/src/sampler.cpp.o" \
"CMakeFiles/exercise00.dir/src/texture.cpp.o" \
"CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o" \
"CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o" \
"CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o" \
"CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o" \
"CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o" \
"CMakeFiles/exercise00.dir/src/gui/gui.cpp.o" \
"CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o" \
"CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o" \
"CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o"

# External object files for target exercise00
exercise00_EXTERNAL_OBJECTS =

exercise00: CMakeFiles/exercise00.dir/src/main.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/mesh.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/intersection.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/bvh.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/raytracer.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/sampler.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/texture.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gl/pbr_shader.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gl/simple_shader.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gl/helper_shader.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gl/gl_utils.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gl/gl_envmap.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gui/gui.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gui/camera_controls.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gui/gl_view.cpp.o
exercise00: CMakeFiles/exercise00.dir/src/gui/raytracer_view.cpp.o
exercise00: CMakeFiles/exercise00.dir/build.make
exercise00: ext/nanogui/libnanogui.a
exercise00: CMakeFiles/exercise00.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable exercise00"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exercise00.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exercise00.dir/build: exercise00
.PHONY : CMakeFiles/exercise00.dir/build

CMakeFiles/exercise00.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exercise00.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exercise00.dir/clean

CMakeFiles/exercise00.dir/depend:
	cd /home/jakob/Documents/gdv/gdv/GDV_Christmas/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jakob/Documents/gdv/gdv/GDV_Christmas /home/jakob/Documents/gdv/gdv/GDV_Christmas /home/jakob/Documents/gdv/gdv/GDV_Christmas/build /home/jakob/Documents/gdv/gdv/GDV_Christmas/build /home/jakob/Documents/gdv/gdv/GDV_Christmas/build/CMakeFiles/exercise00.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/exercise00.dir/depend

