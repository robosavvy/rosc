var sebs__parse__xml_8h =
[
    [ "sebs_parse_xml_data_t", "d9/dbb/structsebs__parse__xml__data__t.html", "d9/dbb/structsebs__parse__xml__data__t" ],
    [ "SEBS_PARSE_XML_INIT", "d6/d7d/sebs__parse__xml_8h.html#a384386f86f05a2f326f22405d4b2ab85", null ],
    [ "SEBS_PARSE_XML_MAX_DEPTH", "d6/d7d/sebs__parse__xml_8h.html#a6306db3ab64217ceada971230a2dca52", null ],
    [ "sebs_parse_xml_event_t", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385", [
      [ "SEBS_PARSE_XML_EVENT_NONE", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385acdfb5dc65c5edd0bbedd8b1427c68066", null ],
      [ "SEBS_PARSE_XML_EVENT_ERROR_DEPTH", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385af15e1f7cdf900e26e5e9d90ce47ee26a", null ],
      [ "SEBS_PARSE_XML_EVENT_ERROR_MALFORMED", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385ab22298e4d23dd8494a6f314ca598e14b", null ],
      [ "SEBS_PARSE_XML_EVENT_TAG", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385ac08da50795b5bb925bdc168cef880064", null ],
      [ "SEBS_PARSE_XML_EVENT_INSIDE_TAG", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385a6e91aebe84fd01ad9476866de5912ea4", null ],
      [ "SEBS_PARSE_XML_EVENT_HANDLER_CALLED_SUBMODE_FINISHED", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385acc206ee1b712660781895e37e24aa661", null ],
      [ "SEBS_PARSE_XML_EVENT_CONTENT_START", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385a1f0f8bbb000ef11eaf1efa11fae804fd", null ],
      [ "SEBS_PARSE_XML_EVENT_CDATA", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385af4175729de9e26e778f6c14bbb085fb6", null ],
      [ "SEBS_PARSE_XML_EVENT_ATTRIBUTE_APOSTROPHE", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385a4463edff7d81e1bd1cc3a41085ed943d", null ],
      [ "SEBS_PARSE_XML_EVENT_ATTRIBUTE_QUOTES", "d6/d7d/sebs__parse__xml_8h.html#a07ec8b686627a47b700a4309bf346385ace90d5b9491987d7931d85738da1a9a1", null ]
    ] ],
    [ "sebs_parse_xml_state_t", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922", [
      [ "SEBS_PARSE_XML_STATE_INIT", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922ae58801432e3349714830aebc4f91a3e9", null ],
      [ "SEBS_PARSE_XML_STATE_ROOT", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922aff0a2a7e23e8a4369de390ea48bab468", null ],
      [ "SEBS_PARSE_XML_STATE_INNER", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922addaa9f756184d00aa3bd159315126f21", null ],
      [ "SEBS_PARSE_XML_STATE_TAG_START", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a2873e86612e41efaa7fe8684a642b86f", null ],
      [ "SEBS_PARSE_XML_STATE_TAG", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922afb97d37fcf9cc52e73883349d4211f98", null ],
      [ "SEBS_PARSE_XML_STATE_CLOSE_TAG_START", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a18645a1bf8105f9931889e730d8a9a43", null ],
      [ "SEBS_PARSE_XML_STATE_EXPECT_SELFCLOSE_TAG_END", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a74a453a3e59ab703dc180b18f51abc76", null ],
      [ "SEBS_PARSE_XML_STATE_CDATA", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922afe4457dc58691d9b1c0295a92f5edfd5", null ],
      [ "SEBS_PARSE_XML_STATE_CDATA_START", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922ac0e83bea600402e01165731afcd0cbbb", null ],
      [ "SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922af5bf24e70e7f76c28dda40434e1b5fd9", null ],
      [ "SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a8aae408888f952e178fc1a309ffa9935", null ],
      [ "SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a0aed4ded88834fa15ca48a8c4bc53024", null ],
      [ "SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a79609dc9d904bcbcc1daec44ca938699", null ],
      [ "SEBS_PARSE_XML_STATE_COMMENT_START_1ST_DASH", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922ab04126980f5a2d6d2f66f1a85ac38eda", null ],
      [ "SEBS_PARSE_XML_STATE_COMMENT", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a957c926037d6702111ec2af422becc66", null ],
      [ "SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922af9a60a71b64af29630272f8f4aa23b54", null ],
      [ "SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922ac3aae1604890a92622917fc681972202", null ],
      [ "SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922aa7799fc42aaf7ec503b8e3ee91e6f0e8", null ],
      [ "SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a79443c1ecd1502d1e0b36f98e4ed6cdc", null ],
      [ "SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a11b1c30d6f421af6e83ff68cf782f83a", null ],
      [ "SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922aec6fe1350c2e71484e99ff42f1bd0ffb", null ],
      [ "SEBS_PARSE_XML_STATE_INNER_CONTENT", "d6/d7d/sebs__parse__xml_8h.html#aaaa9d91ae98831297f18bf19091f1922a619c1956b3df3acd6ec6006df96e99f2", null ]
    ] ],
    [ "sebs_parse_xml_sub_state_t", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812f", [
      [ "SEBS_PARSE_XML_SUBSTATE_NONE", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812faa46b66affabd31c14c7ce76067cc8622", null ],
      [ "SEBS_PARSE_XML_SUBSTATE_TAG_ID", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812fae12f30a657cc91fd54e5dbaa7cc15f2a", null ],
      [ "SEBS_PARSE_XML_SUBSTATE_ATTRIBUTE_ID", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812fa99b4afe1513bb16f6ebab4f1d33baa09", null ],
      [ "SEBS_PARSE_XML_SUBSTATE_COMMENT_END_1ST_DASH", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812fac961677e8cfa9bafe4750b5aae1a3a49", null ],
      [ "SEBS_PARSE_XML_SUBSTATE_COMMENT_END_2ND_DASH", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812fa97b70b54e4c2b063bb04b159600f4e15", null ],
      [ "SEBS_PARSE_XML_SUBSTATE_CDATA_TAG_STRING", "d6/d7d/sebs__parse__xml_8h.html#a57412c792c6c73c80ff8dbf72ab8812faecd71ab543cf04411b65700c19681120", null ]
    ] ],
    [ "sebs_parse_xml_tag_type_t", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6", [
      [ "SEBS_PARSE_XML_TAG_TYPE_NORMAL", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6afdd8982a2e05a02e7ab3f506738d8cc7", null ],
      [ "SEBS_PARSE_XML_TAG_TYPE_CLOSE", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6ae39f67ab14c1fb2cca3d2c00d18c2c49", null ],
      [ "SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6a8e50559a97dd2d238f794578cb192ee3", null ],
      [ "SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6ab7ed4892ec8da66f71ce4def481190e9", null ],
      [ "SEBS_PARSE_XML_TAG_TYPE_COMMENT", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6ac5b1b5586a4971490d9d82b0ed7631f1", null ],
      [ "SEBS_PARSE_XML_TAG_TYPE_CDATA", "d6/d7d/sebs__parse__xml_8h.html#ace13bfaf94d110cace2d261d18a840d6adbbdb74aa94290836ccd0c9241a1ee44", null ]
    ] ],
    [ "sebs_parse_xml", "d6/d7d/sebs__parse__xml_8h.html#aee03081d74f5ce257b5e0ed62b6e68c1", null ]
];