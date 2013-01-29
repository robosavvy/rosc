// /////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// /////////////////////////

#include <rosc/rosc.h>

void callbackTest(void *package)
{

}

int main()
{
	rosc_init();






	while(1)
	{


		rosc_spin();
	}


	return 0;
}















//#pragma pack(push, 1)
//	struct
//	{
//		char test[5];
//		uint32_t size;
//	}gaga={.test="test",.size=4};
//#pragma pack(pop)
//
//	void *ptr = &gaga;
//	int i;
//	for (i = 0; i < sizeof(gaga); ++i)
//	{
//		char *p=(char *)ptr;
//		unsigned int o = *p;
//
//		printf("%x\n", o);
//
//		ptr++;
//	}

