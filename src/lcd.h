/*
 * lcd.h
 *
 *  Created on: May 2, 2010
 *      Author: dekar
 */

#ifndef LCD_H_
#define LCD_H_
#include "stm32f2xx.h"
//
    #define GPIO_LCD     GPIOB //
    #define RCC_AHB1Periph_GPIO_LCD RCC_AHB1Periph_GPIOB
    #define PIN_SCLK     GPIO_Pin_11 //SCK 14
    #define PIN_SDA      GPIO_Pin_10 //TX 15
    #define PIN_CS       GPIO_Pin_9 //OTH_1 12
    #define PIN_RST      GPIO_Pin_8 //OTH_2 13
    #define PIN_LED      GPIO_Pin_2


//

	void Lcd_Clear(void);
	void Lcd_Init(void);
	void Lcd_HWInit(void);
	void Lcd_Write(char cd,unsigned char c);
	void Lcd_SetX(char x);
	void Lcd_SetY(char y);
	void Lcd_Gotoxy(char x,char y);
	void Lcd_VLine(u8 x,u8 y/*,char on*/);
	void Lcd_Line(unsigned char x,unsigned char y,unsigned char y2/*,unsigned char on*/);
	void Lcd_PrintChar(u8 c);
	void Lcd_PrintString(const char * message);
    void Lcd_Return();
    void Lcd_PrintInt (u16 c);
    void Lcd_SetLED(FunctionalState state);
    u8 Lcd_x;

#endif
