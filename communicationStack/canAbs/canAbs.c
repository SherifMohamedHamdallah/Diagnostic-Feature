#include "canAbs.h"

static LOGICAL_CH_t udtlogChannels[CAN_ABS_NUM_LOG_CHANNELS] = {{0x741,0x740}};

CIRCULAR_BUFFER_t canAbs_udtpCirBufOfChs[CAN_ABS_NUM_LOG_CHANNELS];

static void vidRxIndication(uint8_t u8LogCh)
{
	CANTP_CALLBACK_RX_INDICATION(u8LogCh);
}

void canAbs_vidInit()
{
	uint8_t u8iInex;
	for(u8iInex=0;u8iInex<CAN_ABS_NUM_LOG_CHANNELS;u8iInex++)
	{
		memUtils_vidConfigCirBuff(&canAbs_udtpCirBufOfChs[u8iInex]);
	}
}
void canAbs_vidSend(uint8_t u8ChId,uint8_t* u8pData,uint8_t u8Size)
{
	can_vidSend(udtlogChannels[u8ChId].u32ClientID,u8pData,u8Size);
}

void canAbs_vidRxMain()
{
	uint8_t u8Index;
	int8_t u8LogCh = -1;
	uint8_t u8pTempBuff[8];
	uint8_t u8Size;
	uint32_t u32canId;
	bool_t udtResult = false;
	if(can_udtRead(&u32canId,u8pTempBuff,&u8Size))
	{
		for(u8Index=0;u8Index<CAN_ABS_NUM_LOG_CHANNELS;u8Index++)
		{
			if(u32canId == udtlogChannels[u8Index].u32ServerID )
			{
				u8LogCh = u8Index;
			}
		}
		if(u8LogCh == -1)
		{
			return;
		}
		//first push size
		udtResult = memUtils_udtPushData(&canAbs_udtpCirBufOfChs[u8LogCh],&u8Size,1);
		if(!udtResult)
		{
			return;
		}
		//second push data
		udtResult = memUtils_udtPushData(&canAbs_udtpCirBufOfChs[u8LogCh],u8pTempBuff,u8Size);
		if(!udtResult)
		{
			return;
		}
		vidRxIndication(u8LogCh);

	}

}
