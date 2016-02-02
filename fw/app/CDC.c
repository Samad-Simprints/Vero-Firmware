#include "CDC.h"
#include "CDCDescriptors.h"
#include "Endpoint.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/** nxpUSBlib CDC Class driver interface configuration and state information. This structure is
 *  passed to all Mass Storage Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t CDC_Device_Interface =
	{ .Config =
		{ .ControlInterfaceNumber = 0, /**< Interface number of the CDC control interface within the device. */

		.DataINEndpointNumber = CDC_TX_EPNUM, /**< Endpoint number of the CDC interface's IN data endpoint. */
		.DataINEndpointSize = 64, /**< Size in bytes of the CDC interface's IN data endpoint. */
		.DataINEndpointDoubleBank = false, /**< Indicates if the CDC interface's IN data endpoint should use double banking. */

		.DataOUTEndpointNumber = CDC_RX_EPNUM, /**< Endpoint number of the CDC interface's OUT data endpoint. */
		.DataOUTEndpointSize = 64, /**< Size in bytes of the CDC interface's OUT data endpoint. */
		.DataOUTEndpointDoubleBank = false, /**< Indicates if the CDC interface's OUT data endpoint should use double banking. */

		.NotificationEndpointNumber = CDC_NOTIFICATION_EPNUM, /**< Endpoint number of the CDC interface's IN notification endpoint, if used. */
		.NotificationEndpointSize = 16, /**< Size in bytes of the CDC interface's IN notification endpoint, if used. */
		.NotificationEndpointDoubleBank = false, /**< Indicates if the CDC interface's notification endpoint should use double banking. */

		.PortNumber = CDC_DEVICE_CORENUM, },

		.State =
		{ .LineEncoding =
			{ .BaudRateBPS = 115200/*9600*/, /**< Baud rate of the virtual serial port, in bits per second. */
			.CharFormat = CDC_LINEENCODING_OneStopBit, /**< Character format of the virtual serial port, a value from the
			 *   @ref CDC_LineEncodingFormats_t enum.
			 */
			.ParityType = CDC_PARITY_None, /**< Parity setting of the virtual serial port, a value from the
			 *   @ref CDC_LineEncodingParity_t enum.
			 */
			.DataBits = 8, /**< Bits of data per character of the virtual serial port. */
			}, }, };

USB_ClassInfo_CDC_Host_t CDC_Host_Interface =
	{ .Config =
		{ .DataINPipeNumber = 2, /**< Pipe number of the CDC interface's IN data pipe. */
		.DataINPipeDoubleBank = false, /**< Indicates if the CDC interface's IN data pipe should use double banking. */

		.DataOUTPipeNumber = 1, /**< Pipe number of the CDC interface's OUT data pipe. */
		.DataOUTPipeDoubleBank = false, /**< Indicates if the CDC interface's OUT data pipe should use double banking. */

		.NotificationPipeNumber = 0, /**< Pipe number of the CDC interface's IN notification endpoint, if used. */
		.NotificationPipeDoubleBank = false, /**< Indicates if the CDC interface's notification pipe should use double banking. */

		.PortNumber = CDC_HOST_CORENUM,		//< Port number that this interface is running.

		  },

//} Config; /**< Config data for the USB class interface within the device. All elements in this section
//           *   <b>must</b> be set or the interface will fail to enumerate and operate correctly.
//           */
	    .State =
		    { .LineEncoding =
			    { .BaudRateBPS = 115200/*9600*/, /**< Baud rate of the virtual serial port, in bits per second. */
			    .CharFormat = CDC_LINEENCODING_OneStopBit, /**< Character format of the virtual serial port, a value from the
			     *   @ref CDC_LineEncodingFormats_t enum.
			     */
			    .ParityType = CDC_PARITY_None, /**< Parity setting of the virtual serial port, a value from the
			     *   @ref CDC_LineEncodingParity_t enum.
			     */
			    .DataBits = 8, /**< Bits of data per character of the virtual serial port. */
			    }, }, };

#if 0
struct
{
	bool IsActive; /**< Indicates if the current interface instance is connected to an attached device, valid
	 *   after @ref CDC_Host_ConfigurePipes() is called and the Host state machine is in the
	 *   Configured state.
	 */
	uint8_t ControlInterfaceNumber; /**< Interface index of the CDC-ACM control interface within the attached device. */

	uint16_t DataINPipeSize; /**< Size in bytes of the CDC interface's IN data pipe. */
	uint16_t DataOUTPipeSize; /**< Size in bytes of the CDC interface's OUT data pipe. */
	uint16_t NotificationPipeSize; /**< Size in bytes of the CDC interface's IN notification pipe, if used. */

	struct
	{
		uint8_t HostToDevice; /**< Control line states from the host to device, as a set of \c CDC_CONTROL_LINE_OUT_*
		 *   masks - to notify the device of changes to these values, call the
		 *   @ref CDC_Host_SendControlLineStateChange() function.
		 */
		uint8_t DeviceToHost; /**< Control line states from the device to host, as a set of \c CDC_CONTROL_LINE_IN_*
		 *   masks. This value is updated each time @ref CDC_Host_USBTask() is called.
		 */
	}ControlLineStates; /**< Current states of the virtual serial port's control lines between the device and host. */

	CDC_LineEncoding_t LineEncoding; /**< Line encoding used in the virtual serial port, for the device's information.
	 *   This is generally only used if the virtual serial port data is to be
	 *   reconstructed on a physical UART. When set by the host application, the
	 *   @ref CDC_Host_SetLineEncoding() function must be called to push the changes
	 *   to the device.
	 */
}State; /**< State data for the USB class interface within the device. All elements in this section
 *   <b>may</b> be set to initial values, but may also be ignored to default to sane values when
 *   the interface is enumerated.
 */
}USB_ClassInfo_CDC_Host_t;
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

#if 0
/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* HW set up function */
void
CDCDeviceSetupHardware ( void )
{

	USB_Init( CDC_Device_Interface.Config.PortNumber, USB_MODE_Device );
}

/* HW set up function */
void
CDCDeviceShutdownHardware ( void )
{
	USB_Disable( CDC_Device_Interface.Config.PortNumber, USB_MODE_Device );
}

void
CDCDeviceSendBytes ( USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo, uint8_t* acDataBytes, int iLen )
{
	uint8_t iLoop;
	enum Endpoint_WaitUntilReady_ErrorCodes_t eResult;

	//for ( iLoop = 0; iLoop < iLen; iLoop++ )
	//{
	eResult = CDC_Device_SendData( CDCInterfaceInfo, acDataBytes, iLen );

	if( eResult != ENDPOINT_READYWAIT_NoError )
	{
		CDC_Device_Flush( CDCInterfaceInfo );
	}
	//}

	CDC_Device_Flush( CDCInterfaceInfo );

	return;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief Event handler for the library USB Configuration Changed event
 * @return Nothing
 */
void
EVENT_USB_Device_CDC_ConfigurationChanged ( void )
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints( &CDC_Device_Interface );
}

/**
 * @brief Event handler for the library USB Control Request reception event
 * @return	Nothing
 */
void
EVENT_USB_Device_CDC_ControlRequest ( void )
{
	CDC_Device_ProcessControlRequest( &CDC_Device_Interface );
}

/**
 * @brief CDC class driver callback function
 * @return Nothing
 * @note   The reception of SCSI commands from the host, which must be processed
 */
bool
CALLBACK_CDC_Device_SCSICommandReceived ( USB_ClassInfo_CDC_Device_t * const CDCInterfaceInfo )
{
	bool CommandSuccess = TRUE;

	//CommandSuccess = SCSI_DecodeSCSICommand(CDCInterfaceInfo);
	return CommandSuccess;
}

void
EVENT_CDC_Device_LineEncodingChanged ( USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo )
{
	return;
}
#endif

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* HW set up function */
void
CDCDeviceSetupHardware ( void )
{

	USB_Init( CDC_Device_Interface.Config.PortNumber, USB_MODE_Device );
}

void
CDCHostSetupHardware ( void )
{

	USB_Init( CDC_Host_Interface.Config.PortNumber, USB_MODE_Host );
}

/* HW shutdown functions */
void
CDCDeviceShutdownHardware ( void )
{
	USB_Disable( CDC_Device_Interface.Config.PortNumber, USB_MODE_Device );
}

void
CDCHostShutdownHardware ( void )
{
	USB_Disable( CDC_Host_Interface.Config.PortNumber, USB_MODE_Host );
}

void
CDCDeviceSendBytes ( USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo, uint8_t* acDataBytes, int iLen )
{
	uint8_t iLoop;
	enum Endpoint_WaitUntilReady_ErrorCodes_t eResult;

//for ( iLoop = 0; iLoop < iLen; iLoop++ )
//{
	eResult = CDC_Device_SendData( CDCInterfaceInfo, acDataBytes, iLen );

	if( eResult != ENDPOINT_READYWAIT_NoError )
	{
		CDC_Device_Flush( CDCInterfaceInfo );
	}
//}

	CDC_Device_Flush( CDCInterfaceInfo );

	return;
}

void
CDCHostSendBytes ( USB_ClassInfo_CDC_Host_t* const CDCInterfaceInfo, uint8_t* acDataBytes, int iLen )
{
	uint8_t iLoop;
	enum Endpoint_WaitUntilReady_ErrorCodes_t eResult;

//for ( iLoop = 0; iLoop < iLen; iLoop++ )
//{
	eResult = CDC_Host_SendData( CDCInterfaceInfo, acDataBytes, iLen );

	if( eResult != ENDPOINT_READYWAIT_NoError )
	{
		CDC_Host_Flush( CDCInterfaceInfo );
	}
//}

	CDC_Device_Flush( CDCInterfaceInfo );

	return;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief Event handler for the library USB Configuration Changed event
 * @return Nothing
 */
void
EVENT_USB_Device_CDC_ConfigurationChanged ( void )
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints( &CDC_Device_Interface );
}

/**
 * @brief Event handler for the library USB Configuration Changed event
 * @return Nothing
 */
void
EVENT_USB_Host_CDC_ConfigurationChanged ( void )
{
	bool ConfigSuccess = true;

	//ConfigSuccess &= CDC_Host_ConfigurePipes( &CDC_Device_Interface,
	//                                         uint16_t ConfigDescriptorSize,
	//                                         void* ConfigDescriptorData)

	return;
}

/**
 * @brief Event handler for the library USB Control Request reception event
 * @return	Nothing
 */
void
EVENT_USB_Device_CDC_ControlRequest ( void )
{
	CDC_Device_ProcessControlRequest( &CDC_Device_Interface );
}

/**
 * @brief CDC class driver callback function
 * @return Nothing
 * @note   The reception of SCSI commands from the host, which must be processed
 */
bool
CALLBACK_CDC_Device_SCSICommandReceived ( USB_ClassInfo_CDC_Device_t * const CDCInterfaceInfo )
{
	bool CommandSuccess = TRUE;

//CommandSuccess = SCSI_DecodeSCSICommand(CDCInterfaceInfo);
	return CommandSuccess;
}

void
EVENT_CDC_Device_LineEncodingChanged ( USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo )
{
	CDC_Device_Flush( CDCInterfaceInfo );
	return;
}

void
EVENT_CDC_Host_LineEncodingChanged ( USB_ClassInfo_CDC_Host_t* const CDCInterfaceInfo )
{
	CDC_Host_Flush( CDCInterfaceInfo );
	return;
}

void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
	CDC_Device_Flush( CDCInterfaceInfo );
	return;
}

void EVENT_CDC_Device_BreakSent(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo,
                                const uint8_t Duration)
{
	CDC_Device_Flush( CDCInterfaceInfo );
	return;
}

