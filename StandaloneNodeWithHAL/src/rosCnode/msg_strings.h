/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_strings.h
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */


#ifndef MSG_STRINGS_H_
#define MSG_STRINGS_H_

/**
 * This are values for inserting tag values into enums of message generators or parsers
 */
#define __RPC_XML_TAG_STRINGS(ENUM) 			\
	ENUM ## _TAG_ARRAY,							\
	ENUM ## _TAG_BODY,       					\
	ENUM ## _TAG_BOOLEAN,    					\
	ENUM ## _TAG_DATA,       					\
	ENUM ## _TAG_FAULT,      					\
    ENUM ## _TAG_H1,        					\
    ENUM ## _TAG_HEAD,       					\
    ENUM ## _TAG_INT,       					\
    ENUM ## _TAG_MEMBER,     					\
    ENUM ## _TAG_METHODCALL,					\
    ENUM ## _TAG_METHODNAME,					\
    ENUM ## _TAG_METHODRESPONSE,				\
    ENUM ## _TAG_P,								\
    ENUM ## _TAG_PARAM,							\
    ENUM ## _TAG_PARAMS,						\
    ENUM ## _TAG_STRING,						\
    ENUM ## _TAG_STRUCT,						\
	ENUM ## _TAG_TITLE,							\
	ENUM ## _TAG_VALUE

#define __RPC_STDTXT(ENUM) 						\
 	ENUM ## _STDTXT__XMLDEF  					\
	ENUM ## _STDTXT__HASPARAM 					\
	ENUM ## _STDTXT__REGISTERPUBLISHER			\
	ENUM ## _STDTXT__REGISTERSUBSCRIBER			\
	ENUM ## _STDTXT__REQUESTTOPIC				\
	ENUM ## _STDTXT__TCP_KEEPALIVE				\
	ENUM ## _STDTXT__UNREGISTERPUBLISHER		\
	ENUM ## _STDTXT__UNREGISTERSUBSCRIBER		\


/**
 * This array contains the strings for standard tags.
 */
extern const char *rpc_xml_tag_strings[];

/**
 * This array contains strings for standard text pieces.
 */
extern const char *rpc_xml_stdtext[];

/**
 * This array contains the strings for standard fields in the message headers
 */
extern const char *http_header_descriptors[];

/**
 * This array contains the standard value strings for generating message headers
 */
extern const char *http_header_stdtext[];

#endif /* MSG_STRINGS_H_ */
