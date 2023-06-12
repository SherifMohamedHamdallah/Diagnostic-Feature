#include "canTp.h"

static smRxCantTp_t udtRxStateM = start;
static smTxCantTp_t udtTxStateM = waitingFc;
canTpBuffer_t udtArraysOfBuff[CAN_TP_NUM_LOG_CHS];
static uint8_t u8ActiveCh = 0;
static activeProcess_t udtActiveProcess = rxProcess;
static canTpTxBuffer_t udtpArrOfTxBuff[CAN_TP_NUM_LOG_CHS];
bool_t canTp_udtSetParameter(uint8_t u8LogCh,uint8_t * u8pBuff,uint16_t u16Size)
{
	if(u8LogCh<CAN_TP_NUM_LOG_CHS)
	{
		udtArraysOfBuff[u8LogCh].u8pBuffer = u8pBuff;
		udtArraysOfBuff[u8LogCh].u16MaxBuffSize = u16Size;
		udtArraysOfBuff[u8LogCh].u16BuffSize = 0;
		return true;
	}
	return false;

}
void canTp_vidRxIndication(uint8_t u8LogCh)
{
	u8ActiveCh = u8LogCh;
	if(udtActiveProcess == rxProcess)
	{
		//activate canTp_vidRxMain
		sched_vidActivateTask(&sched_canTpRXMain);
	}
	else
	{
		//activate canTp_vidTxMain
		sched_vidActivateTask(&sched_canTpTXMain);
	}

}
void canTp_vidRxMain()
{
	uint8_t u8CanDL;
	uint8_t u8SfDLen;
	uint8_t u8pTempBuff[8];
	static uint8_t u8SN = 0;
	static uint16_t u16TotalCF = 0;
	uint16_t u16FF_DL;
	if(udtRxStateM == start )
	{
		if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],&u8CanDL,1))
		{
			if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],u8pTempBuff,u8CanDL))
			{
				//SF
				if((u8pTempBuff[0]>>4)==0x0)
				{
					u8SfDLen = u8pTempBuff[0]&0xf;
					if((u8SfDLen!=0)&&(u8SfDLen<=(u8CanDL-1)))
					{
						memUtils_vidCopyBuffer(u8pTempBuff+1,udtArraysOfBuff[u8ActiveCh].u8pBuffer,u8SfDLen);
						udtArraysOfBuff[u8ActiveCh].u16BuffSize = u8SfDLen;
						NETIF_vidCALLBACK_START_OF_RECEPTION();
						NETIF_vidCALLBACK_DATA_RECEIVEID(u8ActiveCh);
					}
				}
				//FF
				else if((u8pTempBuff[0]>>4)==0x1)
				{
					u16FF_DL = (((uint16_t)u8pTempBuff[0]&0xf)<<8)|(uint16_t)u8pTempBuff[1];
					if(u16FF_DL>=8)
					{
						if(u16FF_DL>udtArraysOfBuff[u8ActiveCh].u16BuffSize)
						{
							//send FC with OV
							u8pTempBuff[0] = 0x32;
							memUtils_vidMemSet(u8pTempBuff+1,7,0x0);
							canAbs_vidSend(u8ActiveCh,u8pTempBuff,8);

						}
						else{
							NETIF_vidCALLBACK_START_OF_RECEPTION();
							u16TotalCF = (u16FF_DL-6)/7+((u16FF_DL-6)%7)!=0;
							memUtils_vidCopyBuffer(u8pTempBuff+2,udtArraysOfBuff[u8ActiveCh].u8pBuffer,6);
							udtArraysOfBuff[u8ActiveCh].u16BuffSize = 6;
							u8SN = 0;
							udtRxStateM = segmentation;
							//send FC with continous send
							u8pTempBuff[0] = 0x30;
							u8pTempBuff[1] = CAN_TP_u8BS;
							u8pTempBuff[2] = CAN_TP_u8ST_MIN;
							memUtils_vidMemSet(u8pTempBuff+3,5,0x0);
							canAbs_vidSend(u8ActiveCh,u8pTempBuff,8);
						}
					}
				}
			}
		}
		else if(udtRxStateM == segmentation )
		{
			if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],&u8CanDL,1))
			{
				if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],u8pTempBuff,u8CanDL))
				{
					//CF
					if((u8pTempBuff[0]>>4)==0x2)
					{
						if(u8SN ==((u8pTempBuff[0]&0xf)%15))
						{
							memUtils_vidCopyBuffer(u8pTempBuff+1,
									udtArraysOfBuff[u8ActiveCh].u8pBuffer+udtArraysOfBuff[u8ActiveCh].u16BuffSize,u8CanDL);
							udtArraysOfBuff[u8ActiveCh].u16BuffSize+=u8CanDL;
							u8SN++;
							u16TotalCF--;
							if(u16TotalCF==0)
							{
								udtRxStateM = start;
								NETIF_vidCALLBACK_DATA_RECEIVEID(u8ActiveCh);
							}
							else if(u8SN%CAN_TP_u8BS == 0)
							{
								//send FC with continous send
								u8pTempBuff[0] = 0x30;
								u8pTempBuff[1] = CAN_TP_u8BS;
								u8pTempBuff[2] = CAN_TP_u8ST_MIN;
								memUtils_vidMemSet(u8pTempBuff+3,5,0x0);
								canAbs_vidSend(u8ActiveCh,u8pTempBuff,8);
							}
						}
					}
				}

			}

		}
	}


	//Deactivate canTp_vidRxMain
	sched_vidDeactivateTask(&sched_canTpRXMain);
}
void canTp_vidSendFrame(uint8_t u8LogCh,uint8_t * u8pBuff,uint16_t u16Size)
{
	uint8_t u8pTempBuff[8]={0};
	if(u16Size<=7)
	{
		//sending SF
		u8pTempBuff[0] = u16Size&0xf;
		memUtils_vidCopyBuffer(u8pBuff,u8pTempBuff+1,u16Size);
		canAbs_vidSend(u8LogCh,u8pTempBuff,u16Size+1);
	}
	else
	{
		//sending FF
		u8pTempBuff[0] = 0x10|((u16Size>>8)&0xf);
		u8pTempBuff[1] = (uint8_t)u16Size;
		memUtils_vidCopyBuffer(u8pBuff,u8pTempBuff+2,6);
		canAbs_vidSend(u8LogCh,u8pTempBuff,8);
		memUtils_vidCopyBuffer(u8pBuff+6,udtpArrOfTxBuff[u8LogCh].u8pBuffer,u16Size-6);
		udtpArrOfTxBuff[u8LogCh].u16BuffSize = u16Size-6;
		udtpArrOfTxBuff[u8LogCh].u16StartIndex = 0;
		udtActiveProcess = txProcess;
		udtTxStateM = waitingFc;
		u8ActiveCh = u8LogCh;
		//set period of canTp_vidTxMain min
		sched_vidSetPeriodOfTask(&sched_canTpTXMain,1);
	}
}

void canTp_vidTxMain()
{
	uint8_t u8pTempBuff[8];
	uint8_t u8CanDL;
	static uint8_t u8BS=0;
	uint8_t u8STmin=0;
	static uint8_t u8SN=0;
	if(udtTxStateM == waitingFc)
	{
		if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],&u8CanDL,1))
		{
			if(memUtils_udtRetrieveData(&canAbs_udtpCirBufOfChs[u8ActiveCh],u8pTempBuff,u8CanDL))
			{
				//FC
				if((u8pTempBuff[0]>>4)==0x2)
				{
					if((u8pTempBuff[0]&0xf)==0)
					{

						u8BS = u8pTempBuff[1];
						u8STmin = u8pTempBuff[2];
						//set period of canTp_vidTxMain with u8STmin
						sched_vidSetPeriodOfTask(&sched_canTpTXMain,u8STmin*1000);
						udtTxStateM = sendingCF;
					}
				}
			}
		}
	}
	else
	{
		if(udtpArrOfTxBuff[u8ActiveCh].u16BuffSize<=7)
		{
			//sending last CF
			u8pTempBuff[0] = 0x20|(u8SN&0xf);
			memUtils_vidCopyBuffer(udtpArrOfTxBuff[u8ActiveCh].u8pBuffer + udtpArrOfTxBuff[u8ActiveCh].u16StartIndex
					,u8pTempBuff+1,udtpArrOfTxBuff[u8ActiveCh].u16BuffSize);
			canAbs_vidSend(u8ActiveCh,u8pTempBuff,udtpArrOfTxBuff[u8ActiveCh].u16BuffSize);
			udtActiveProcess = rxProcess;
			u8SN=0;
		}
		else
		{
			u8pTempBuff[0] = 0x20|(u8SN&0xf);
			memUtils_vidCopyBuffer(udtpArrOfTxBuff[u8ActiveCh].u8pBuffer + udtpArrOfTxBuff[u8ActiveCh].u16StartIndex
					,u8pTempBuff+1,7);
			canAbs_vidSend(u8ActiveCh,u8pTempBuff,8);
			udtpArrOfTxBuff[u8ActiveCh].u16BuffSize-=7;
			udtpArrOfTxBuff[u8ActiveCh].u16StartIndex+=7;
			u8SN++;
			u8SN%=15;
			if(u8SN%u8BS)
			{
				udtTxStateM = waitingFc;
			}
			else
			{
				//without deactivation
				return;
			}
		}

	}

	//Deactivate canTp_vidTxMain
	sched_vidDeactivateTask(&sched_canTpRXMain);
}
