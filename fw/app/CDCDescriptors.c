/*
 * @brief USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *        computer-readable structures which the host requests upon device enumeration, to determine
 *        the device's capabilities and functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * Copyright(C) Dean Camera, 2011, 2012
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
#include "CDCDescriptors.h"

/* On some devices, there is a factory set internal serial number which can be automatically sent to the host as
 * the device's serial number when the Device Descriptor's .SerialNumStrIndex entry is set to USE_INTERNAL_SERIAL.
 * This allows the host to track a device across insertions on different ports, allowing them to retain allocated
 * resources like COM port numbers and drivers. On demos using this feature, give a warning on unsupported devices
 * so that the user can supply their own serial number descriptor instead or remove the USE_INTERNAL_SERIAL value
 * from the Device Descriptor (forcing the host to generate a serial number for each device from the VID, PID and
 * port location).
 */

#define LE_WORD(x)                              ((x)&0xFF),((x)>>8)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
const USB_Descriptor_Device_t const CDCDeviceDescriptor = {
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t),
  .Type                   = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(02.00),
	//.Class                  = USB_CSCP_NoDeviceClass,
	//.SubClass               = USB_CSCP_NoDeviceSubclass,
	//.Protocol               = USB_CSCP_NoDeviceProtocol,
	.Class                  = CDC_CSCP_CDCClass,
	.SubClass               = CDC_CSCP_ACMSubclass,
	.Protocol               = CDC_CSCP_NoDataProtocol, // CDC_CSCP_ATCommandProtocol, // CDC_CSCP_NoSpecificProtocol, // CDC_CSCP_VendorSpecificProtocol, // CDC_CSCP_ATCommandProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x03EB, // Atmel 0x1fc9,	/* NXP */
	.ProductID              = 0x2009, // Atmel 0x5, // 0x2045,
	.ReleaseNumber          = VERSION_BCD(01.01),

	.ManufacturerStrIndex   = 1,
	.ProductStrIndex        = 2,
	.SerialNumStrIndex      = USE_INTERNAL_SERIAL,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t const CDCConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

#if defined(USB_DEVICE_ROM_DRIVER) && defined(__LPC11UXX__) || defined(__LPC13UXX__)
			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t) - 1,
#else
			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
#endif
			.TotalInterfaces        = 2,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},

#if 1
		.CDC_IF_Assoc =
  		{
  				.Header                 = {.Size = sizeof(USB_Descriptor_Interface_Association_t), .Type = DTYPE_InterfaceAssociation},

          .FirstInterfaceIndex           = 0,
          .TotalInterfaces               = 2,
          .Class                         = 2, // USB_DEVICE_CLASS_COMMUNICATIONS,
          .SubClass                      = 2, // ACM subclass
          .Protocol                      = 0, // no protocol
          .IADStrIndex                   = NO_DESCRIPTOR
  		},
#endif

	.CDC_CCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 1,

			.Class                  = CDC_CSCP_CDCClass,
			.SubClass               = CDC_CSCP_ACMSubclass,
			.Protocol               = CDC_CSCP_NoDataProtocol, // CDC_CSCP_VendorSpecificProtocol, // CDC_CSCP_ATCommandProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_Functional_Header =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalHeader_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_Header,

			.CDCSpecification       = VERSION_BCD(01.10),
		},

	.CDC_Call_Management =
		{
				.Header                 = {.Size = sizeof(USB_CDC_Descriptor_CallManagement_t), .Type = DTYPE_CSInterface},
				.Subtype                = CDC_DSUBTYPE_CSInterface_CallManagement,
				.SelfCallManagement     = 0x03, // 0x01 means handles call management
				.DataInterface          = 0, // Associated data interface
		},

	.CDC_Functional_ACM =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalACM_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_ACM,

			.Capabilities           = 0x02,
		},

	.CDC_Functional_Union =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalUnion_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_Union,

			.MasterInterfaceNumber  = 0,
			.SlaveInterfaceNumber   = 1,
		},

	.CDC_NotificationEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_NOTIFICATION_EPNUM),
			.EndpointAddress        = (ENDPOINT_DIR_IN | CDC_NOTIFICATION_EPNUM),
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = 16, // CDC_NOTIFICATION_EPSIZE,
			.PollingIntervalMS      = 0xFF,
		},

	.CDC_DCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 1,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 2,

			//.Class                  = CDC_CSCP_CDCDataClass,
			//.SubClass               = CDC_CSCP_NoDataSubclass,
			//.Protocol               = CDC_CSCP_NoDataProtocol,
			.Class                  = CDC_CSCP_CDCDataClass, // CDC_CSCP_CDCClass,
			.SubClass               = CDC_CSCP_NoDataSubclass, // CDC_CSCP_ACMSubclass,
			.Protocol               = CDC_CSCP_NoDataProtocol, // CDC_CSCP_VendorSpecificProtocol, // CDC_CSCP_ATCommandProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_OUT | CDC_RX_EPNUM),
			.EndpointAddress        = (ENDPOINT_DIR_OUT | CDC_RX_EPNUM),
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = 64, // CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 5, // 0x01
		},

	.CDC_DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_TX_EPNUM),
			.EndpointAddress        = (ENDPOINT_DIR_IN | CDC_TX_EPNUM),
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = 64, // CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 5, // 0x01
		},

#if 0
	.abStringDescriptors = {
		// LANGUAGE_STRING_INDEX
		4, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, LE_WORD( 0x0409 ),
		// DEVICE_NAME_STRING_INDEX
		20, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, 'F', 0, 'E', 0, 'N', 0, 'N', 0, 'S', 0, ' ', 0, 'l', 0, 'p', 0, 'c', 0,
		// MANUFACTURER_STRING_INDEX
		14, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, 'F', 0, 'e', 0, 'n', 0, 'j', 0, 'e', 0, 't', 0,
		// PRODUCT_STRING_INDEX
		20, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, 'F', 0, 'E', 0, 'N', 0, 'L', 0, 'S', 0, ' ', 0, 'l', 0, 'p', 0, 'c', 0,
		// SERIAL_NUMBER_STRING_INDEX
		14, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '1', 0,
		// IF_NAME_STRING_INDEX_COM1
		12, 3 /*USB_STRING_DESCRIPTOR_TYPE*/, 'P', 0, 'O', 0, 'R', 0, 'T', 0, '1', 0,
		// terminator
		0
	}
#endif

	#if defined(USB_DEVICE_ROM_DRIVER) && defined(__LPC11UXX__) || defined(__LPC13UXX__)
		// Required by IP3511
		.Configuration_Termination = 0x00
#endif
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
uint8_t LanguageString[] =
{
  USB_STRING_LEN(1),
  DTYPE_String,
  WBVAL(LANGUAGE_ID_ENG),
};
USB_Descriptor_String_t *CDCLanguageStringPtr = (USB_Descriptor_String_t*)LanguageString;

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t CDCManufacturerString[] = {
	USB_STRING_LEN(3),
	DTYPE_String,
	WBVAL('F'),
	WBVAL('e'),
	WBVAL('n'),
};
USB_Descriptor_String_t *CDCManufacturerStringPtr = (USB_Descriptor_String_t *) CDCManufacturerString;

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t CDCProductString[] = {
	USB_STRING_LEN(21),
	DTYPE_String,
	WBVAL('F'),
	WBVAL('e'),
	WBVAL('n'),
	WBVAL(' '),
	WBVAL('C'),
	WBVAL('D'),
	WBVAL('C'),
	WBVAL(' '),
	WBVAL('U'),
	WBVAL('S'),
	WBVAL('B'),
	WBVAL(' '),
	WBVAL('D'),
	WBVAL('e'),
	WBVAL('v'),
	WBVAL('i'),
	WBVAL('c'),
	WBVAL('e'),
	WBVAL(' '),
	WBVAL('0'),
	WBVAL('1'),
};
USB_Descriptor_String_t *CDCProductStringPtr = (USB_Descriptor_String_t *) CDCProductString;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_CDC_GetDescriptor(uint8_t corenum,
									const uint16_t wValue,
									const uint8_t wIndex,
									const void * *const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void *Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	printf ("CALLBACK_USB_CDC_GetDescriptor\n");

	switch (DescriptorType) {
	case DTYPE_Device:
		Address = &CDCDeviceDescriptor;
		Size    = sizeof(USB_Descriptor_Device_t);
		break;

	case DTYPE_Configuration:
		Address = &CDCConfigurationDescriptor;
		Size    = sizeof(USB_Descriptor_Configuration_t);
		break;

	case DTYPE_String:
		switch (DescriptorNumber)
		{
		case 0x00:
			Address = CDCLanguageStringPtr;
			Size    = pgm_read_byte(&CDCLanguageStringPtr->Header.Size);
			break;

		case 0x01:
			Address = CDCManufacturerStringPtr;
			Size    = pgm_read_byte(&CDCManufacturerStringPtr->Header.Size);
			break;

		case 0x02:
		default:
			Address = CDCProductStringPtr;
			Size    = pgm_read_byte(&CDCProductStringPtr->Header.Size);
			break;
		}

		break;
	}

	*DescriptorAddress = Address;
	return Size;
}
