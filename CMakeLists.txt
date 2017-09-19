cmake_minimum_required(VERSION 2.8.3)
project(speed_node_cpp)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--no-as-needed")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_package(catkin REQUIRED COMPONENTS
	geometry_msgs
	roscpp
	rospy
	sensor_msgs
	std_msgs
	tf
	image_transport
        fla_utils
        fla_msgs
        cv_bridge
	)

catkin_package(
	)

find_package(mavlink)
find_package(Eigen3)

include_directories(
	${catkin_INCLUDE_DIRS}
	)

#set(DRAKE_INCLUDE_DIR "/home/drc/drake-distro/build/include")


add_executable(detection_image src/detection_image.cpp)
target_link_libraries(detection_image ${catkin_LIBRARIES})
