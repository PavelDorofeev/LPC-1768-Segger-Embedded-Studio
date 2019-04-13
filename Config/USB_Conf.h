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
File    : USB_Conf.h
Purpose : Config file. Modify to reflect your configuration
--------  END-OF-HEADER  ---------------------------------------------
*/


#ifndef USB_CONF_H           /* Avoid multiple inclusion */
#define USB_CONF_H

 #ifdef USB_IS_HIGH_SPEED
   #define USB_SUPPORT_HIGH_SPEED           1
 #else
   #define USB_SUPPORT_HIGH_SPEED           0
 #endif

#define USB_MSD_MAX_UNIT                  2

#ifdef DEBUG
  #if DEBUG
    #define USB_DEBUG_LEVEL        2   // Debug level: 1: Support "Panic" checks, 2: Support warn & log
  #endif
#endif

//
// Configure profiling support.
//
#if defined(SUPPORT_PROFILE) && (SUPPORT_PROFILE)
  #ifndef   USBD_SUPPORT_PROFILE
    #define USBD_SUPPORT_PROFILE           1                   // Define as 1 to enable profiling via SystemView.
  #endif
#endif

#define     USB_MAX_NUM_IAD        2

//
// IAR ARM compiler related macros
//
#ifdef __ICCARM__
  #if ((__TID__ >> 4) & 0x0F) < 6   // For any ARM CPU core < v7, we will use optimized routines
    #include "SEGGER.h"
    #define USB_MEMCPY(pDest, pSrc, NumBytes) SEGGER_ARM_memcpy(pDest, pSrc, NumBytes)    // Speed optimization: Our memcpy is much faster!
  #else
//    #include "SEGGER.h"
//    #define USB_MEMCPY(pDest, pSrc, NumBytes) SEGGER_memcpy(pDest, pSrc, NumBytes)    // Speed optimization: Our memcpy is much faster!
  #endif
#endif

#define USB_V2_V3_MIGRATION_DEVINFO      0
#define USB_V2_V3_MIGRATION_CONFIG       0
#define USB_V2_V3_MIGRATION_MSD_LUN_INFO 0
#define USB_V2_V3_MIGRATION_MTP_INFO     0

#define USBD_OS_LAYER_EX                 1      // Use extended OS layer

#define USBD_USE_LEGACY_CACHE_ROUTINES   0

#endif     /* Avoid multiple inclusion */

/*************************** End of file ****************************/
