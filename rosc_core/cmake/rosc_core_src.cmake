message(STATUS "Getting rosc sources...")

#update the current sources list in the next include
execute_process(COMMAND python ${CMAKE_CURRENT_LIST_DIR}/../code_gen/refreshFiles.py)

#all sources of rosc_core
include(${CMAKE_CURRENT_LIST_DIR}/rosc_core_src_list.cmake)

SET(ROSC_CORE_DIR ${CMAKE_CURRENT_LIST_DIR}/..) #get the current file directory
#message(STATUS "rosc_core directory is:" ${ROSC_CORE_DIR})

SET(ROSC_CORE_SOURCES_TMP "") #Create empty temporary list

#Insert directory to all entries
foreach(entry ${ROSC_CORE_SOURCES})
  list(APPEND ROSC_CORE_SOURCES_TMP ${ROSC_CORE_DIR}/${entry})
endforeach(entry ${ROSC_CORE_SOURCES})

#Add the directory to the sources
SET(ROSC_CORE_SOURCES ${ROSC_CORE_SOURCES_TMP})
#message(STATUS ${ROSC_CORE_SOURCES})

#Remove temporary list
UNSET(ROSC_CORE_SOURCES_TMP)


