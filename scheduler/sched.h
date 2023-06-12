#ifndef SCHED_H
#define SCHED_H
#include "../helper/dataTypes.h"
#include "../timer/timer.h"

#define SCHED_NUM_OF_TASKS                  6

typedef enum{inSuperLoopTask,periodicTask} taskType_t;
typedef enum{deActivated,activated} taskStatus_t;

typedef struct{
	void (*ptrToTask)(void);
	taskType_t udtTaskType;
	taskStatus_t udtTaskStatus;
	uint32_t u32Period;
	uint32_t u32StartTime;
}taskInfo_t;

extern taskInfo_t sched_canAbsRXMain;

extern taskInfo_t sched_canTpRXMain;

extern taskInfo_t sched_canTpTXMain;

extern taskInfo_t sched_p2Event;

extern taskInfo_t sched_p2StarEvent;

extern taskInfo_t sched_sendindDids;

void sched_vidActivateTask(taskInfo_t * task);
void sched_vidRefreshStartTimeOfTask(taskInfo_t * task);
void sched_vidSetPeriodOfTask(taskInfo_t * task,uint32_t u32NewPeriod);
void sched_vidDeactivateTask(taskInfo_t * task);
void sched_vidMain();



#endif
