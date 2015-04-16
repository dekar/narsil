#include "uart_dma.h"
#include "stm32f2xx.h"



#define USART_DR_Base(USART) ((int)USART + 0x04)


__INLINE void USART_DMA_Config (void)
{
    RCC_MY_DMA_ENABLE;

    USART_Config();
    USART_DMACmd(MY_USART,USART_DMAReq_Tx,ENABLE);
    DMA_Cmd(MY_USART_DMA_TX, ENABLE);
}
__INLINE void USART_DMA_SendStr (const char* buf)
{
    uint8_t i;
    for (i =0;i<255;i++)
        if (buf[i] == 0)
            break;
    USART_DMA_Send ((uint32_t) buf,i);
}

__INLINE void USART_DMA_Send (uint32_t bufAddr, uint16_t size)
{
    USART_DMA_wait_send();
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(MY_USART_DMA_TX);
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_Base(MY_USART);
    DMA_InitStructure.DMA_Channel = MY_USART_DMA_CHAN_TX;
    DMA_InitStructure.DMA_Memory0BaseAddr = bufAddr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(MY_USART_DMA_TX, &DMA_InitStructure);
#ifdef USART_USE_INTERRUPT

    DMA_ITConfig(MY_USART_DMA_TX, DMA_IT_TCIF4,ENABLE);

#endif
    DMA_Cmd(MY_USART_DMA_TX, ENABLE);
}

__INLINE void USART_DMA_wait_send(void)
{
    //uint16_t dmaCnt = DMA_GetCurrDataCounter(MY_USART_DMA_TX);
    while (DMA_GetCurrDataCounter(MY_USART_DMA_TX) >0);
/*    if (dmaCnt >0)
    {

        while (DMA_GetFlagStatus(MY_USART_DMA_TX, DMA_FLAG_TCIF4 )==RESET)
            dmaCnt = DMA_GetCurrDataCounter(MY_USART_DMA_TX);
        DMA_ClearFlag(MY_USART_DMA_TX, MY_USART_DMA_TX_FLG);
    }*/

}
__INLINE void USART_Config()
{
  // Тактирование
    RCC_MY_USART_ENABLE;
    RCC_MY_USART_GPIO_ENABLE;

  // Настройка GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
     // Порты вывода
    GPIO_InitStructure.GPIO_Pin = GPIO_USART_Pins_Out |GPIO_USART_Pins_In ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    GPIO_Init(MY_USART_GPIO, &GPIO_InitStructure);

    GPIO_PinAFConfig(MY_USART_GPIO, GPIO_USART_PinSource_Out, GPIO_USART_AF);
    GPIO_PinAFConfig(MY_USART_GPIO, GPIO_USART_PinSource_In, GPIO_USART_AF);

  // Настройка USART
    USART_InitTypeDef USART_InitStructure;

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = MY_BAUDRATE;
    USART_InitStructure.USART_HardwareFlowControl = DISABLE;
    USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;

    USART_DeInit(MY_USART);
    USART_Init(MY_USART, &USART_InitStructure);

#ifdef USART_USE_INTERRUPT
    USART_ITConfig(MY_USART,USART_IT_RXNE,ENABLE);
#endif

  // Включение периферии
    USART_Cmd(MY_USART,ENABLE);
}
