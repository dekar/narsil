/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"
#include "usbd_ioreq.h"



/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"
#include "usbd_desc.h"

#include "usb_core.h"
#include "usbd_core.h"
#include "usb_dcd_int.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void usb_init(void)
{
  /* USB configuration */
  USBD_Init(&USB_OTG_dev,
        USB_OTG_FS_CORE_ID,
        &USR_desc,
        &USBD_CDC_cb,
        &USR_cb);

}



void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
	/* Reset SLEEPDEEP and SLEEPONEXIT bits */
	SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

	/* After wake-up from sleep mode, reconfigure the system clock */
	SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}


void OTG_FS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler (&USB_OTG_dev);
}



USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,


};



/**
* @brief  USBD_USR_Init
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{

}
/**
* @brief  USBD_USR_DeviceReset
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed )
{

}


/**
* @brief  USBD_USR_DeviceConfigured
*         Displays the message on LCD on device configuration Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConfigured (void)
{
}


/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConnected (void)
{
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceDisconnected (void)
{
}

/**
* @brief  USBD_USR_DeviceSuspended
*         Displays the message on LCD on device suspend Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_DeviceResumed
*         Displays the message on LCD on device resume Event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
  /* Users can do their application actions here for the USB-Reset */
}



struct usartFlags UsbUsart;
extern tINBufer INBufer;

/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len)
{
	uint32_t i;
	//loop through buffer
	for( i = 0; i < Len; i++ )
	{
		INBufer.Buffer[INBufer.len] = (uint8_t) Buf[i];// буфер APP_Rx_Buffer используется драйвером USB
		//increase pointer value
		INBufer.len++;
		/* To avoid buffer overflow */
		if(INBufer.len == APP_RX_DATA_SIZE)
		{
			INBufer.len = 0;
		}
	}

	return USBD_OK;
}
void VCP_DataSendStr (const char * Buf)
{
    while(*(Buf))
    {
		INBufer.Buffer[INBufer.len] = (uint8_t) *Buf++;// буфер APP_Rx_Buffer используется драйвером USB
		//increase pointer value
		INBufer.len++;
		/* To avoid buffer overflow */
		if(INBufer.len == APP_RX_DATA_SIZE)
		{
			INBufer.len = 0;
		}
    }
}



uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	uint32_t i;
	for (i = 0; i < Len; i++)
	{
        UsbUsart.buffer_out[i] = *(Buf + i);
	}
	UsbUsart.count_out += Len;
	return USBD_OK;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/






























