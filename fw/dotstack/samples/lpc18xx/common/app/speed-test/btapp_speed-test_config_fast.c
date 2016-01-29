/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "cdbt/bt/bt_std.h"
#include "btapp_config.h"

#ifdef FAST_CONFIG

#define BT_INCLUDE_RFCOMM
#define BT_INCLUDE_SPP

#define BT_ENABLE_SSP

// Number of HCI command buffers.
#define HCI_MAX_CMD_BUFFERS      2

// Number of HCI data buffers.
#define HCI_MAX_DATA_BUFFERS     2

// Maximum number of simultaneous HCI connections (ACL or SCO).
#define HCI_MAX_HCI_CONNECTIONS  1

// Size of the HCI receive buffer
// Cannot be less than either 
// 40 or
// HCI_TRANSPORT_HEADER_LEN(1 byte) + HCI_ACL_DATA_HEADER_LEN(4 bytes) or
// HCI_TRANSPORT_HEADER_LEN(1 byte) + HCI_MAX_CMD_PARAM_LEN + 2
#define HCI_RX_BUFFER_LEN          1026 // this value corresponds to the maximum baseband packet 3-DH5 
                                        // which can carry up to 1021 bytes of data (excluding 2 byte of header)
                                        // 5 bytes are used for ACL and transport headers

// Size of the HCI send buffer
// Cannot be less than either 
// 32 or
// HCI_TRANSPORT_HEADER_LEN(1 byte) + HCI_ACL_DATA_HEADER_LEN(4 bytes) or
// HCI_TRANSPORT_HEADER_LEN(1 byte) + HCI_CMD_HEADER_LEN(3 bytes) + HCI_MAX_CMD_PARAM_LEN
// If not defined, it is set to HCI_RX_BUFFER_LEN
//#define HCI_TX_BUFFER_LEN          315 // this value is for CSR8810/11. The length of ACL buffers in them is 310 bytes by default
                                       // 5 bytes are used for ACL and transport headers

#define HCI_TX_BUFFER_LEN           1026 // this value is for TI CC2564.The length of ACL buffers in it is 1021 bytes by default
                                         // 5 bytes are used for ACL and transport headers

// Maximum size of a complete L2CAP packet including packet header
// Cannot be less than HCI_RX_BUFFER_LEN - HCI_ACL_DATA_HEADER_LEN(4 bytes) - HCI_TRANSPORT_HEADER_LEN(1 byte)
// If not defined, it is set to HCI_RX_BUFFER_LEN - HCI_ACL_DATA_HEADER_LEN - HCI_TRANSPORT_HEADER_LEN
//#define HCI_L2CAP_BUFFER_LEN        192

// Maximum length in bytes of an HCI command parameter.
#define HCI_MAX_CMD_PARAM_LEN       32

// Number of L2CAP command buffers
#define L2CAP_MAX_CMD_BUFFERS    4

#define L2CAP_MAX_PSMS           3

#define L2CAP_MAX_CHANNELS       2

// SDP buffers
#define SDP_MAX_SEARCH_RESULT_LEN	    1

#define SDP_MAX_ATTRIBUTE_RESULT_LEN	10

#define SDP_MAX_PDU_BUFFERS      1

// RFCOMM buffers
#define RFCOMM_MAX_SESSIONS      1

#define RFCOMM_MAX_DLCS          2

#define RFCOMM_MAX_SERVER_CHANNELS RFCOMM_MAX_DLCS - 1

// Maximum size of the data portion of a UIH frame. If CFC is used the actual length of the data portion will be 1 byte less.
// Must be less than or equal to HCI_L2CAP_BUFFER_LEN - RFCOMM_FRAME_HEADER_LEN - L2CAP_HEADER_LEN.
#define RFCOMM_INFO_LEN             (HCI_L2CAP_BUFFER_LEN - RFCOMM_FRAME_HEADER_LEN - L2CAP_HEADER_LEN)//31

#define RFCOMM_MAX_DATA_BUFFERS  (RFCOMM_MAX_DLCS - 1) * 2

#define RFCOMM_MAX_CMD_BUFFERS   (RFCOMM_MAX_DLCS - 1) * 2

#define RFCOMM_LOCAL_CREDIT         12

// SPP ports
#define SPP_MAX_PORTS               1


#include "cdbt/bt/bt_oem_config.h"

#endif
