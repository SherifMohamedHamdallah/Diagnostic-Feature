#include "sched.h"
#include "../communicationStack/canAbs/canAbs.h"
#include "../communicationStack/canTp/canTp.h"
#include "../application/uds.h"
taskInfo_t sched_canAbsRXMain ={
		canAbs_vidRxMain,inSuperLoopTask,activated,0,0
};

taskInfo_t sched_canTpRXMain ={
		canTp_vidRxMain,inSuperLoopTask,deActivated,0,0
};

taskInfo_t sched_canTpTXMain ={
		canTp_vidTxMain,periodicTask,deActivated,1,0
};

taskInfo_t sched_p2Event ={
		UDS_vidP2Event,periodicTask,deActivated,50000,0
};

taskInfo_t sched_p2StarEvent ={
		UDS_vidP2StartEvent,periodicTask,deActivated,5000000,0
};

taskInfo_t sched_sendindDids ={
		UDS_vidSendReadDids,periodicTask,activated,1000000,0
};

static taskInfo_t * sched_udtpArraysOfTask[SCHED_NUM_OF_TASKS]={
	&sched_canTpRXMain,&sched_canTpTXMain,&sched_p2Event,&sched_p2StarEvent,&sched_sendindDids
};

static bool_t udtIsTaskPeriodElapsed(taskInfo_t * udtPTask)
{

	uint32_t u32ElapsedTime;
	uint32_t u32timeStamp = timer_u32GetTimeStap();

	if(u32timeStamp >= udtPTask->u32StartTime)
	{
		u32ElapsedTime = u32timeStamp - udtPTask->u32StartTime;
	}
	else
	{
		u32ElapsedTime = 0xFFFFFFFF-udtPTask->u32StartTime+u32timeStamp+1;
	}
	return u32ElapsedTime>=udtPTask->u32Period;
}
void sched_vidRefreshStartTimeOfTask(taskInfo_t * udtPTask)
{
	udtPTask->u32StartTime = timer_u32GetTimeStap();
}
void sched_vidActivateTask(taskInfo_t * udtPTask)
{
	udtPTask->udtTaskStatus = activated;
	if(udtPTask->udtTaskType == periodicTask)
	{
		sched_vidRefreshStartTimeOfTask(udtPTask);
	}
}
void sched_vidSetPeriodOfTask(taskInfo_t * udtPTask,uint32_t u32NewPeriod)
{
	udtPTask->u32Period = u32NewPeriod;
}
void sched_vidDeactivateTask(taskInfo_t * udtPTask)
{
	udtPTask->udtTaskStatus = deActivated;
}
void sched_vidMain()
{
	uint8_t u8Index;
	for(u8Index=0;u8Index<SCHED_NUM_OF_TASKS;u8Index++)
	{
		if(sched_udtpArraysOfTask[u8Index]->udtTaskType == inSuperLoopTask &&
				sched_udtpArraysOfTask[u8Index]->udtTaskStatus == activated	)
		{
			sched_udtpArraysOfTask[u8Index]->ptrToTask();
		}
		else if(sched_udtpArraysOfTask[u8Index]->udtTaskType == inSuperLoopTask &&
				sched_udtpArraysOfTask[u8Index]->udtTaskStatus == periodicTask &&
				udtIsTaskPeriodElapsed(sched_udtpArraysOfTask[u8Index]))
		{
			sched_udtpArraysOfTask[u8Index]->ptrToTask();
			sched_vidRefreshStartTimeOfTask(sched_udtpArraysOfTask[u8Index]);
		}
	}
}
