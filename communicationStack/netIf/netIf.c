#include "netIf.h"

uint8_t activeLogCh = 0;
void netIf_vidSetActiveChannel(uint8_t u8logCh)
{
	activeLogCh = u8logCh;
	canTp_udtSetParameter(activeLogCh,dcm_u8pBufferForDCM,DCM_u16MAX_BUFFER_SIZE);
}
void netIf_vidSendFrame(uint8_t * u8pBuff,uint16_t u16Size)
{
	canTp_vidSendFrame(activeLogCh,u8pBuff,u16Size);
}
void netIf_vidStartOfReception()
{
	sched_vidDeactivateTask(&sched_p2Event);
	sched_vidDeactivateTask(&sched_p2StarEvent);
}
void netIf_vidDataReceived(uint8_t u8logCh)
{
	if(u8logCh == activeLogCh)
	{
		NET_IF_CALL_DCM_PROCESS_RESPONSE();
	}

}
