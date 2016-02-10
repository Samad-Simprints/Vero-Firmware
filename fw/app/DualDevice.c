/*
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "CDC.h"
#include <stdarg.h>
#include "global.h"
#include "lpc18xx_gpio.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_cgu.h"
#include "task.h"
#include "gpio_dd.hpp"

void FenSerial_Send_Data ( USB_ClassInfo_CDC_Device_t* CDCInterfaceInfo, USB_ClassInfo_CDC_Host_t* CDCHostInterfaceInfo );
void FenSerial_Debug( char *format, ... );

uint8_t USB_GetCurrentMode ( uint8_t corenum );
int USB_GetVBUSState ( void );

typedef enum
{
	BUTTON_UP, BUTTON_PENDING, BUTTON_COMPLETE,
} BUTTON_STATE;
BUTTON_STATE Button1State = BUTTON_UP;

static int CDC_Host_DeviceEnumerated = 0;

void FenSerial_Debug( char *format, ... )
{
  char acBuf[ 256 ];
  va_list args;
  int iArg;

  va_start(args, format);
  iArg = va_arg(args, int);
  sprintf(acBuf, format, iArg);
  va_end(args);

  //sprintf ( acBuf, ... );

  CDCDeviceSendBytes( &CDC_Device_Interface, acBuf, strlen ( acBuf ) );
  return;
}

void FenSerial_Send_Data ( USB_ClassInfo_CDC_Device_t* CDCInterfaceInfo, USB_ClassInfo_CDC_Host_t* CDCHostInterfaceInfo )
{
  static bool boTX = true;
  static int iCount = 0;
  static uint8_t cTxChar = ' ';

  uint8_t acBytes[ 256 ];
  int16_t cRxChar;
  uint16_t iCharCount = 0;

  if( boTX )
  {
    iCount++;
    if( iCount > 1000 )
    {
      iCount = 0;

      //for( iLoop = 0; iLoop < 128; iLoop++ )
      //{
      //	acBytes[ iLoop ] = iLoop + 32;
      //}

      //for ( iRepeat = 0; iRepeat < 100; iRepeat++ )
      //{
      acBytes[ 0 ] = cTxChar;
#if 1 // set to 1 to generate data on port.
      CDCDeviceSendBytes( CDCInterfaceInfo, acBytes, 1 );
#endif
      cTxChar++;
      if( cTxChar > 127 )
              cTxChar = 32;

      //for ( iLoop = 0; iLoop < 10000; iLoop++ );
      //}
    }

    if( CDC_Device_ReceiveByte( CDCInterfaceInfo ) > 0 )
    {
      boTX = false;
      acBytes[ 0 ] = '>';
      acBytes[ 1 ] = ' ';
      CDCDeviceSendBytes( CDCInterfaceInfo, acBytes, 2 );
    }
  }
  else
  {
    if( CDC_Device_BytesReceived( CDCInterfaceInfo ) > 0 )
    {
      cRxChar = CDC_Device_ReceiveByte( CDCInterfaceInfo );
      if( cRxChar > 0 )
      {
        acBytes[ 0 ] = cRxChar & 0x7F;
        if( acBytes[ 0 ] < ' ' )
                acBytes[ 0 ] = '?';

#if 0
        if( ( acBytes[ 0 ] >= '0' ) && ( acBytes[ 0 ] <= '9' ) )
        {
          acBytes[ 0 ] = '9' - acBytes[ 0 ] + '0';
        }
#endif
        CDCHostSendBytes( CDCHostInterfaceInfo, acBytes, 1 );

        if( ( ( cRxChar & 0x7f ) == 10 ) || ( ( cRxChar & 0x7f ) == 13 ) )
        {
          boTX = true;
          acBytes[ 0 ] = 10;
          acBytes[ 1 ] = 13;
          CDCHostSendBytes( CDCHostInterfaceInfo, acBytes, 2 );
        }
      }
      else
      {
        // Unexpected fail.
        acBytes[ 0 ] = 10;
      }
    }

    iCharCount = CDC_Host_BytesReceived( CDCHostInterfaceInfo );
    while( iCharCount > 0 )
    {
      cRxChar = CDC_Host_ReceiveByte( CDCHostInterfaceInfo );
      if( cRxChar >= ' ' )
      {
        acBytes[ 0 ] = cRxChar & 0x7F;

#if 0
        if( ( acBytes[ 0 ] >= '0' ) && ( acBytes[ 0 ] <= '9' ) )
        {
          acBytes[ 0 ] = '9' - acBytes[ 0 ] + '0';
        }
#endif

        CDCDeviceSendBytes( CDCInterfaceInfo, acBytes, 1 );

        if( ( ( cRxChar & 0x7f ) == 10 ) || ( ( cRxChar & 0x7f ) == 13 ) )
        {
          boTX = true;
          acBytes[ 0 ] = 10;
          acBytes[ 1 ] = 13;
          CDCDeviceSendBytes( CDCInterfaceInfo, acBytes, 2 );
        }
      }
      else
      {
        // Unexpected fail.
        acBytes[ 0 ] = 10;
      }

      iCharCount--;
    }
    return;
  }
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect ( uint8_t corenum )
{
  DEBUGMSG(ZONE_DBG_ALWAYS, ("EVENT_USB_Device_Connect on port %d\r\n", corenum));
  return;
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect ( uint8_t corenum )
{
  DEBUGMSG(ZONE_DBG_ALWAYS, ("EVENT_USB_Device_Disconnect on port %d\r\n", corenum));
  return;
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged ( uint8_t corenum )
{
  if( corenum == CDC_DEVICE_CORENUM )
          EVENT_USB_Device_CDC_ConfigurationChanged();
  if( corenum == CDC_HOST_CORENUM )
          EVENT_USB_Host_CDC_ConfigurationChanged();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest ( uint8_t corenum )
{
  if( corenum == CDC_DEVICE_CORENUM )
          EVENT_USB_Device_CDC_ControlRequest();
}

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor ( uint8_t corenum, const uint16_t wValue, const uint8_t wIndex,
                             const void * * const DescriptorAddress )
{
  uint16_t iSize = 0;

  if( corenum == CDC_DEVICE_CORENUM )
  {
    iSize = CALLBACK_USB_CDC_GetDescriptor( corenum, wValue, wIndex, DescriptorAddress );
    //CDC_Host_ConfigurePipes ( &CDC_Host_Interface,
    //                          iSize,
    //			                    *DescriptorAddress );
  }
  if( corenum == CDC_HOST_CORENUM )
  {
    CALLBACK_USB_CDC_GetDescriptor( corenum, wValue, wIndex, DescriptorAddress );
  }

  if ( iSize > 0 ) return ( iSize );
  return -1;
}

/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached ( const uint8_t corenum )
{
  DEBUGMSG(ZONE_DBG_ALWAYS, ("Device Attached on port %d\r\n", corenum));
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached ( const uint8_t corenum )
{
  CDC_Host_DeviceEnumerated = 0;
  DEBUGMSG(ZONE_DBG_ALWAYS, ("\r\nDevice Unattached on port %d\r\n", corenum));
  //USB_Host_ResetBus();
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete ( const uint8_t corenum )
{
  uint8_t bResponse;
  uint16_t ConfigDescriptorSize;
  uint8_t ConfigDescriptorData[ 512 ];
  USB_StdDescriptor_Device_t *psDevDescr;

  DEBUGMSG(ZONE_DBG_ALWAYS, ("\r\nEVENT_USB_Host_DeviceEnumerationComplete, corenum %d\r\n", corenum));

  bResponse = USB_Host_GetDeviceDescriptor( corenum,ConfigDescriptorData );

  if( USB_Host_GetDeviceConfigDescriptor( corenum, 1, &ConfigDescriptorSize, ConfigDescriptorData,
                                          sizeof ( ConfigDescriptorData ) ) != HOST_GETCONFIG_Successful )
  {
    DEBUGMSG(ZONE_DBG_ALWAYS, ("Error Retrieving Configuration Descriptor.\r\n"));
    return;
  }

  psDevDescr = (USB_StdDescriptor_Device_t *) ConfigDescriptorData;

  DEBUGMSG(ZONE_DBG_ALWAYS, ("\r\nUSB_Host_GetDeviceConfigDescriptor OK, len %d\r\n", ConfigDescriptorSize));
  DEBUGMSG(ZONE_DBG_ALWAYS, ("\r\nUSB_Host_GetDeviceConfigDescriptor num configs %d\r\n", psDevDescr->bNumConfigurations ));

  // AARG CDC_Host_Interface.Config.PortNumber = corenum;
  if( CDC_Host_ConfigurePipes( &CDC_Host_Interface, ConfigDescriptorSize, ConfigDescriptorData )
      != MS_ENUMERROR_NoError )
  {
    DEBUGMSG(ZONE_DBG_ALWAYS,("Attached Device Not a Valid CDC Device.\r\n"));
    return;
  }

  if( USB_Host_SetDeviceConfiguration( CDC_Host_Interface.Config.PortNumber, 1 ) != HOST_SENDCONTROL_Successful )
  {
    DEBUGMSG(ZONE_DBG_ALWAYS,("Error Setting Device Configuration.\r\n"));
    return;
  }

  CDC_Host_Interface.State.LineEncoding.BaudRateBPS = 115200;
  CDC_Host_Interface.State.LineEncoding.CharFormat = CDC_LINEENCODING_OneStopBit;
  CDC_Host_Interface.State.LineEncoding.DataBits = 8;
  CDC_Host_Interface.State.LineEncoding.ParityType = CDC_PARITY_None;

  bResponse = CDC_Host_SetLineEncoding( &CDC_Host_Interface );
  if( bResponse != HOST_SENDCONTROL_Successful )
  {
    DEBUGMSG(ZONE_DBG_ALWAYS,("Error Setting Line Encoding.\r\n"));
    return;
  }


  //DEBUGMSG(ZONE_DBG_ALWAYS, ("Vendor \"%.8s\", Product \"%.16s\"\r\n", InquiryData.VendorID, InquiryData.ProductID)); */
  CDC_Host_DeviceEnumerated = 1;

  DEBUGMSG(ZONE_DBG_ALWAYS,("Serial Device Enumerated.\r\n"));
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError ( const uint8_t corenum, const uint8_t ErrorCode )
{
  USB_Disable( corenum, USB_MODE_Host );

  DEBUGMSG(ZONE_DBG_ALWAYS,(("Host Mode Error\r\n"
                                  " -- Error port %d\r\n"
                                  " -- Error Code %d\r\n" ), corenum, ErrorCode));

  for( ;; )
  {
  }
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed ( const uint8_t corenum, const uint8_t ErrorCode, const uint8_t SubErrorCode )
{
  DEBUGMSG(ZONE_DBG_ALWAYS,("Dev Enum Error\r\n"
                                  " -- Error port %d\r\n"
                                  " -- Error Code %d\r\n"
                                  " -- Sub Error Code %d\r\n"
                                  " -- In State %d\r\n",
                  corenum, ErrorCode, SubErrorCode, USB_HostState[corenum]));
}

void xBoard_Init(void)
{
  scu_pinmux(0x9, 5, (MD_PDN | MD_EZI), FUNC4);           /* P9_5 GPIO5[18] USB1_PWR_EN, USB1 VBus function */
  scu_pinmux(0x2, 5, (MD_PDN | MD_EZI | MD_ZI), FUNC2);   /* P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION */
  scu_pinmux(0x6, 3, (MD_PDN | MD_EZI), FUNC1);           /* P6_3 USB0_PWR_EN, USB0 VBus function Xplorer */

  GPIO_SetDir(5, (1UL << 18), 1);                         /* GPIO5[18] = USB1_PWR_EN */
  GPIO_SetValue(5, 1UL << (18));                          /* GPIO5[18] output high */

  // Extra Index pins
  scu_pinmux(0x6, 3, (MD_PDN | MD_EZI), FUNC0);           /* P6_3, 3[2] USB1_PWR_EN */
  scu_pinmux(0x8, 2, (MD_PDN | MD_EZI), FUNC0);           /* P8_2, 4[2] USB_nCON */
  scu_pinmux(0x8, 3, (MD_PDN | MD_EZI), FUNC0);           /* P8_3, 4[3] USBILIM0 */
  scu_pinmux(0x8, 4, (MD_PDN | MD_EZI), FUNC0);           /* P8_4, 4[4] USBILIM1 */

  GPIO_SetDir( 3, (1UL << 2),true);                       /* GPIO3[2] */
  GPIO_SetValue(3, 1UL << (2));                           /* GPIO3[2] output high */

  GPIO_SetDir( 4, (1L << 2),true);                        /* GPIO4[2] */
  GPIO_SetValue( 4, 1L << 2);                             /* GPIO4[2] output high */

  GPIO_SetDir( 4, 3, true);                               /* GPIO4[3] */
  GPIO_SetValue( 4, (1L << 3));                           /* GPIO4[3] output high */

  GPIO_SetDir( 4, 4, true);                               /* GPIO4[4] */
  GPIO_SetValue( 4, (1L << 4));                           /* GPIO4[4] output high */
}

/* Pin initialization using gpio_pindefs where possible
 */
void Board_Init(void)
{
 // scu_pinmux(0x2, 5, (MD_PDN | MD_EZI | MD_ZI), FUNC2);   /* P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION */
  scu_pinmux(0x6, 3, (MD_PDN | MD_EZI), FUNC1);           /* P6_3 USB0_PWR_EN, USB0 VBus function Xplorer */

  // Configure USB1 Pins
  vGPIODDconfigurePin( USB_VBUS_PWR_EN );               // pin 9_5 GPIO5[18]

  // Configure USB0 Pins
  vGPIODDconfigurePin( USB_HOST_PWR_EN );               // pin 6_3 GPIO3[2]
  vGPIODDconfigurePin( USB_HOST_nCON );                 // pin 8_2 GPIO4[2]
  vGPIODDconfigurePin( USB_HOST_ILIM0 );                // pin 8_3 GPIO4[3]
  vGPIODDconfigurePin( USB_HOST_ILIM1 );                // pin 8_4 GPIO4[4]

  // set to required state
  vGPIODDsetInactive(USB_VBUS_PWR_EN);  // set low (USB 1)

  vGPIODDsetInactive(USB_HOST_PWR_EN);  // set low (USB 0)
  vGPIODDsetInactive(USB_HOST_nCON);    // set high  "
  vGPIODDsetActive(USB_HOST_ILIM0);     // set high  "
  vGPIODDsetActive(USB_HOST_ILIM1);     // set high  "
}


static void USBTaskProc(void* params)
{
  FenSerial_Debug( "\r\n\n\n\n\n== Dual USB demo ==\r\n" );

  DEBUGMSG(ZONE_DBG_ALWAYS,("\r\n\n\n\n\n== Dual USB demo ==\r\n"));
  DEBUGMSG(ZONE_DBG_ALWAYS,("CPU speed = %d MHz\n\r", CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE)/1000000));

  CDCHostSetupHardware();
  CDCDeviceSetupHardware();

  for( ;; )
  {
    volatile int USB0_CurrentMode = USB_GetCurrentMode(	CDC_HOST_CORENUM );
#if 0
    // Implement a one-shot trigger that is loaded on button down and reset on button up
    if( Buttons_GetStatus() == BUTTONS_BUTTON1 )
    {
      if( Button1State == BUTTON_UP )
      {
        Button1State = BUTTON_PENDING;
        DEBUGMSG(ZONE_DBG_ALWAYS,("Enabling USB1 interrupts...\n\r"));
      }
    }
    else
      Button1State = BUTTON_UP;
#endif
    switch( USB0_CurrentMode )
    {
    case USB_MODE_Device:
      // Run the device mode and MSC class stacks
      CDC_Host_USBTask( &CDC_Host_Interface );
      USB_USBTask( CDC_HOST_CORENUM, USB_MODE_Host );
      CDC_Device_USBTask( &CDC_Device_Interface );
      USB_USBTask( CDC_DEVICE_CORENUM, USB_MODE_Device );

      FenSerial_Send_Data( &CDC_Device_Interface, &CDC_Host_Interface );

      // Switch over to HOST mode
      if( Button1State == BUTTON_PENDING )
      {
        Button1State = BUTTON_COMPLETE;
        DEBUGMSG(ZONE_DBG_ALWAYS,("** switching to host  mode **\r\n"));
        //MassStorageDeviceShutdownHardware();
//					CDCHostShutdownHardware();
        CDCDeviceShutdownHardware();
//					CDCHostSetupHardware();
        CDCDeviceSetupHardware();
        //MassStorageHostSetupHardware();
      }
      break;

    case USB_MODE_Host:
    default:
      // Run the device mode and MSC class stacks
      CDC_Host_USBTask( &CDC_Host_Interface );
      USB_USBTask( CDC_HOST_CORENUM, USB_MODE_Host );
      CDC_Device_USBTask( &CDC_Device_Interface );
      USB_USBTask( CDC_DEVICE_CORENUM, USB_MODE_Device );

      FenSerial_Send_Data( &CDC_Device_Interface, &CDC_Host_Interface );

      // Switch over to DEVICE mode




      if( Button1State == BUTTON_PENDING )
      {
        Button1State = BUTTON_COMPLETE;
        DEBUGMSG(ZONE_DBG_ALWAYS,("** switching to device  mode **\r\n"));
        CDCHostShutdownHardware();
        //CDCDeviceShutdownHardware();

        CDCHostSetupHardware();
        //CDCDeviceSetupHardware();
      }
      break;
    }
  }
}

/** USB task entry point.
 */
void usb_main ( void )
{
  Board_Init();

  xTaskCreate( &USBTaskProc,       // task proc
               "USB",   // task name
                USB_TASK_STACK_SIZE,  // stack depth
                NULL,                 // task parameters
                USB_TASK_PRIORITY,    // priority
                NULL);                // task handle (not used)
}

