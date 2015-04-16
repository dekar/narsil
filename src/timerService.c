#include "timerService.h"
#include <time.h>

#ifdef HEAP_SIZE
    extern uint8_t heap[HEAP_SIZE];
#endif

/******************************************************************
    Приватные структуры и прототипы
******************************************************************/



/******************************************************************
    Функция инициализации таймерной службы
******************************************************************/
void TS_Config(void (*secondTick)(void),void (*minuteTick)(void))
{
  //Инициализация переменных
    timeSrvs.tasksNum = 0;
    timeSrvs.secondTick = secondTick;
    timeSrvs.minuteTick = minuteTick;
  //Инициализация таймера.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;

#ifdef HEAP_SIZE
    #define TIM_TimeBaseStructure (TIM_TimeBaseInitTypeDef*)heap
#else
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure_m;
    #define TIM_TimeBaseStructure (&TIM_TimeBaseStructure_m)
#endif

    TIM_TimeBaseStructure->TIM_ClockDivision = 0;
    TIM_TimeBaseStructure->TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure->TIM_Period = 0xffffffff;
    TIM_TimeBaseStructure->TIM_Prescaler = 59999;// 60MHz / (pres+1) Пример: 59999 - 1kHz

    TIM_TimeBaseInit(TIM2, TIM_TimeBaseStructure);
    TIM_SetCompare3(TIM2,TS_MAX_COUNTER+1);

  //Запуск таймера синхронно с тиком секундного таймера RTC
    uint16_t old_tr = RTC->TR;
    while (RTC->TR == old_tr);
    TIM_Cmd(TIM2, ENABLE);
    RTC_GetTime(RTC_Format_BIN, &timeSrvs.currentTime);
    TIM_SetCompare1(TIM2, 1000);
    TIM_SetCompare2(TIM2, 60000 - 1000*timeSrvs.currentTime.RTC_Seconds);
    TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2,ENABLE);

    RTC_DateTypeDef currentDate;
    RTC_GetDate(RTC_Format_BIN, &currentDate);

    struct tm tm;
    tm.tm_sec   = timeSrvs.currentTime.RTC_Seconds;
    tm.tm_min   = timeSrvs.currentTime.RTC_Minutes;
    tm.tm_hour  = timeSrvs.currentTime.RTC_Hours;
    tm.tm_mday  = currentDate.RTC_Date;
    tm.tm_mon   = currentDate.RTC_Month -1;
    tm.tm_year  = 100 + currentDate.RTC_Year;
    tm.tm_wday  = currentDate.RTC_WeekDay;

    timeSrvs.timestamp = mktime(&tm);
}

/****************************************************************
    Добавление задачи
*****************************************************************/
void TS_addTask(uint32_t mtime, void (*function)(uint32_t),uint32_t argument)
{
  // Отключаем срабатывание прерывания
    __disable_irq();

    if(timeSrvs.tasksNum > 0)
    {
        //считаем время до срабатывания текущей задачи
        timeSrvs.tasks[timeSrvs.tasksNum-1].mtime = TIM2->CCR3 - TIM2->CNT;

        uint32_t mtime0 = 0;
        uint8_t taskNum = timeSrvs.tasksNum;
        while (taskNum >0)
        {
            if(mtime0 + timeSrvs.tasks[taskNum-1].mtime > mtime)
            {
                timeSrvs.tasks[taskNum-1].mtime -= mtime - mtime0;
                break;
            }
            mtime0 += timeSrvs.tasks[taskNum-1].mtime;
            taskNum--;
        }
        mtime -= mtime0;
        timeSrvs.tasks[timeSrvs.tasksNum].function = function;
        timeSrvs.tasks[timeSrvs.tasksNum].mtime = mtime;
        timeSrvs.tasks[timeSrvs.tasksNum].argument = argument;

        while(taskNum<timeSrvs.tasksNum)
        {
            TS_Task_t unsortedTask = timeSrvs.tasks[taskNum];
            timeSrvs.tasks[taskNum] = timeSrvs.tasks[timeSrvs.tasksNum];
            timeSrvs.tasks[timeSrvs.tasksNum] = unsortedTask;
            taskNum++;
        }
        TIM2->CCR3 = TIM2->CNT + timeSrvs.tasks[timeSrvs.tasksNum].mtime;
        timeSrvs.tasksNum++;
    }
    else
    {
        timeSrvs.tasks[timeSrvs.tasksNum].function = function;
        timeSrvs.tasks[timeSrvs.tasksNum].mtime = mtime;
        timeSrvs.tasks[timeSrvs.tasksNum].argument = argument;
        timeSrvs.tasksNum++;
        TIM2->CCR3 = mtime + TIM2->CNT;
        TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
    }
    __enable_irq();
}
/****************************************************************
    Рабочее прерывание
*****************************************************************/

/*

    void (*secondTick)(void); // Функция, вызываемая каждую секунду
    void (*minuteTick)(void); // Функция, вызываемая каждую секунду*/

void TIM2_IRQHandler(void)
{
    TIM_Cmd(TIM2, DISABLE);
    if (TIM2->CNT == 0)
        USART_DMA_SendStr("Mimimi");
  // Секундное прерывание
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        timeSrvs.timestamp++;
        if(timeSrvs.currentTime.RTC_Seconds == 59)
            timeSrvs.currentTime.RTC_Seconds =0;
        else
            timeSrvs.currentTime.RTC_Seconds++;
        TIM2->CCR1+=1000;
        if(timeSrvs.secondTick)
            timeSrvs.secondTick();
    }
  // Минутное прерывание
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
      // Работаем с внутренними часами
        uint8_t tmp = timeSrvs.currentTime.RTC_Minutes+1;
        if(tmp >= 60)
        {
            tmp -= 60;
            timeSrvs.currentTime.RTC_Hours++;
            if (timeSrvs.currentTime.RTC_Hours >23)
                timeSrvs.currentTime.RTC_Hours=0;
            timeSrvs.currentTime.RTC_Minutes = tmp;
        }
        timeSrvs.currentTime.RTC_Minutes = tmp;
        TIM2->CCR2+=60000;
        if(timeSrvs.minuteTick)
            timeSrvs.minuteTick();
    }
  // Прерывание таймерной службы
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        TS_Task_t currentTask = timeSrvs.tasks[timeSrvs.tasksNum -1];
        currentTask.function(currentTask.argument);
        if(timeSrvs.tasksNum > 1)
        {
            timeSrvs.tasksNum--;
            TIM2->CCR3 += timeSrvs.tasks[timeSrvs.tasksNum-1].mtime;
        }
        else
        {
            timeSrvs.tasksNum = 0;
            TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
        }
    }
    TIM_Cmd(TIM2, ENABLE);
}
