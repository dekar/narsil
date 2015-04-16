#include "stm32f2xx.h"

#ifndef UARTDMA_H_
#define UARTDMA_H_

/*
*   Configure DMA sender
*
*/

#define MY_BAUDRATE 57600
//#define MY_BAUDRATE 460800
#define USART_USE_INTERRUPT
#define MY_USART1 //Set up UART1
//#define MY_UART_Master_Prescaler UART_BaudRatePrescaler_256 // Enable Master mode, and set prescaler







void USART_Config(void);
void USART_DMA_Config(void);
void USART_DMA_Send(uint32_t bufAddr, uint16_t size);
void USART_DMA_SendStr (const char* buf);
void USART_DMA_wait_send(void);


#ifdef MY_USART1

    #define RCC_MY_USART_GPIO_ENABLE  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)

    #define MY_USART_GPIO GPIOA
    #define GPIO_USART_Pins_In GPIO_Pin_10
    #define GPIO_USART_Pins_Out GPIO_Pin_9

    #define GPIO_USART_PinSource_Out GPIO_PinSource9
    #define GPIO_USART_PinSource_In GPIO_PinSource10
    #define GPIO_USART_AF GPIO_AF_USART1

    #define MY_USART USART1

    #define RCC_MY_USART_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,    ENABLE)
    #define RCC_MY_DMA_ENABLE  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,      ENABLE)

    #define MY_USART_DMA_TX           DMA2_Stream7
    #define MY_USART_DMA_TX_FLG       DMA_FLAG_FEIF4
    #define MY_USART_DMA_CHAN_TX      DMA_Channel_4
    #define MY_USART_DMA_RX           DMA2_Stream5
    //#define MY_USART_DMA_RX_FLG
    #define MY_USART_DMA_CHAN_RX      DMA2_Channel2


#elif defined MY_UART2
    #define RCC_APB2Periph_MY_UART_GPIO RCC_APB2Periph_GPIOA
    #define MY_UART_GPIO GPIOA
    #define GPIO_UART_Pis_In GPIO_Pin_3
    #define GPIO_UART_Pis_Out GPIO_Pin_2

    #define RCC_APB2Periph_MY_UART_GPIO RCC_APB2Periph_GPIOD
    #define MY_UART_GPIO GPIOD
    #define GPIO_UART_Pis_In GPIO_Pin_6
    #define GPIO_UART_Pis_Out GPIO_Pin_5



    #define MY_UART USART2
    #define RCC_MY_UART_ENABLE RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)

    #define RCC_AHBPeriph_MY_UART_DMA  RCC_AHBPeriph_DMA1
    #define MY_UART_DMA_CHAN_TX        DMA1_Channel7
    #define MY_UART_DMA_CHAN_TX_FLG    DMA1_FLAG_TC7
    #define MY_UART_DMA_CHAN_RX        DMA1_Channel6
    #define MY_UART_DMA_CHAN_RX_FLG    DMA1_FLAG_TC6


    #error NO UART DEFINED
#endif


#endif
