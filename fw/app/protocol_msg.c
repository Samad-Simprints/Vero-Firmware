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
// MODULE: protocol_msg.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  INDEX message protocol structures and functions.
//
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"
#include "protocol_msg.h"
#include "msg_format.h"

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

// We can hold the maximum size protocol message plus the source as 32 bits.
#define MSG_BUFFER_LEN    ( MSG_PACKET_MAX_SIZE + 4 )

//******************************************************************************
// Local Storage
//******************************************************************************

/* These buffers hold incoming received protocol messages as they are built and transmitted.
// The struct is used to force byte alignment without using any non-portable code. */
static union sMsgBufferAligned
{
//#if portBYTE_ALIGNMENT == 8
//  volatile portDOUBLE dDummy;
//#else
  volatile uint32 ulDummy;
//#endif

  uint8 abProtocolMessageBuffer[ MSG_SOURCE_NUM_SOURCES ][ MSG_BUFFER_LEN ];
}
__attribute__((packed)) sMsgBuffer;

uint16 iFreeSpaceIndex[ MSG_SOURCE_NUM_SOURCES ];

vMsgCallback pvRegisteredMsgCallback = NULL;
vMsgErrorCallback pvRegisteredMsgErrorCallback = NULL;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

// Initialises the Protocol message block.
void vProtocolInit ( void )
{
  int iLoop;

  // Empty all incoming buffers and place a source marker at the base of each.
  // This marker will remain there indefinitely and the rest of the buffer is used
  // to hold incoming msg data.
  for ( iLoop = 0; iLoop < MSG_SOURCE_NUM_SOURCES; iLoop++ )
  {
    sMsgBuffer.abProtocolMessageBuffer[ iLoop ][ 0 ] = (tMsgSource) iLoop;
    sMsgBuffer.abProtocolMessageBuffer[ iLoop ][ 1 ] = 0;
    sMsgBuffer.abProtocolMessageBuffer[ iLoop ][ 2 ] = 0;
    sMsgBuffer.abProtocolMessageBuffer[ iLoop ][ 3 ] = 0;
    iFreeSpaceIndex[ iLoop ] = 4;
  }

  return;
}

// Stores received data bytes from a given source and builds up whole messages.
// Note: We hope that the data passed to this function never
// contains the end of one protocol message and the start of another, but we
// will try to cope with this situation by using any bytes "left over" after
// extracting a whole message as the start of the next message.
void vIncomingBytes(tMsgSource eSource, uint8 *pcData, int iLength)
{
  uint32 uiSyncword;
  uint16 uiLength;
  uint8  bMsgId;

  // Add the received bytes to the receive buffer, if there's space.
  if ( iFreeSpaceIndex[ eSource ] + iLength > MSG_BUFFER_LEN )
  {
    // Message too long for buffer.

    // Notify the error via a callback.
    if ( pvRegisteredMsgErrorCallback != NULL )
    {
      pvRegisteredMsgErrorCallback( MSG_ERROR_BAD_LENGTH );
    }

    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = 4;

    // We make no attempt to re-synchronise the incoming byte stream with protocol packets
    // if we have got out of step.

    return;
  }

  // Add the received bytes to the buffer.
  memcpy ((void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ iFreeSpaceIndex[ eSource ]],
          (void *) pcData,
          iLength );

  iFreeSpaceIndex[ eSource ] += iLength;

  // See if we have enough message to cover the header.
  if ( iFreeSpaceIndex[ eSource ] > sizeof( MsgPacketheader ))
  {
    // Yes.

    // Check the header syncword.
    uiSyncword = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 4 ])->uMsgHeaderSyncWord;
    if( uiSyncword != MSG_PACKET_HEADER_SYNC_WORD )
    {
      // Badly-formatted message.

      // Notify the error via a callback.
      if ( pvRegisteredMsgErrorCallback != NULL )
      {
        pvRegisteredMsgErrorCallback( MSG_ERROR_NO_HEADER_SYNCWORD );
      }

      // Empty the buffer (apart from the source marker) ready for the next msg.
      iFreeSpaceIndex[ eSource ] = 4;

      // We make no attempt to re-synchronise the incoming byte stream with protocol packets
      // if we have got out of step.

      return;
    }

    // Find the size of this message.
    uiLength = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 4 ])->iLength;

    // Check whether we have received enough bytes.
    if (( iFreeSpaceIndex[ eSource ] - 1 ) < uiLength )
    {
      // Not got enough message yet.
      return;
    }
  }
  else
  {
    // Not got enough message yet.
    return;
  }

  // Check the message type.
  bMsgId = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 4 ])->bMsgId;

  // Ignore the Response/Reply bit.
  bMsgId &= ~MSG_REPLY;

  if (( bMsgId < MSG_GET_SENSOR_INFO ) || ( bMsgId >= MSG_NUM_MSGS ))
  {
    // Badly-formatted message.

    // Notify the error via a callback.
    if ( pvRegisteredMsgErrorCallback != NULL )
    {
      pvRegisteredMsgErrorCallback( MSG_ERROR_BAD_MSG_ID );
    }

    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = 4;

    // We make no attempt to re-synchronise the incoming byte stream with protocol packets
    // if we have got out of step.

    return;
  }

  // Check the footer.
  uiSyncword = *((uint32 *)( &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ uiLength ] ));
  if( uiSyncword != MSG_PACKET_FOOTER_SYNC_WORD )
  {
    // Badly-formatted message.

    // Notify the error via a callback.
    if ( pvRegisteredMsgErrorCallback != NULL )
    {
      pvRegisteredMsgErrorCallback( MSG_ERROR_NO_FOOTER_SYNCWORD );
    }

    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = 4;

    // We make no attempt to re-synchronise the incoming byte stream with protocol packets
    // if we have got out of step.

    return;
  }

  // This message looks like a good 'un.

  // Notify of a complete message. This callback must not return until it is
  // safe to discard the message contents.
  if ( pvRegisteredMsgCallback != NULL )
  {
    pvRegisteredMsgCallback( (MsgInternalPacket *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 0 ] );
  }

  // See if this message ran up to the end of the received data.
  if ( iFreeSpaceIndex[ eSource ] == ( 4 + uiLength ))
  {
    // The buffer contained one message and no more.
    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = 4;
  }
  else
  {
    // There are more bytes in the buffer. Shift them to the start of the
    // buffer as the beginning of the next message.
    memcpy ((void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 4 ],
            (void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 4 + uiLength ],
            iFreeSpaceIndex[ eSource ] - ( 4 + uiLength ));
    iFreeSpaceIndex[ eSource ] -= uiLength;
  }

  return;
};

// Registers a callback for notification of a complete incoming message
void vProtocolMsgNotify(vMsgCallback pvCallback)
{
  pvRegisteredMsgCallback = pvCallback;
  
  return;
};

// Registers a callback for notification of an incomplete or errored incoming message
void vProtocolMsgError(vMsgErrorCallback pvCallback)
{
  pvRegisteredMsgErrorCallback = pvCallback;
  
  return;
};

