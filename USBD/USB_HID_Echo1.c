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

File    : USB_HID_Echo1.c
Purpose : This sample shows a simple echo server realized with
          the HID protocol.

Additional information:
  Preparations:
    The sample should be used together with it's PC counterpart
    found under \Windows\USB\HID\SampleApp\Exe\

  Expected behavior:
    When the HIDEcho1 sample is started on the PC it should display
    information about the connected HID device.
    The sample should transmit the user specified number of bytes
    to the target and back.

  Sample output:
    The target side does not produce terminal output.
    HID PC counterpart output:
    Device 0:
    Productname: HID generic sample
    VID        : 0x8765
    PID        : 0x1114
    ReportSizes:
    Input     : 64 bytes
    Output    : 64 bytes
    Starting Echo...
    Enter the number of echoes to be sent to the echo client: 500

    <...>

    500 echoes successfully transferred.
    Communication with USB HID device was successful!
    Press enter to exit.
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
*       Defines configurable
*
**********************************************************************
*/
#define INPUT_REPORT_SIZE   64    // Defines the input (device -> host) report size
#define OUTPUT_REPORT_SIZE  64    // Defines the output (Host -> device) report size
#define VENDOR_PAGE_ID      0x12  // Defines the vendor specific page that
                                  // shall be used, allowed values 0x00 - 0xff
                                  // This value must be identical to HOST application

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
  0x1114,         // ProductId. Should be unique for this sample
  "Vendor",       // VendorName
  "HID generic sample",  // ProductName
  "12345678"      // SerialNumber
};

/*********************************************************************
*
*       Const data
*
**********************************************************************
*/

const U8 _aHIDReport[] = {
    0x06, VENDOR_PAGE_ID, 0xFF,    // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x06,                    //   USAGE_PAGE (Generic Device)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, INPUT_REPORT_SIZE,       //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x06,                    //   USAGE_PAGE (Generic Device)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, OUTPUT_REPORT_SIZE,      //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static char _ac[USB_HS_INT_MAX_PACKET_SIZE];

/*********************************************************************
*
*       _AddHID
*
*  Function description
*    Add HID class to USB stack
*/
static USB_HID_HANDLE _AddHID(void) {
  static U8 _abOutBuffer[USB_HS_INT_MAX_PACKET_SIZE];

  USB_HID_INIT_DATA     InitData;
  U8                    Interval = 8;
  USB_HID_HANDLE        hInst;

  memset(&InitData, 0, sizeof(InitData));
  InitData.EPIn    = USBD_AddEP(USB_DIR_IN,  USB_TRANSFER_TYPE_INT, Interval, NULL, 0);
  InitData.EPOut   = USBD_AddEP(USB_DIR_OUT, USB_TRANSFER_TYPE_INT, Interval, _abOutBuffer, sizeof(_abOutBuffer));
  InitData.pReport = _aHIDReport;
  InitData.NumBytesReport  = sizeof(_aHIDReport);
  USBD_SetDeviceInfo(&_DeviceInfo);
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
* Function description
*   USB handling task.
*   Modify to implement the desired protocol
*/
#ifdef __cplusplus
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
void MainTask(void);
#ifdef __cplusplus
}
#endif
void MainTask(void) {
  USB_HID_HANDLE hInst;

  USBD_Init();
  hInst = _AddHID();
  USBD_Start();
  //
  // Loop: Receive data byte by byte, send back (data + 1)
  //
  while (1) {
    //
    // Wait for configuration
    //
    while ((USBD_GetState() & (USB_STAT_CONFIGURED | USB_STAT_SUSPENDED)) != USB_STAT_CONFIGURED) {
      BSP_ToggleLED(0);
      USB_OS_Delay(50);
    }
    BSP_SetLED(0);
    USBD_HID_Read(hInst, &_ac[0], OUTPUT_REPORT_SIZE, 0);
    _ac[0]++;
    USBD_HID_Write(hInst, &_ac[0], INPUT_REPORT_SIZE, 0);
    USB_OS_Delay(50);
  }
}
