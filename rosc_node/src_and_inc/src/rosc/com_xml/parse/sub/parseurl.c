#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif

	#include <rosc/com_xml/parse/sub/parseurl.h>
#endif


#ifndef FORCE_INLINE
	void parseurl(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact)
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
		uint32_t len=*len_ptr;
		char *buf=*buf_ptr;
	#endif


	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif
}
#endif


