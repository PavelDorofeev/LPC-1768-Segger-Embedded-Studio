
#include "USB_HID.h"
#include "USB.h"
#include "BSP.h"
#include "LPC17xx.h"
#include <stdio.h>

void usbSettings(void);

void set(uint32_t *port, int pin, int val);

void myUsbHarwareConnector()
{
  // Подтягиваем USB D+ к питанию
  set(&LPC_PINCON->PINMODE7 , 19 , 0);
  set(&LPC_PINCON->PINMODE7 , 18 , 0);
  /*    PINMODE
    00 Pin has an on-chip pull-up resistor enabled. 00
    01 Repeater mode (see text below).
    10 Pin has neither pull-up nor pull-down resistor enabled.
    11 Pin has an on-chip pull-down resistor enabled.
  */    

  set(&LPC_GPIO3->FIODIR, 25,1); 
  set(&LPC_GPIO3->FIOSET, 25,1); 
}

void  set(uint32_t *port, int pin, int val) //port=LPC_GPIO3->FIODIR  pin=25 val=0..1
{
  if(val==1)
    *port |= (1 << pin);
  else if(val==0)
    *port &= ~(1 << pin);
}


void usbSettings(void) 
{
  //SEGGER_RTT_printf(0, "USBD_X_Config\n");
  BSP_Init();   
  BSP_ClrLED(0);                   // Initially set LED             
  BSP_USB_Init();

  U32 Temp;
  unsigned int tmp[2];

  //LPC_PINCON->PINSEL1 &= ~((3<<26)|(3<<28));   // P0.29 D+, P0.30 D-
  set(&LPC_PINCON->PINSEL1 , 26 , 1);
  set(&LPC_PINCON->PINSEL1 , 27 , 0);

  set(&LPC_PINCON->PINSEL1 , 28 , 1);
  set(&LPC_PINCON->PINSEL1 , 29 , 0);
  
  //LPC_PINCON->PINSEL4 &= ~((3<<18)        );   // P2.9 SoftConnect 
  //LPC_PINCON->PINSEL4 |=  ((1<<18)        );   // PINSEL4 18.19 = 01 
  set(&LPC_PINCON->PINSEL4 , 18 , 1);
  set(&LPC_PINCON->PINSEL4 , 19 , 0);


  //LPC_PINCON->PINSEL3 |=  ((1<< 4)|(2<<28));   // PINSEL3 4.5 = 01, 28.29 = 10 
  // 29:28=P1.30= VBus = 0:1
  // 5:4=P1.18 = USB_UP_LED = 0:1
  set(&LPC_PINCON->PINSEL3 , 4 , 1);
  set(&LPC_PINCON->PINSEL3 , 5 , 0);
  set(&LPC_PINCON->PINSEL3 , 28 , 1);
  set(&LPC_PINCON->PINSEL3 , 29 , 0);


  //LPC_SC->PCONP|=0x80000000U; //enable PCUSB == LPC_SC->PCONP |= (1UL<<31);
  // 0x400F C108
  //LPC_SC->PCONP |= (1UL<<31);   // 0x80000000 USB PCLK -> enable USB Per.       
  set(&LPC_SC->PCONP , 31 , 1);

  LPC_SC->USBCLKCFG=5;  // USBCLKCFG (bit 0..3) USBSEL: Divide Value for USB Clock from PLL0
  // 5 PLL0 output is divided by 6. PLL0 output must be 288 MHz 

  //LPC_USB->USBClkCtrl|=0x02; // turn on DEV_CLK_ON (=DEV_CLK_EN) and AHB_CLK_ON (=AHB_CLK_EN)
  set(&LPC_USB->USBClkCtrl , 1 , 1);

  while((LPC_USB->USBClkSt & 0x02)!=0x02) // USB Clock Status register
  ;

  LPC_USB->USBClkCtrl|=0x10;

  while((LPC_USB->USBClkSt & 0x10)!=0x10) // USB Clock Status register
  ;

  //LPC_PINCON->PINSEL1|=0x14000000; // =LPC_PINCON_BASE=LPC_APB0_BASE(0x40000000UL) + 0x2C000 -> PINSEL1
  //LPC_PINCON->PINSEL1 |=  ((1<<26)|(1<<28));   // PINSEL1 26.27, 28.29  = 01 
  set( &LPC_PINCON->PINSEL1 , 26 , 1 );
  set( &LPC_PINCON->PINSEL1 , 28 , 1 );
  //

  LPC_PINCON->PINSEL3|=0x20802090; // 0x20000010
  // 0010 0000 1000 0000 0010 0000 1001 0000    4=1
  set( &LPC_PINCON->PINSEL1 , 5 , 0 );  // 5:4    01=USB_UP_LED
  set( &LPC_PINCON->PINSEL1 , 4 , 1 );  // 5:4    01=USB_UP_LED
  set( &LPC_PINCON->PINSEL1 , 7 , 1 );  // 7:6    10=~USB_PPWR 
  set( &LPC_PINCON->PINSEL1 , 6 , 0 );  // 7:6    10=~USB_PPWR 
  set( &LPC_PINCON->PINSEL1 , 13 , 1 ); // 13:12  10=USB_PWRD
  set( &LPC_PINCON->PINSEL1 , 12 , 0 ); // 13:12  10=USB_PWRD
  set( &LPC_PINCON->PINSEL1 , 23 , 1 ); // 23:22  10=~USB_PWRD
  set( &LPC_PINCON->PINSEL1 , 22 , 0 ); // 23:22  10=~USB_PWRD
  set( &LPC_PINCON->PINSEL1 , 29 , 1 ); // 29:28  10=VBUS
  set( &LPC_PINCON->PINSEL1 , 28 , 0 ); // 29:28  10=VBUS

  //LPC_PINCON->PINSEL4|=0x00040000;  // ?0100 0000 0000 0000 14=1
  //LPC_PINCON->PINSEL4 &= ~((3<<18)        );   // P2.9 SoftConnect 
  set( &LPC_PINCON->PINSEL4 , 19 , 1 );  // 19:18 01=USB_CONNECT
  set( &LPC_PINCON->PINSEL4 , 18 , 0 );  // 19:18 01=USB_CONNECT
  
  LPC_PINCON->PINMODE3|=0x20000000;  // 0010 0000 0000 0000 0000 0000 0000 0000   29=1 P1.30MODE
  // P1.18 GoodLink, P1.30 VBUS 
  set( &LPC_PINCON->PINMODE3 , 29 , 1 );
  set( &LPC_PINCON->PINMODE3 , 28 , 0 );


  // USB_Reset 
  LPC_USB->USBDevIntClr=0x100;  // USB Device Interrupt Clear ( 0x5000C208)  x0100= EP_SLOW
  //set( &LPC_USB->USBDevIntClr , 2 , 1 );

  LPC_USB->USBEpInd=0; 
  LPC_USB->USBMaxPSize=64; // USB MaxPacketSize
  
  while(!(LPC_USB->USBDevIntSt & 0x100))  // USB Device Interrupt Status  0x100 = EP_SLOW
    ;
  
  LPC_USB->USBDevIntClr=0x100;
  LPC_USB->USBEpInd=1;
  LPC_USB->USBMaxPSize=64; // USB MaxPacketSize
  
  while(!(LPC_USB->USBDevIntSt & 0x100)) // USB Device Interrupt Status  0x100 = EP_SLOW
    ;
  
  LPC_USB->USBDevIntClr=0x100;

  LPC_USB->USBEpIntClr|=0xFFFFFFFF;  //  USB Endpoint Interrupt Clear register (0x5000C238)
  LPC_USB->USBDevIntClr|=0xFFFFFFFF;
  LPC_USB->USBEpIntEn|=0x3; // USB Endpoint Interrupt Enable register
  // set mode SEI
  tmp[0]=1U;

  //SetWriteCommand(CMD_SET_MODE,tmp,1U);
  LPC_USB->USBDevIntEn|=0x0000000E; // USB Device Interrupt Enable register 
  //x1110 = DEV_STAT|EP_SLOW|EP_FAST

  NVIC_EnableIRQ(USB_IRQn);

  LPC_SC->USBIntSt|=0x80000000;

#if USBD_SUPPORT_PROFILE
  USBD_SYSVIEW_Init();
#endif

  //USB_SetWarnFilter(0xFFFFFFFFF);
  //USBD_SetLogFilter(0XFFFFFFFFF);
  USB_DRIVER_LPC17xx_ConfigAddr(0x5000C000); //  0x2008C000
  USBD_AddDriver(&USB_Driver_NXP_LPC17xx);//USB_Driver_Freescale_KinetisEHCI);
  //USBD_AssignMemory(_MemPool, sizeof(_MemPool));
}
