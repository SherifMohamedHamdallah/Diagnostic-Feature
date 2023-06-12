#ifndef CAN_H
#define CAN_H

#include "../../helper/dataTypes.h"
#include "../../helper/memUtils.h"
#include "../../debug.h"

void can_vidInit(void);
bool_t can_udtRead(uint32_t* u32pCanId,uint8_t* u8pCanData,uint8_t *u8pDataSize);
void can_vidSend(uint32_t u32CanId,uint8_t* u8pCanData,uint8_t u8DataSize);

#endif
