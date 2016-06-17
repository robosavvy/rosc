cmake_minimum_required(VERSION 2.8.3)
project(rosc_linux_test)

find_package(catkin REQUIRED rosc_core a_test_pkg rosc_sys_linux_static std_msgs message_generation)

## Generate messages in the 'msg' folder
add_message_files(
   FILES
   testbuiltin.msg
   testsubmessage.msg
   simple1.msg
   simple2.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

## Specify additional locations of header files
include_directories(include 
  ${catkin_INCLUDE_DIRS} 
  ${rosc_core_INCLUDE_DIRS}
  ${rosc_sys_linux_static_INCLUDE_DIRS}
  ${a_test_pkg_INCLUDE_DIRS}
)

## Declare a cpp executable
add_executable(rosc_linux_test_node src/main.c ${ROSC_EXAMPLE_MESSAGES_SRC})

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
add_dependencies(rosc_linux_test_node ${catkin_EXPORTED_TARGETS} rosc_sys_linux_static)

target_link_libraries(rosc_linux_test_node rosc_sys_linux_static)

# Why?
#~ execute_process(COMMAND clang ${ROSC_CORE_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/src/main.c -I/usr/include -I/usr/include/static_64-linux-gnu -I${rosc_core_INCLUDE_DIRS} -I${rosc_sys_linux_static_INCLUDE_DIRS})
