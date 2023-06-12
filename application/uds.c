#include "uds.h"

uint16_t UDS_u16P2MS = 50;
uint16_t UDS_u16P2StarMS = 500;
bool_t UDS_udtExtendedSessionEnabled = false;
static void (*ptrToResponsFun)(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType);

void UDS_vidSendExtendedSession()
{
	dcm_vidSendRequest(&dcm_udtRequestExtendedSession);
	ptrToResponsFun = UDS_vidReceiveResExtendedSession;
}
void UDS_vidReceiveResExtendedSession(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType)
{
	if(responseType == pos_res && u16Size == 6 )
	{
		UDS_u16P2MS = ((uint16_t)u8PBuff[2]<<8)|(uint16_t)u8PBuff[3];
		UDS_u16P2MS = ((uint16_t)u8PBuff[2]<<8)|(uint16_t)u8PBuff[3];
		UDS_udtExtendedSessionEnabled = true;
	}
	else{
		UDS_udtExtendedSessionEnabled = false;
	}
}
void UDS_vidSendReadDids()
{
	dcm_vidSendRequest(&dcm_udtRequestReadDids);
	ptrToResponsFun = UDS_vidReceiveResReadDids;
}
void UDS_vidReceiveResReadDids(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType)
{
	if(responseType == pos_res && u16Size == 20 )
	{
		//perform operations according to each Did
	}
}
void UDS_vidProcessResponse(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType)
{
	ptrToResponsFun(u8PBuff,u16Size,responseType);
}
void UDS_vidP2Event()
{
	if(dcm_udtPendingReceived == true)
	{
		dcm_udtPendingReceived = false;
		sched_vidActivateTask(&sched_p2StarEvent);
		sched_vidDeactivateTask(&sched_p2Event);
	}
	else
	{
		//timeout occured
	}
}
void UDS_vidP2StartEvent()
{
	//timeout occured
}
