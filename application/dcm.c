#include "dcm.h"

bool_t dcm_udtPendingReceived = false;
static uint8_t u8WaitngPosResFirstByte=0;

service_t dcm_udtRequestExtendedSession =  {
		{0x10,0x3},2
};
service_t dcm_udtRequestReadDids =  {
		{0x22,0x20,0x30,0x20,0x31,0x20,0x32},7
};

uint8_t dcm_u8pBufferForDCM[DCM_u16MAX_BUFFER_SIZE];

void dcm_vidInit()
{
	dcm_udtPendingReceived = false;
	u8WaitngPosResFirstByte=0;
	netIf_vidSetActiveChannel(DCM_u8CAN_CH);
}
void dcm_vidSendRequest(service_t * udtpService)
{
	//activate p2 event
	sched_vidActivateTask(&sched_p2Event);
	netIf_vidSendFrame(udtpService->u8pRequestData,udtpService->u16RequestLen);
	u8WaitngPosResFirstByte = udtpService->u8pRequestData[0]+0x40;
}
void dcm_vidProcessResponse()
{
	//check postive response
	if(udtArraysOfBuff[DCM_u8CAN_CH].u8pBuffer[0] == u8WaitngPosResFirstByte)
	{
		DCM_CALL_BACK_UDS_PROCESS_RESPONSE(udtArraysOfBuff[DCM_u8CAN_CH].u8pBuffer,
				udtArraysOfBuff[DCM_u8CAN_CH].u16BuffSize,pos_res);
	}
	//negative response
	else if(udtArraysOfBuff[DCM_u8CAN_CH].u8pBuffer[0] == 0x7f)
	{
		if(udtArraysOfBuff[DCM_u8CAN_CH].u8pBuffer[1] == 0x78)
		{
			dcm_udtPendingReceived = true;
		}
		else
		{
			DCM_CALL_BACK_UDS_PROCESS_RESPONSE(udtArraysOfBuff[DCM_u8CAN_CH].u8pBuffer,
							udtArraysOfBuff[DCM_u8CAN_CH].u16BuffSize,neg_res);
		}
	}
}
