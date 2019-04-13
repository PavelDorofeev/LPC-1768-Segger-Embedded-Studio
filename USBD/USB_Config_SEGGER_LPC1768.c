/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2017  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : USB_Config_SEGGER_emPower.c
Purpose : Config file for SEGGER's emPower eval board.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "USB.h"
#include "BSP_USB.h"
#include "LPC17xx.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define ALLOC_SIZE               0xF000

//#define USBOTG_BASE           0x400A1000
//#define USB_DEV_OTGSC         *((volatile U32*)(USBOTG_BASE +0x1A4))

#define USB_ISR_PRIO  (254)

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       _HWAttach
*/
static void _HWAttach(void) 
{
  //
  // Uncomment the line to enable VBUS charge. This allows to enable the USB PHY
  // without connecting the VBUS pin to the real VBUS of USB.
  //
  //USB_DEV_OTGSC |= (1 << 1);
  //SEGGER_RTT_printf(0, "_HWAttach\n");
}

/*********************************************************************
*
*       _EnableISR
*/
static void _EnableISR(USB_ISR_HANDLER * pfISRHandler) 
{
  //BSP_USB_InstallISR_Ex((U32)USBHS_IRQn, pfISRHandler, USB_ISR_PRIO);
  //SEGGER_RTT_printf(0, "_EnableISR\n");
  BSP_USB_InstallISR_Ex((U32)USB_IRQn, pfISRHandler, USB_ISR_PRIO);
  
}

/*********************************************************************
*
*       USBD_X_Config
*
*  Function description
*   Initialize the USB HS controller.
*
*  Notes
*    (1) Change priorities for the flash slave module:
*        Leaving it in the default configuration which is
*           M0 (ARM core code bus)    -> 0 (highest priority)
*           M1 (ARM core system bus)  -> 1
*           M2 (DMA/EzPort)           -> 2
*           M3 (Ethernet)             -> 3
*           M4 (USB FS/LS OTG)        -> 4
*           M5 (SDHC)                 -> 5
*           M6 (USB HS/FS/LS OTG)     -> 6 (lowest priority)
*         access to the flash from the USB FS Controller is stalled
*         which result in sending/retrieving less data than expected
*         which results in data transmissions error (mostly CRC checksum error).
*
*         We will give other DMA masters a higher priority otherwise it is
*         not guaranteed that they will work properly as described above.
*
*           M0 (ARM core code bus)    -> 5
*           M1 (ARM core system bus)  -> 6 (lowest priority)
*           M2 (DMA/EzPort)           -> 4
*           M3 (Ethernet)             -> 3
*           M4 (USB FS/LS OTG)        -> 2
*           M5 (SDHC)                 -> 1
*           M6 (USB HS/FS/LS OTG)     -> 0 (highest priority)
*    (2) The current limit function must be disabled before
*        beginning USB data communication on either of the USB0
*        or USB1 ports, and should remain disabled as long as
*        the USB ports are in use.
*        This is also described in "Mask Set Errata for Mask 0N65N, Rev. 17Nov2015" as e9359:
*          "USBReg: In some conditions when using both VREG_INn inputs, voltage regulator
*                   current limit can fall below specification"
*/

char _MemPool[1024];

void USBD_X_Config(void) 
////This function is always called from USBD_Init().
{
  usbSettings();
  USBD_SetISRMgmFuncs(_EnableISR,USB_OS_IncDI, USB_OS_DecRI);
  USBD_SetAttachFunc(_HWAttach);
}


/*********************************************************************
*
*       USBD_X_EnableInterrupt
*/
void USBD_X_EnableInterrupt(void) 
{
  //SEGGER_RTT_printf(0, "USBD_X_EnableInterrupt\n");
  NVIC_EnableIRQ(USB_IRQn);//USBActivity_IRQn);//USBHS_IRQn);
}

/*********************************************************************
*
*       USBD_X_DisableInterrupt
*/
void USBD_X_DisableInterrupt(void) 
{
  //NVIC_DisableIRQ(USBHS_IRQn);
  //SEGGER_RTT_printf(0, "USBD_X_DisableInterrupt\n");
  NVIC_DisableIRQ(USB_IRQn);//USBActivity_IRQn);
  
}

/*************************** End of file ****************************/
