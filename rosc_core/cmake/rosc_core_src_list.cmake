SET(ROSC_CORE_SOURCES
src/rosc/sebs_parse_fw/sebs_parser_frame.c
src/rosc/sebs_parse_fw/send_modules/msggen.c
src/rosc/sebs_parse_fw/send_modules/socket_connect.c
src/rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.c
src/rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.c
src/rosc/sebs_parse_fw/std_modules/sebs_parse_skip.c
src/rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.c
src/rosc/sebs_parse_fw/std_modules/sebs_parse_parseurl.c
src/rosc/sebs_parse_fw/adv_modules/sebs_parse_http.c
src/rosc/sebs_parse_fw/adv_modules/sebs_parse_xml.c
src/rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.c
src/rosc/com/publish.c
src/rosc/com/msg_gen_common.c
src/rosc/com/xmlrpc.c
src/rosc/com/msg_gen.c
src/rosc/com/ros_handler.c
src/rosc/system/eth.c
src/rosc/system/endian.c
src/rosc/system/system.c
src/rosc/system/rosc_spin.c
src/rosc/system/rosc_init.c
src/rosc/string_res/msg_strings.c
)
