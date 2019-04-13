
#include <stdio.h>
///////////#include "USB_Audio.h"
#include "RTOS.h"
//////#include "FS.h"
#include "USB.h"
#include "BSP.h"
#include "SEGGER_RTT.h"

/*********************************************************************
*
*       MainTask
*
* Function description
*   USB handling task.
*/

/*

#ifdef __cplusplus
extern "C" {    
#endif
void MainTask(void);
#ifdef __cplusplus
}
#endif

static const USB_DEVICE_INFO _DeviceInfo = {
  0x8765,         // VendorId
  0x1312,         // ProductId
  "SEGGER",       // VendorName
  "Test USB",// ProductName
  "13245678"      // SerialNumber
};

void MainTask(void) 
{
  USBD_Init();
  USBD_SetDeviceInfo(&_DeviceInfo);
  USBD_Start();

  //BSP_SetLED(0);
  static char s[1024]={'M','a'};
  while (1) 
  {
    // Wait for configuration

    //while ((USBD_GetState() & (USB_STAT_CONFIGURED | USB_STAT_SUSPENDED)) != USB_STAT_CONFIGURED) {

      BSP_SetLED(0);
      OS_Delay(1000);
      BSP_ClrLED(0);
      OS_Delay(300);
      
      //SEGGER_snprintf(s, sizeof(s), "Written 0x%x bytes in %u usec\n", 2, 33);
      //unsigned char aa[]="sdfdsf";
      SEGGER_RTT_Write(0, &s[0], 7);
  }
}
*/
/**************************** end of file ***************************/
