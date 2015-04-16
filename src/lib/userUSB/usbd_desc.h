/**
  ******************************************************************************
  * @file    usbd_desc.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   header file for the usbd_desc.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USB_DESC
  * @brief general defines for the usb device library file
  * @{
  */

/** @defgroup USB_DESC_Exported_Defines
  * @{
  */
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define USB_SIZ_DEVICE_DESC                     18
#define USB_SIZ_STRING_LANGID                   4

/**
  * @}
  */


/** @defgroup USBD_DESC_Exported_TypesDefinitions
  * @{
  */
/**
  * @}
  */



/** @defgroup USBD_DESC_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_DESC_Exported_Variables
  * @{
  */
extern const uint8_t USBD_DeviceDesc  [USB_SIZ_DEVICE_DESC];
extern  uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ];
extern  uint8_t USBD_OtherSpeedCfgDesc[USB_LEN_CFG_DESC];
extern  uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];
extern  uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID];
extern  USBD_DEVICE USR_desc;
/**
  * @}
  */

/** @defgroup USBD_DESC_Exported_FunctionsPrototype
  * @{
  */


uint8_t *     USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_ManufacturerStrDescriptor ( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_ProductStrDescriptor ( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t *     USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length);

#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t *     USBD_USR_USRStringDesc (uint8_t speed, uint8_t idx , uint16_t *length);
#endif /* USB_SUPPORT_USER_STRING_DESC */

/**
  * @}
  */

#endif /* __USBD_DESC_H */


/**
  ******************************************************************************
  * @file    usbd_cdc_core.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   header file for the usbd_cdc_core.c file.
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

#ifndef __USB_CDC_CORE_H_
#define __USB_CDC_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_cdc
  * @brief This file is the Header file for USBD_cdc.c
  * @{
  */


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */
#define USB_CDC_CONFIG_DESC_SIZ                (18+14)
//#define USB_CDC_DESC_SIZ                       (67-9)

//#define CDC_DESCRIPTOR_TYPE                     0x21

/*#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00*/


#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#define CDC_DATA_IN_PACKET_SIZE                 128

#define CDC_DATA_OUT_PACKET_SIZE                128

/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/

/**************************************************/
/* CDC Requests                                   */
/**************************************************/
#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct _CDC_IF_PROP
{
  uint16_t (*pIf_Init)     (void);
  uint16_t (*pIf_DeInit)   (void);
  uint16_t (*pIf_Ctrl)     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
  uint16_t (*pIf_DataTx)   (uint8_t* Buf, uint32_t Len);
  uint16_t (*pIf_DataRx)   (uint8_t* Buf, uint32_t Len);
}
CDC_IF_Prop_TypeDef;

__ALIGN_BEGIN typedef struct
{
 uint8_t Buffer   [APP_RX_DATA_SIZE]  ;
    uint16_t len;
} __ALIGN_END tINBufer;
/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_Class_cb_TypeDef  USBD_CDC_cb;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/**
  * @}
  */

#endif  // __USB_CDC_CORE_H_
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/**
  * @}
  */

/**
* @}
*/
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
