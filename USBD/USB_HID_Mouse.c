/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2019     SEGGER Microcontroller GmbH              *
*                                                                    *
*       www.segger.com     Support: www.segger.com/ticket            *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Device * USB Device stack for embedded applications    *
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
*       emUSB-Device version: V3.18b                                 *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : USB_HID_Mouse.c
Purpose : Demonstrates usage of the HID component as a mouse.

Additional information:
  Preparations:
    None.

  Expected behavior:
    The mouse cursor constantly jumps from left to right and back.

  Sample output:
    The target side does not produce terminal output.
*/

/*********************************************************************
*
*       #include section
*
**********************************************************************
*/
#include <string.h>
#include "USB.h"
#include "USB_HID.h"
#include "BSP.h"

/*********************************************************************
*
*       Static const data
*
**********************************************************************
*/
//
//  Information that is used during enumeration.
//
static const USB_DEVICE_INFO _DeviceInfo = {
  0x8765,         // VendorId
  0x1112,         // ProductId
  "Vendor",       // VendorName
  "HID mouse sample",  // ProductName
  "12345678"      // SerialNumber
};

/*********************************************************************
*
*       _aHIDReport
*
*  This report is generated according to HID spec and
*  HID Usage Tables specifications.
*/
const U8 _aHIDReport[] = 
{
  USB_HID_GLOBAL_USAGE_PAGE + 1, USB_HID_USAGE_PAGE_GENERIC_DESKTOP,
  USB_HID_LOCAL_USAGE + 1, USB_HID_USAGE_MOUSE,
  USB_HID_MAIN_COLLECTION + 1, USB_HID_COLLECTION_APPLICATION,
    USB_HID_LOCAL_USAGE + 1, USB_HID_USAGE_POINTER,
    USB_HID_MAIN_COLLECTION + 1, USB_HID_COLLECTION_PHYSICAL,
      USB_HID_GLOBAL_USAGE_PAGE + 1, USB_HID_USAGE_PAGE_BUTTON,
      USB_HID_LOCAL_USAGE_MINIMUM + 1, 1,
      USB_HID_LOCAL_USAGE_MAXIMUM + 1, 3,
      USB_HID_GLOBAL_LOGICAL_MINIMUM + 1, 0,
      USB_HID_GLOBAL_LOGICAL_MAXIMUM + 1, 1,
      USB_HID_GLOBAL_REPORT_COUNT + 1, 3,
      USB_HID_GLOBAL_REPORT_SIZE + 1, 1,
      USB_HID_MAIN_INPUT + 1, USB_HID_VARIABLE,  // 3 button bits
      USB_HID_GLOBAL_REPORT_COUNT + 1, 1,
      USB_HID_GLOBAL_REPORT_SIZE + 1, 5,
      USB_HID_MAIN_INPUT + 1, USB_HID_CONSTANT,  // 5 bit padding
      USB_HID_GLOBAL_USAGE_PAGE + 1, USB_HID_USAGE_PAGE_GENERIC_DESKTOP,
      USB_HID_LOCAL_USAGE + 1, USB_HID_USAGE_X,
      USB_HID_LOCAL_USAGE + 1, USB_HID_USAGE_Y,
      USB_HID_GLOBAL_LOGICAL_MINIMUM + 1, (unsigned char) -127,
      USB_HID_GLOBAL_LOGICAL_MAXIMUM + 1, 127,
      USB_HID_GLOBAL_REPORT_SIZE + 1, 8,
      USB_HID_GLOBAL_REPORT_COUNT + 1, 2,
      USB_HID_MAIN_INPUT + 1, USB_HID_VARIABLE | USB_HID_RELATIVE,
    USB_HID_MAIN_ENDCOLLECTION,
  USB_HID_MAIN_ENDCOLLECTION
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static U8 _IntervalTime;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _AddHID
*
*  Function description
*    Add HID mouse class to USB stack
*/
static USB_HID_HANDLE _AddHID(void) 
{
  USB_HID_INIT_DATA InitData;
  USB_HID_HANDLE    hInst;

  SEGGER_RTT_printf(0, "_AddHID\n");

  memset(&InitData, 0, sizeof(InitData));

  SEGGER_RTT_printf(0, "USBD_AddEP...\n");
  InitData.EPIn    = USBD_AddEP(USB_DIR_IN,  USB_TRANSFER_TYPE_INT, _IntervalTime, NULL, 0);
  InitData.pReport = _aHIDReport;
  InitData.NumBytesReport = sizeof(_aHIDReport);
  
  SEGGER_RTT_printf(0, "USBD_SetDeviceInfo\n");
  USBD_SetDeviceInfo(&_DeviceInfo);
  
  SEGGER_RTT_printf(0, "USBD_HID_Add\n");
  hInst = USBD_HID_Add(&InitData);
  return hInst;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       MainTask
*
* USB handling task.
*   Modify to implement the desired protocol
*/
#ifdef __cplusplus
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
void MainTask(void);
#ifdef __cplusplus
}
#endif
void MainTask(void) 
{
  U8             ac[3]={'a','b','c'};
  USB_HID_HANDLE hInst;


  _IntervalTime = 64;   // We set a interval time of 8 ms (64 micro frames (64 * 125us = 8ms))
  USBD_Init();
  hInst = _AddHID();

  USBD_Start(); 
  
  int stt2=0;

  while (1) 
  {
    int stt;
    
    while (((stt=USBD_GetState()) & (USB_STAT_CONFIGURED | USB_STAT_SUSPENDED)) != USB_STAT_CONFIGURED) 
    {
     /* if(stt2!=stt)
        SEGGER_RTT_printf(0, "stt=0x%x\n",stt);
      stt2=stt;*/

      BSP_ToggleLED(0);
      USB_OS_Delay(50);
    }

    BSP_SetLED(0);
    memset (ac, 0, sizeof(ac));
    ac[1] = 20;   // To the left !
    USBD_HID_Write(hInst, &ac[0], 3, 0);      // Make sure we send the number of bytes defined in REPORT
    USB_OS_Delay(500);
    ac[1] = (U8)-20;  // To the right !
    USBD_HID_Write(hInst, &ac[0], 3, 0);      // Make sure we send the number of bytes defined in REPORT
    USB_OS_Delay(100);

  }
}
/*
The USB specification defines six device states. 
During enumeration, a device moves through four of the states: 
powered, default, address, and configured. 
(The other states are attached and suspend.)

Attached: The device can be in an attached or detached stat; the specification only defines
the attached stat.

Powered: A USB device must be able to be addressed within a specified time period from
when power is initially applied. After an attachment to a port has been detected,
the host may enable the port, which will also reset the device attached to the port.
The host will provide up to 100 ma at this point.

Default: After the device has been powered, it must not respond to any bus transactions
until it has received a reset from the bus. After receiving a reset, the device is then
addressable at the default address (0).

Address: All USB devices use the default address when initially powered or after the device
has been reset. Each USB device is assigned a unique address by the host after
attachment or after reset. A USB device maintains its assigned address while
suspended. A USB device responds to requests on its default pipe whether the
device is currently assigned a unique address or is using the default address.

Configured: Before a USB device’s function may be used, the device must be configured.
Configuration involves correctly processing a SetConfiguration() request with a
non-zero configuration value. Configuring a device or changing an alternate
setting causes all of the status and configuration values associated with endpoints
in the affected interfaces to be set to their default values. This includes setting the
data toggle of any endpoint using data toggles to the value DATA0.

Suspended: To conserve power, USB devices automatically enter the suspended state when the
device has observed no bus traffic for a specified period (refer to Section 7, “The
USB Device-Side Driver,”). When suspended, the USB device maintains any
internal status, including its address and configuration. All devices must suspend
if bus activity has not been observed for the length of time specified in the USB
specification. Attached devices must be prepared to suspend at any time they are
powered, whether they have been assigned a non-default address or are
configured.
*/

/**************************** end of file ***************************/

