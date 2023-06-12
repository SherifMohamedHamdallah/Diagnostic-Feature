#ifndef MEM_UTILS_H
#define MEM_UTILS_H
#include "dataTypes.h"

#define memUtils_u32MAX_BUFFER_SIZE 100UL

typedef struct{
	uint8_t buff[memUtils_u32MAX_BUFFER_SIZE];
	uint16_t startId;
	uint16_t endId;
	uint16_t byteCount;
}CIRCULAR_BUFFER_t;

void memUtils_vidMemSet(uint8_t * u8pBuff,uint8_t u8Size,uint8_t u8Value);
void memUtils_vidCopyBuffer(uint8_t * u8psrc_buf,uint8_t * u8pDest_buff,uint8_t u8Size);
void memUtils_vidConfigCirBuff(CIRCULAR_BUFFER_t * udtpBuff);
bool_t memUtils_udtPushData(CIRCULAR_BUFFER_t * udtPCbuff,uint8_t * u8pDataPtr,uint16_t u16dataLen);
bool_t memUtils_udtRetrieveData(CIRCULAR_BUFFER_t * udtPCbuff,uint8_t * u8pDataPtr,uint16_t  u16dataLen );




#endif
