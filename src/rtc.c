#include "stm32f2xx.h"
#include "rtc.h"

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{


    // Enable the PWR clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // Allow access to RTC
    PWR_BackupAccessCmd(ENABLE);


    // Enable the LSE OSC
    RCC_LSEConfig(RCC_LSE_ON);

    // Wait till LSE is ready
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    // Select the RTC Clock Source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // Enable the RTC Clock
    RCC_RTCCLKCmd(ENABLE);


    // Wait for RTC APB registers synchronisation
    RTC_WaitForSynchro();

    RTC_InitTypeDef RTC_InitStructure;

    RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
    RTC_InitStructure.RTC_SynchPrediv = 0xff;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {

    }
}

uint8_t timeSet(RTC_TimeTypeDef* time)
{
    // Enable the LSE OSC
    RCC_LSEConfig(RCC_LSE_ON);

    // Wait till LSE is ready
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    // Select the RTC Clock Source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /*  SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;*/

    // Enable the RTC Clock
    RCC_RTCCLKCmd(ENABLE);

    // Wait for RTC APB registers synchronisation
    RTC_WaitForSynchro();

    if(RTC_SetTime(RTC_Format_BIN, time) != ERROR)
        return 0;
    return 1;
}
