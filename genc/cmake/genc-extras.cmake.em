@[if DEVELSPACE]@
# bin and template dir variables in develspace
set(GENC_BIN "@(CMAKE_CURRENT_SOURCE_DIR)/scripts/gen_c.py")
set(GENC_TEMPLATE_DIR "@(CMAKE_CURRENT_SOURCE_DIR)/scripts")
@[else]@
# bin and template dir variables in installspace
set(GENC_BIN "${genc_DIR}/../../../@(CATKIN_PACKAGE_BIN_DESTINATION)/gen_c.py")
set(GENCPP_TEMPLATE_DIR "${genc_DIR}/..")
@[end if]@

# Generate .msg->.h for c
# The generated .h files should be added ALL_GEN_OUTPUT_FILES_c
macro(_generate_msg_c ARG_PKG ARG_MSG ARG_IFLAGS ARG_MSG_DEPS ARG_GEN_OUTPUT_DIR)
  file(MAKE_DIRECTORY ${ARG_GEN_OUTPUT_DIR})

  #Create input and output filenames
  get_filename_component(MSG_NAME ${ARG_MSG} NAME)
  get_filename_component(MSG_SHORT_NAME ${ARG_MSG} NAME_WE)

  set(MSG_GENERATED_NAME ${MSG_SHORT_NAME}_c.h)
  set(GEN_OUTPUT_FILE ${ARG_GEN_OUTPUT_DIR}/${MSG_GENERATED_NAME})

  assert(CATKIN_ENV)
  add_custom_command(OUTPUT ${GEN_OUTPUT_FILE}
    DEPENDS ${GENC_BIN} ${ARG_MSG} ${ARG_MSG_DEPS} "${GENCPP_TEMPLATE_DIR}/msg.h.template" ${ARGN}
    COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENC_BIN} ${ARG_MSG}
    ${ARG_IFLAGS}
    -p ${ARG_PKG}
    -o ${ARG_GEN_OUTPUT_DIR}
    -e ${GENC_TEMPLATE_DIR}
    COMMENT "Generating C code from ${ARG_PKG}/${MSG_NAME}"
    )
  list(APPEND ALL_GEN_OUTPUT_FILES_c ${GEN_OUTPUT_FILE})

  genc_append_include_dirs()
endmacro()

#genc uses the same program to generate srv and msg files, so call the same macro
macro(_generate_srv_c ARG_PKG ARG_SRV ARG_IFLAGS ARG_MSG_DEPS ARG_GEN_OUTPUT_DIR)
  _generate_msg_c(${ARG_PKG} ${ARG_SRV} "${ARG_IFLAGS}" "${ARG_MSG_DEPS}" ${ARG_GEN_OUTPUT_DIR} "${GENCPP_TEMPLATE_DIR}/srv.h.template")
endmacro()

macro(_generate_module_c)
  # the macros, they do nothing
endmacro()

set(genc_INSTALL_DIR include)

macro(genc_append_include_dirs)
  if(NOT genc_APPENDED_INCLUDE_DIRS)
    # make sure we can find generated messages and that they overlay all other includes
    include_directories(BEFORE ${CATKIN_DEVEL_PREFIX}/${genc_INSTALL_DIR})
    # pass the include directory to catkin_package()
    list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${CATKIN_DEVEL_PREFIX}/${genc_INSTALL_DIR})
    set(genc_APPENDED_INCLUDE_DIRS TRUE)
  endif()
endmacro()
