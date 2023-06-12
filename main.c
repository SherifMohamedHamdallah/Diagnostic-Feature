/*
 * Owner : Sherif Hamdallah
 * Date :  12/6/2023
 * */

#include "debug.h"
#include"helper/memUtils.h"
#include "scheduler/sched.h"
#include "application/uds.h"

int main(){
	/*
	 * initialize clock and peripherals
	 */
	canAbs_vidInit();
	dcm_vidInit();

	UDS_vidSendExtendedSession();
	while(1)
	{
		sched_vidMain();
	}
	return 0;
}
