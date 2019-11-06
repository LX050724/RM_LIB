#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_
#include <stm32f4xx.h>

typedef struct WatchDog
{
	uint32_t Life;
	uint32_t Max;
}WatchDog_TypeDef,*WatchDogp;

#define IS_Dog(handle,Dog) (handle) == &(Dog)

void WatchDog_CallBack(WatchDogp handle);
void FeedDog_CallBack(WatchDogp handle);
void Feed_Dog(WatchDogp handle);
void WatchDog_Init(WatchDogp handle,uint32_t Life);

#endif
