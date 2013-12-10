


#ifndef BYTE_HANDLING_H_
#define BYTE_HANDLING_H_

/**
 * This copies one string into another location.
 * It replaces strcpy to get rid of the function call itself
 */
#define ROSC_STRCPY(DST,SRC)\
{\
	char *rosc_strcpy_src=SRC;\
	int rosc_strcpy_pos=0;\
	while(rosc_strcpy_src)\
	{	\
		DST[rosc_strcpy_pos]=rosc_strcpy_src[rosc_strcpy_pos];\
		rosc_strcpy_pos++;\
	}\
}
#endif /* BYTE_HANDLING_H_ */
