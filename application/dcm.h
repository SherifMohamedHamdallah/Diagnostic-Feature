#ifndef DCM_H
#define DCM_H

#include "../communicationStack/netIf/netIf.h"
typedef enum{neg_res,pos_res}responseType_t;
#include "uds.h"
#define DCM_u8CAN_CH			  0
#define DCM_u16MAX_BUFFER_SIZE    100
#define DCM_u16MAX_REQUEST_LEN    100
#define DCM_CALL_BACK_UDS_PROCESS_RESPONSE(BUFF,LEN,RES) UDS_vidProcessResponse(BUFF,LEN,RES)

extern uint8_t dcm_u8pBufferForDCM[DCM_u16MAX_BUFFER_SIZE];

typedef struct{
	uint8_t u8pRequestData[DCM_u16MAX_REQUEST_LEN];
	uint16_t u16RequestLen;
}service_t;

extern bool_t dcm_udtPendingReceived;
extern service_t dcm_udtRequestExtendedSession;
extern service_t dcm_udtRequestReadDids;

void dcm_vidInit();
void dcm_vidSendRequest(service_t * udtpService);
void dcm_vidProcessResponse();

#endif
