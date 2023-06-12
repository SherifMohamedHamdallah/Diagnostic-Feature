#include "memUtils.h"

void memUtils_vidMemSet(uint8_t * u8pBuff,uint8_t u8Size,uint8_t u8Value)
{
	uint8_t u8Index = 0;
	for(u8Index=0;u8Index<u8Size;u8Index++)
	{
		u8pBuff[u8Index] = u8Value;
	}
}

void memUtils_vidCopyBuffer(uint8_t * u8psrc_buf,uint8_t * u8pDest_buff,uint8_t u8Size)
{
	uint8_t i;
	for(i=0;i<u8Size;i++)
	{
		u8pDest_buff[i] = u8psrc_buf[i];
	}
}


void memUtils_vidConfigCirBuff(CIRCULAR_BUFFER_t * udtpBuff)
{
	udtpBuff->startId = 0;
	udtpBuff->endId = 0;
	udtpBuff->byteCount = 0;
}

bool_t memUtils_udtPushData(CIRCULAR_BUFFER_t * udtPCbuff,uint8_t * u8pDataPtr,uint16_t u16dataLen)
{
	uint16_t u16RemainingBytes = 0;
	if(u16dataLen + udtPCbuff->byteCount> memUtils_u32MAX_BUFFER_SIZE)
	{
		return false;
	}
	if((udtPCbuff->endId + u16dataLen) >=memUtils_u32MAX_BUFFER_SIZE)
	{
		u16RemainingBytes = u16dataLen - (memUtils_u32MAX_BUFFER_SIZE-udtPCbuff->endId);
		memUtils_vidCopyBuffer(u8pDataPtr,udtPCbuff->buff +udtPCbuff->endId,u16dataLen - u16RemainingBytes);
		memUtils_vidCopyBuffer(u8pDataPtr+u16dataLen - u16RemainingBytes,udtPCbuff->buff ,u16RemainingBytes);
		udtPCbuff->endId += u16dataLen;
		udtPCbuff->endId %= memUtils_u32MAX_BUFFER_SIZE;
	}
	else{
		memUtils_vidCopyBuffer(u8pDataPtr,udtPCbuff->buff +udtPCbuff->endId,u16dataLen);
		udtPCbuff->endId += u16dataLen;
	}

	udtPCbuff->byteCount+=u16dataLen;

	return true;
}
bool_t memUtils_udtRetrieveData(CIRCULAR_BUFFER_t * udtPCbuff,uint8_t * u8pDataPtr,uint16_t  u16dataLen )
{
	uint16_t u16RemainingBytes = 0;
	if(u16dataLen > udtPCbuff->byteCount)
	{
		return false;
	}
	if((udtPCbuff->startId + u16dataLen)>=memUtils_u32MAX_BUFFER_SIZE)
	{
		u16RemainingBytes = u16dataLen - (memUtils_u32MAX_BUFFER_SIZE-udtPCbuff->startId);
		memUtils_vidCopyBuffer(udtPCbuff->buff + udtPCbuff->startId,u8pDataPtr,u16dataLen - u16RemainingBytes);
		memUtils_vidCopyBuffer(udtPCbuff->buff,u8pDataPtr +(u16dataLen - u16RemainingBytes) ,u16RemainingBytes);
		udtPCbuff->startId += u16dataLen;
		udtPCbuff->startId%=memUtils_u32MAX_BUFFER_SIZE;
	}
	else
	{
		memUtils_vidCopyBuffer(udtPCbuff->buff + udtPCbuff->startId,u8pDataPtr,u16dataLen);
		udtPCbuff->startId += u16dataLen;
	}

	udtPCbuff->byteCount -= u16dataLen;

	return true;
}
