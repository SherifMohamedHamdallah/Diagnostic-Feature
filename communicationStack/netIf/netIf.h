#ifndef NET_IF_H
#define NET_IF_H
#include "../canTp/canTp.h"
#include "../../application/dcm.h"

#define NET_IF_CALL_DCM_PROCESS_RESPONSE()   dcm_vidProcessResponse()

extern uint8_t activeLogCh;
void netIf_vidSetActiveChannel(uint8_t u8logCh);
void netIf_vidSendFrame(uint8_t * u8pBuff,uint16_t u16Size);
void netIf_vidStartOfReception();
void netIf_vidDataReceived(uint8_t u8logCh);


#endif
