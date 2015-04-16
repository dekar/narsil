#include "stm32f2xx.h"

#ifndef ADCDMA_H_
#define ADCDMA_H_

/*
*   Configure DMA sender
*
*/


void ADC_DMA_Config (uint8_t* bufer,uint32_t bufSize);


#define MY_ADC_SAMPLING_TIME     ADC_SampleTime_28Cycles
#define MY_ADC_SAMPLING_TIME_NUM 28.5

#define MY_ADC_DIVISOR           RCC_PCLK2_Div6
//#define MY_ADC_DIVISOR_TIM       TIM_CKD_DIV4



#define MY_ADC_CPU_CICLES        (MY_ADC_SAMPLING_TIME_NUM + 12.5)*3



#define ADC1_DR_Address    ((uint32_t)0x4001204C)
#define GPIO_MY_ADC_PIN GPIO_Pin_1
#define GPIO_MY_ADC_PORT GPIOA
#define RCC_AHB1Periph_MY_ADC_GPIO RCC_AHB1Periph_GPIOA
#define MY_DMA_ADC_Channel ADC_Channel_1

#endif
