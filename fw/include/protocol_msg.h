//******************************************************************************
//
// Project Index source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// MODULE: protocol_msg.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  INDEX message protocol structures and function definitions.
//
//******************************************************************************

#if !defined(_PROTOCOL_MSG_H_)
#define _PROTOCOL_MSG_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"
#include "msg_format.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

// Message error codes.
typedef enum
{
  MSG_ERROR_OK = 0,
  MSG_ERROR_NO_HEADER_SYNCWORD,
  MSG_ERROR_BAD_LENGTH,
  MSG_ERROR_NO_FOOTER_SYNCWORD,
  MSG_ERROR_BAD_MSG_ID,
  MSG_ERROR_NUM_ERRORS
}
tMsgError;

// Message-complete callback definition.
typedef void (*vMsgCallback)(MsgInternalPacket *psMsg);

// Message-error callback definition.
typedef void (*vMsgErrorCallback)(tMsgError eErrorCode);

//******************************************************************************
// Function Prototypes
//******************************************************************************

// Initialises the Protocol message block.
extern void vProtocolInit ( void );

extern void vIncomingBytes(tMsgSource eSource, uint8 *pcData, int iLength); // Stores received data bytes from a given source

// Register for notification of a complete message.
// This callback must not return until it is safe to discard the message contents.
extern void vProtocolMsgNotify(vMsgCallback pvCallback);

// Register for notification of an incomplete or errored message.
// This callback must not return until it is safe to discard the message contents.
extern void vProtocolMsgError(vMsgErrorCallback pvCallback);

#ifdef __cplusplus
}
#endif

#endif  // _PROTOCOL_MSG_H_
