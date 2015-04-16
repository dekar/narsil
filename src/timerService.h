#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H
#include "stm32f2xx.h"

#define TS_USEC (TIM2->CNT + 1000 - TIM2->CCR1)

#define TS_MAX_COUNTER 3600000

typedef struct
{
    uint32_t mtime;
    void (*function)(uint32_t);
    uint32_t argument;
}TS_Task_t;


struct
{
    TS_Task_t tasks[20]; // Сортированные задачи
    uint8_t tasksNum;
    void (*secondTick)(void); // Функция, вызываемая каждую секунду
    void (*minuteTick)(void); // Функция, вызываемая каждую секунду
    uint32_t timestamp;
    RTC_TimeTypeDef currentTime;
} timeSrvs;


void TS_Config(void (*secondTick)(void),void (*minuteTick)(void));
void TS_addTask(uint32_t time, void (*function)(uint32_t), uint32_t argument);
uint32_t TS_getTimeStamp(RTC_TimeTypeDef *currentTime);



#endif
