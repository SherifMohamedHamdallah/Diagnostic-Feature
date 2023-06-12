#ifndef CAN_ABS_H
#define CAN_ABS_H
#include "../can/can.h"

#define CAN_ABS_NUM_LOG_CHANNELS 1
#include "../canTp/canTp.h"

#define CANTP_CALLBACK_RX_INDICATION(LOGICAL_CH)  canTp_vidRxIndication(LOGICAL_CH)



typedef struct{
	uint32_t u32ServerID;
	uint32_t u32ClientID;
}LOGICAL_CH_t;


extern CIRCULAR_BUFFER_t canAbs_udtpCirBufOfChs[1];

void canAbs_vidInit();
void canAbs_vidSend(uint8_t u8ChId,uint8_t* u8pData,uint8_t u8Size);
void canAbs_vidRxMain();

#endif
