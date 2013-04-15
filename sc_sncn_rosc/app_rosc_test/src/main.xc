

#include <platform.h>
#include <xs1.h>
#include <print.h>
#include <main_code.h>
#include <print.h>
int main(void)
{
	par
	{

		on stdcore[0] :
		{
			unsigned time1,time2,time12;
			timer tmr;

			tmr :> time1;
			rosc_test_main();


			tmr :> time2;
			time12=time2-time1;
			printintln(time12);
			while(1);
		}



	}
}



