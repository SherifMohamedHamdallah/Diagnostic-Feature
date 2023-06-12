#ifndef CAN_TP_H
#define CAN_TP_H

#include "../../scheduler/sched.h"
#include "../canAbs/canAbs.h"
#include "../netIf/netIf.h"
#define CAN_TP_u8BS 										3
#define CAN_TP_u8ST_MIN 									0
#define CAN_TP_MAX_TX_BUFF_SIZE								100
#define CAN_TP_NUM_LOG_CHS  								CAN_ABS_NUM_LOG_CHANNELS
#define NETIF_vidCALLBACK_START_OF_RECEPTION()              netIf_vidStartOfReception()
#define NETIF_vidCALLBACK_DATA_RECEIVEID(LOG_CH)			netIf_vidDataReceived(LOG_CH)

typedef enum{txProcess,rxProcess}activeProcess_t;
typedef enum{start,segmentation}smRxCantTp_t;
typedef enum{waitingFc,sendingCF}smTxCantTp_t;

typedef struct{
	uint8_t * u8pBuffer;
	uint16_t u16BuffSize;
	uint16_t u16MaxBuffSize;
}canTpBuffer_t;

typedef struct{
	uint8_t  u8pBuffer[CAN_TP_MAX_TX_BUFF_SIZE];
	uint16_t u16BuffSize;
	uint16_t u16StartIndex;
}canTpTxBuffer_t;

extern canTpBuffer_t udtArraysOfBuff[CAN_TP_NUM_LOG_CHS];

bool_t canTp_udtSetParameter(uint8_t u8LogCh,uint8_t * u8pBuff,uint16_t u16Size);
void canTp_vidRxIndication(uint8_t u8LogCh);
void canTp_vidRxMain();
void canTp_vidSendFrame(uint8_t u8LogCh,uint8_t * u8pBuff,uint16_t u16Size);
void canTp_vidTxMain();


#endif
