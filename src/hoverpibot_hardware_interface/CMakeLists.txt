cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
project(hoverpibot_hardware_interface)

## Declare a catkin package
#hardware_interface
#control_msgs
#trajectory_msgs
#pluginlib
#control_toolbox
#joint_limits_interface

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
  sensor_msgs
  std_msgs
  roscpp
  hardware_interface
  controller_manager
)

find_package(Boost REQUIRED COMPONENTS
  system
  thread
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS
    sensor_msgs
    std_msgs
)

include_directories(
  include
    ${Boost_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
)

#add_executable(hoverpibotcpp src/hoverpibotcpp.cpp)
#target_link_libraries(hoverpibotcpp
#	${catkin_LIBRARIES})

add_library(hoverpibotcpp src/hoverpibotcpp.cpp)
target_link_libraries(hoverpibotcpp
	${catkin_LIBRARIES})

add_library(hoverpibot_hardware_interface src/hoverpibot_hardware_interface.cpp)
add_dependencies(hoverpibot_hardware_interface hoverpibotcpp)
target_link_libraries(hoverpibot_hardware_interface
	hoverpibotcpp
	${catkin_LIBRARIES})

add_executable(hoverpibot_hardware_interface_node src/hoverpibot_hardware_interface_node.cpp)
add_dependencies(hoverpibot_hardware_interface_node hoverpibot_hardware_interface)
target_link_libraries(hoverpibot_hardware_interface_node
	hoverpibot_hardware_interface
  hoverpibotcpp
	${catkin_LIBRARIES})


install(
  TARGETS
    ${PROJECT_NAME}_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
  DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
