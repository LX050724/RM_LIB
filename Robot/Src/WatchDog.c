#include <WatchDog.h>

extern volatile uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;

WatchDogp List[10];
uint8_t Len;

#ifdef WatchDogENABLE
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
	for(uint8_t i = 0;i < Len;i++)
	{
		List[i]->Life++;
		if(List[i]->Life > List[i]->Max)
		{
			WatchDog_CallBack(List[i]);
		}
	}
}
#endif

void WatchDog_Init(WatchDogp handle,uint32_t Life)
{
	handle->Max = Life;
	List[Len++] = handle;
}

inline void Feed_Dog(WatchDogp handle)
{
	handle->Life = 0;
	FeedDog_CallBack(handle);
}

__weak void FeedDog_CallBack(WatchDogp handle)
{
	UNUSED(handle);
}

__weak void WatchDog_CallBack(WatchDogp handle)
{
	UNUSED(handle);
}
