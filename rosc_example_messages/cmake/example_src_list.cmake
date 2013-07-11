SET(ROSC_EXAMPLE_MESSAGES_SRC
src/rosc/std_msgs/int8multiarray.c
src/rosc/std_msgs/multiarraydimension.c
src/rosc/std_msgs/multiarraylayout.c
)

SET(ROSC_EXAMPLE_MESSAGES_DIR ${CMAKE_CURRENT_LIST_DIR}/..) #get the current file directory


SET(ROSC_EXAMPLE_MESSAGES_SRC_TMP "") #Create empty temporary list

#Insert directory to all entries
foreach(entry ${ROSC_EXAMPLE_MESSAGES_SRC})
  list(APPEND ROSC_EXAMPLE_MESSAGES_SRC_TMP ${ROSC_EXAMPLE_MESSAGES_DIR}/${entry})
endforeach(entry ${ROSC_EXAMPLE_MESSAGES_SRC})

#Add the directory to the sources
SET(ROSC_EXAMPLE_MESSAGES_SRC ${ROSC_EXAMPLE_MESSAGES_SRC_TMP})
#message(STATUS ${ROSC_EXAMPLE_MESSAGES_SRC})

#Remove temporary list
UNSET(ROSC_EXAMPLE_MESSAGES_SRC_TMP)