message(STATUS "Getting rosc sources...")

#all sources of rosc_core
SET(ROSC_CORE_SOURCES
src/rosc/system/rosc_spin.c
src/rosc/system/ports.c
src/rosc/system/system.c
src/rosc/system/rosc_init.c
src/rosc/string_res/msg_strings.c
src/rosc/com_xml/parse/handler/xmlrpc_server.c
src/rosc/com_xml/parse/handler/xmlrpc_client.c
src/rosc/com_xml/parse/sub/copy2buffer.c
src/rosc/com_xml/parse/sub/seekstring.c
src/rosc/com_xml/parse/sub/skipwholemessage.c
src/rosc/com_xml/parse/sub/numberparse.c
src/rosc/com_xml/parse/sub/parseurl.c
src/rosc/com_xml/parse/parser.c
src/rosc/com_xml/parse/mode/parse_mode_xml.c
src/rosc/com_xml/parse/mode/parse_mode_header.c
src/rosc/com_xml/msg_gen.c
)


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


