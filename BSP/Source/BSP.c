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
File    : BSP.c
Purpose : BSP for Segger emPower board
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "LPC17xx.h"    // Device specific header file, contains CMSIS
#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NUM_LEDS 8u


/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
int LED_State=0;

void BSP_Init(void) 
{
  LPC_GPIO0->FIODIR |= (1<<0); // P0.0 is Output pin
  LED_State=0;
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) 
{
  LPC_GPIO0->FIOCLR |= (1<<0);  
  LED_State=1;
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) 
{
  LPC_GPIO0->FIOSET |= (1<<0);
  LED_State=0;
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) 
{
  if(LED_State==0)
    BSP_SetLED(Index);
  else
    BSP_ClrLED(Index);
}

/****** End Of File *************************************************/
