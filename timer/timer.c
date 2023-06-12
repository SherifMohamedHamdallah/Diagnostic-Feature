#include "timer.h"
#include "../debug.h"

#if DEBUG_IS_ENABLED
	uint32_t timer_u32CurrentTime;
#endif

uint32_t timer_u32GetTimeStap()
{
#if DEBUG_IS_ENABLED
	return timer_u32CurrentTime;
#endif
}
