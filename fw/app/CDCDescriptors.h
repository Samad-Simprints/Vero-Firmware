/*
 * @brief CDC device class declarations, definitions for using in application
 *
 * @note
 * Copyright(C) Fen Technology, 2015
 * Copyright(C) Duncan Hurst, 2015
 * All rights reserved.
 *
 */

#ifndef _CDC_DESCRIPTORS_H_
#define _CDC_DESCRIPTORS_H_

#include "USB.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CDC_Device_Descriptor Class descriptors
 * @ingroup USB_CDC_Device_18xx43xx USB_CDC_Device_17xx40xx USB_CDC_Device_11Uxx
 * @{
 */

/** Endpoint number of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPNUM         1

/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define CDC_NOTIFICATION_EPSIZE        8

/** Endpoint number of the CDC device-to-host data IN endpoint. */
#if defined(__LPC17XX__) || defined(__LPC177X_8X__) || defined(__LPC407X_8X__)
#define CDC_IN_EPNUM          5
#else
#define CDC_TX_EPNUM          2
#endif

/** Endpoint number of the CDC host-to-device data OUT endpoint. */
#define CDC_RX_EPNUM         3

#if defined(__LPC18XX__) || defined(__LPC43XX__)
/** Size in bytes of the CDC data endpoints. */
#if USB_FORCED_FULLSPEED
#define CDC_IO_EPSIZE         64
#else
#define CDC_IO_EPSIZE         512
#endif // USB_FORCED_FULLSPEED
#endif // defined(__LPC18XX__) || defined(__LPC43XX__)

/** Size in bytes of the CDC data IN and OUT endpoints. */
#define CDC_TXRX_EPSIZE                16

/** @brief	Type define for the device configuration descriptor structure. This must be defined in the
 *          application code, as the configuration descriptor contains several sub-descriptors which
 *          vary between devices, and which describe the device's usage to the host.
 */
typedef struct
{
	USB_Descriptor_Configuration_Header_t Config;

	// CDC Interface
	USB_Descriptor_Interface_t            CDC_Interface;
	USB_Descriptor_Endpoint_t             CDC_DataInEndpoint;
	USB_Descriptor_Endpoint_t             CDC_DataOutEndpoint;
}
USB_Descriptor_Configuration_CDC_t;


uint16_t CALLBACK_USB_CDC_GetDescriptor(uint8_t corenum,
									const uint16_t wValue,
									const uint8_t wIndex,
									const void * *const DescriptorAddress);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _CDC_DESCRIPTORS_H_ */
