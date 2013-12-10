cmake_minimum_required(VERSION 2.8.3)
project(rosc_linux_test)

find_package(catkin REQUIRED rosc_core a_test_pkg rosc_sys_linux_static std_msgs message_generation)

add_message_files( #If you have your own message files ...
   FILES
   testbuiltin.msg
   testsubmessage.msg
   simple1.msg
   simple2.msg
)

generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(include 
                     ${catkin_INCLUDE_DIRS} #Catkin stuff
                     ${rosc_core_INCLUDE_DIRS} #rosc core sources
                     ${rosc_sys_linux_static_INCLUDE_DIRS} #Link the platform package
                     ${a_test_pkg_INCLUDE_DIRS}) #Other packages ...


add_executable(rosc_linux_test_node src/main.c) #Define your executable
add_dependencies(rosc_linux_test_node rosc_sys_linux_static a_test_pkg)#Add dependencies

target_link_libraries(rosc_linux_test_node
    rosc_sys_linux_static #Add static platform library
)
