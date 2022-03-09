cmake_minimum_required(VERSION 2.8.3)
project(ecn_sonar)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  nav_msgs
  roscpp
  sensor_msgs
)

catkin_package()

#External package includes
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(waypoint src/waypoint_sonar.cpp)
add_executable(display src/display_node.cpp)
add_executable(mapping src/mapping_node_scaled.cpp)
add_executable(slam src/slam_node_2.cpp)

add_executable(display_block src/display_block_node.cpp)
add_executable(display_block_2 src/display_block_node_2.cpp)



## Specify libraries to link a library or executable target against
target_link_libraries(waypoint ${catkin_LIBRARIES} )
target_link_libraries(display ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(mapping ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(slam ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(display_block ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(display_block_2 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

