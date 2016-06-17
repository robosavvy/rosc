cmake_minimum_required(VERSION 2.8.3)
project(a_test_pkg)

find_package(
  catkin 
  REQUIRED 
  roscpp 
  rospy 
  std_msgs 
  message_generation
)

# Generate messages in the 'msg' folder
add_message_files(
 FILES
 abctest.msg
 gnampf.msg
 test12345.msg
)

# TODO: add srvs test
## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)
