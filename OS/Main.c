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
File    : Main.c
Purpose : Generic SEGGER application start
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"
#include "BSP.h"
#include "USB_HID.h"
#include "USB.h"
#include "BSP_USB.h"
#include "LPC17xx.h"
/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
extern "C" {    /* Make sure we have C-declarations in C++ programs */
#endif
void MainTask(void);
#ifdef __cplusplus
}
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static OS_STACKPTR int Stack0[2000];//768];                   /* Task stack */
static OS_TASK         TCB0;                  /* Task-control-block */

/*********************************************************************
*
*       main()
*
* Function description
*   Application entry point
*/


int main(void) 
{ 
  
  myUsbHarwareConnector();
    
  OS_InitKern();                   // Initialize OS                 
  OS_InitHW();                     // Initialize Hardware for OS   

  OS_CREATETASK(&TCB0, "MainTask", MainTask, 100, Stack0);
  OS_Start();                      /* Start multitasking            */
  return 0;
}

/****** End Of File *************************************************/
