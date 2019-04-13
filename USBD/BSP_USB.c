/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2018     SEGGER Microcontroller GmbH              *
*                                                                    *
*       www.segger.com     Support: support_emusb@segger.com         *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Host * USB Host stack for embedded applications        *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product.                          *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Host version: V2.15-r14144                             *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : BSP_USB.c
Purpose : BSP for LPC54605_emUSB_Host board
          Functions for USB controllers
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP_USB.h"
#include "RTOS.h"
//#include "LPC54605.h"
#include "LPC17xx.h"
#include "SEGGER.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef void (ISR_HANDLER)(void);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static ISR_HANDLER * _pfUSB_ISRHandler;
//static ISR_HANDLER * _pfUSB1ISRHandler;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
#ifdef __cplusplus
extern "C" {
#endif
  void USB_IRQHandler(void);
  //void USB1_IRQHandler(void);
#ifdef __cplusplus
}
#endif

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
/*ISR_HANDLER * _pfUSB_ISRHandlerF(void)
{
  SEGGER_RTT_Write("fdsfsdfd");
}*/

/*********************************************************************
*
*       USB_IRQHandler
*/
void USB_IRQHandler(void) 
{
  OS_EnterInterrupt();

  if (_pfUSB_ISRHandler) 
  {
    _pfUSB_ISRHandler();
     //SEGGER_RTT_printf(0, "USB_IRQHandler\n");
  }
  OS_LeaveInterrupt();
}


/*********************************************************************
*
*       BSP_USB_InstallISR_Ex
*/
void BSP_USB_InstallISR_Ex(int ISRIndex, void (*pfISR)(void), int Prio) 
{
  (void)Prio;

  //SEGGER_RTT_printf(0, "BSP_USB_InstallISR_Ex\n");
  
  if (ISRIndex == USB_IRQn)
  {
    _pfUSB_ISRHandler = pfISR;
  }

  NVIC_SetPriority((IRQn_Type)ISRIndex, (1u << __NVIC_PRIO_BITS) - 2u);

  NVIC_EnableIRQ((IRQn_Type)ISRIndex);
}

/*********************************************************************
*
*       BSP_USB_Init
*/
void BSP_USB_Init(void) 
{
}

/****** End Of File *************************************************/
