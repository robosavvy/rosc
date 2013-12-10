


#ifndef BYTE_HANDLING_H_
#define BYTE_HANDLING_H_

/**
 * This copies one string into another location.
 * It replaces strcpy to get rid of the function call itself
 */
#define ROSC_STRCPY(DST,SRC)\
{\
	char *rosc_strcpy_src=SRC;\
	char *rosc_strcpy_dst=DST;\
	do\
	{\
		*rosc_strcpy_dst=*rosc_strcpy_src;\
		rosc_strcpy_dst++;\
		rosc_strcpy_src++;\
	}\
	while(*rosc_strcpy_src);\
}
#endif /* BYTE_HANDLING_H_ */
