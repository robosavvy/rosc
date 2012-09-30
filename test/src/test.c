#include <stdio.h>

         const char *strings[] =
	     {
		 "abc",  //replaced with "cab\0" at compile time
		 "def"   //replaced with "fed\0" at compile time
	     };


#define TA(string)\
         sizeof(string)


#define STR_VALUE(arg)      arg
#define FUNCTION_NAME(name) STR_VALUE(name)


int main()
{



    return 0;
}

