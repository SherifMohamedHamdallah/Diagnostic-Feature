#include "can.h"
#if DEBUG_IS_ENABLED
	volatile bool_t can_udtStatusRead = false;
	volatile uint8_t can_u8pArr[8]={0};
	volatile uint8_t can_u8DataSize = 8;

#endif
void can_vidInit(void)
{
	//hardware dependent
}
bool_t can_udtRead(uint32_t* u32pCanId,uint8_t* u8pCanData,uint8_t *u8pDataSize)
{
#if DEBUG_IS_ENABLED
	memUtils_vidCopyBuffer(can_u8pArr,u8pCanData,&can_u8DataSize);
	return can_udtStatusRead;
#else
	//hardware dependent
#endif

}
void can_vidSend(uint32_t u32CanId,uint8_t* u8pCanData,uint8_t u8DataSize)
{
#if DEBUG_IS_ENABLED
	memUtils_vidCopyBuffer(u8pCanData,can_u8pArr,&can_u8DataSize);
#else
	//hardware dependent
#endif
}
