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
CMAKE_SOURCE_DIR = /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build

# Include any dependencies generated for this target.
include CMakeFiles/object_detection_yolo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/object_detection_yolo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/object_detection_yolo.dir/flags.make

CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o: CMakeFiles/object_detection_yolo.dir/flags.make
CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o: ../object_detection_yolo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o -c /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/object_detection_yolo.cpp

CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/object_detection_yolo.cpp > CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.i

CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/object_detection_yolo.cpp -o CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.s

# Object files for target object_detection_yolo
object_detection_yolo_OBJECTS = \
"CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o"

# External object files for target object_detection_yolo
object_detection_yolo_EXTERNAL_OBJECTS =

object_detection_yolo: CMakeFiles/object_detection_yolo.dir/object_detection_yolo.cpp.o
object_detection_yolo: CMakeFiles/object_detection_yolo.dir/build.make
object_detection_yolo: /usr/local/lib/libopencv_gapi.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_highgui.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_ml.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_objdetect.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_photo.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_stitching.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_video.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_videoio.so.4.5.1
object_detection_yolo: /usr/lib/x86_64-linux-gnu/libcurses.so
object_detection_yolo: /usr/lib/x86_64-linux-gnu/libform.so
object_detection_yolo: libserial.a
object_detection_yolo: /usr/local/lib/libopencv_dnn.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_imgcodecs.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_calib3d.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_features2d.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_flann.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_imgproc.so.4.5.1
object_detection_yolo: /usr/local/lib/libopencv_core.so.4.5.1
object_detection_yolo: CMakeFiles/object_detection_yolo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable object_detection_yolo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/object_detection_yolo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/object_detection_yolo.dir/build: object_detection_yolo

.PHONY : CMakeFiles/object_detection_yolo.dir/build

CMakeFiles/object_detection_yolo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/object_detection_yolo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/object_detection_yolo.dir/clean

CMakeFiles/object_detection_yolo.dir/depend:
	cd /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build /home/r2d2/R2D2-Main/R2D2/RaspBerry/camera-nocommunication/build/CMakeFiles/object_detection_yolo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/object_detection_yolo.dir/depend

