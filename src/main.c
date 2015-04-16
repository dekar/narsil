/*
 * A skeleton main.c
 * Add your own code!
 */

// Load CMSIS and peripheral library and configuration
#include "stm32f2xx.h"
#include "lcd.h"
#include "uart_dma.h"
#include "stm32f2xx_gpio.h"
#include "usbd_usr.h"
#include "rtc.h"
#include "dac.h"
#include "adc_dma.h"

#include <string.h>
#include <stdlib.h>




#define LED_ON(a) GPIOB->BSRRH = 1 << (5+a)
#define LED_OFF(a) GPIOB->BSRRL = 1<<(5+a)


#define LED1 0
#define LED2 1

#define MAIN_BUF_BLOCKS 10
    uint8_t mainBuf[MAIN_BUF_BLOCKS * 512];
#define SINGLE_BLOCK_WRITE_ADDR 0
#define MULTY_BLOCK_WRITE_ADDR 2


void ledInit();
void NVIC_Config();

int main(void)
{
    SystemInit();
// Init LED
    ledInit();
    LED_ON(LED1);
    LED_ON(LED2);
// Init USART
    USART_DMA_Config();
    USART_DMA_SendStr("\r\nOK, let's start.\r\n");
// Init NVIC
    NVIC_Config();
// Init LCD
    USART_DMA_SendStr("Lcd\t\t");
    Lcd_Init();
    Lcd_SetLED(ENABLE);
    Lcd_PrintString("Gogogo\r");
    USART_DMA_SendStr("OK!\r\n");
// Init USB
    usb_init();
// Настройка RTC
    USART_DMA_SendStr("RTC up:\t\t");
    RTC_Config();
    RTC_TimeTypeDef RTC_TimeStructure;
// Настройка DAC
    USART_DMA_SendStr("DAC up:\t\t");
    DAC_Config();
    USART_DMA_SendStr("OK!\r\n");
// Настройка ADC
    USART_DMA_SendStr("ADC up:\t\t");
    ADC_DMA_Config(mainBuf,MAIN_BUF_BLOCKS*512);
    USART_DMA_SendStr("OK!\r\n");
// Настройка ReadLine

// End of initing
    LED_OFF(LED1);
    LED_OFF(LED2);

    spiConfig();

//Регистрирую функции
    //usbFlashConfig();
    uint16_t m;
    for(;;)
    {
        if(m==50)
        {
            LED_ON(0);
        }
        if (m==250)
        {
            LED_OFF(0);
            m=0;
        }
        m++;


        while(UsbUsart.count_out>0)
        {
            //New packet recive
            UsbUsart.buffer_out[UsbUsart.count_out]=0;
            switch(UsbUsart.buffer_out[0])
            {
                case 1:
                    {
                        int addr = *(int*)&(UsbUsart.buffer_out[1]);
                        *(uint8_t*)addr= UsbUsart.buffer_out[5]; // Записываем 1 байт по адресу
                    }
                    break;
                case 4:
                   {
                        int addr = *(int*)&(UsbUsart.buffer_out[1]);
                        *(int*)addr= *(int*)&(UsbUsart.buffer_out[5]); // Записываем 1 байт по адресу
                   }
                    break;
                case 16:
                   {
                        Lcd_PrintString("16");
                        int addr = *(int*)&(UsbUsart.buffer_out[1]);
                        int amount = UsbUsart.buffer_out[5];
                        uint8_t buf[amount];
                        uint8_t i;
                        for(i=0;i<amount;i++)
                        {
                            buf[i] = ((uint8_t*)addr)[i];
                        }

                        VCP_DataTx(buf,amount);
                   }
                    break;

                default:
                    break;
            }
            UsbUsart.count_out=0;
        }
    }
}



void ledInit()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Debug block
    GPIO_InitStructure.GPIO_Pin = 0b1111;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void NVIC_Config()
{
//Configure the NVIC Preemption Priority Bits
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

// SDIO
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

// USART1
    #ifdef USART_USE_INTERRUPT
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    #endif
// SPI2
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
