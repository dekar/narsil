#include "spi.h"


void spiConfig()
{
    // Порты - PORTB
    // SPI2

    //  пин    Названия     SPI    GPIO
    //-----------------------------------
    //  15   SER(14)[ds]    MOSI
    //  14   SCL(10)[mr]           Строб
    //  13   SCK(11)[shcp]  SCK
    //  12   RCK(12)[stcp]         CS

  // Тактирование
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  // Настройка GPIO
	GPIO_InitTypeDef	GPIO_InitStructure;
     // Порты SPI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

     // Порты GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15 , GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13 , GPIO_AF_SPI2);


  // Настройка SPI
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
    SPI_Init(SPI2, &SPI_InitStruct);
  //
    SPI_Cmd(SPI2, ENABLE);


    SPI_I2S_ITConfig(SPI2,SPI_IT_RXNE,ENABLE);
}

void SPI2_IRQHandler(void)
{

    if (SPI_GetITStatus(SPI2,SPI_IT_RXNE))
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_14);//Поднимаю строб защёлки
        uint16_t tmp = SPI_ReceiveData(SPI2);
        GPIO_ResetBits(GPIOB,GPIO_Pin_12);//Сбрасываю CS
        GPIO_ResetBits(GPIOB,GPIO_Pin_14);//Сбрасываю строб защёлки
    }
}
