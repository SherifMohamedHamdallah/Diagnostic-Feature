#ifndef UDS_H
#define UDS_H

#include "dcm.h"

extern uint16_t UDS_u16P2MS;
extern uint16_t UDS_u16P2StarMS;
extern bool_t UDS_udtExtendedSessionEnabled;

void UDS_vidSendExtendedSession();
void UDS_vidReceiveResExtendedSession(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType);
void UDS_vidSendReadDids();
void UDS_vidReceiveResReadDids(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType);
void UDS_vidProcessResponse(uint8_t * u8PBuff,uint16_t u16Size,responseType_t responseType);
void UDS_vidP2Event();
void UDS_vidP2StartEvent();
#endif
