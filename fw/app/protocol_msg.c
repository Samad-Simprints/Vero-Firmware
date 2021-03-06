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

// set to 1 to turn on packet assembly debug output
#define PACKET_DEBUG  0

// How much of the buffer we reserve to hold the message source marker.
#define MSG_BUFFER_RESERVED                       4

// We can hold the maximum size protocol message plus the source as 32 bits.
#define MSG_BUFFER_LEN    ( MSG_PACKET_MAX_SIZE + MSG_BUFFER_RESERVED )

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

// Initialise a protocol channel
void vProtocolReset( tMsgSource eSource )
{
  // Empty the incoming buffer and place a source marker at the base of each.
  // This marker will remain there indefinitely and the rest of the buffer is used
  // to hold incoming msg data.
  sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 0 ] = (tMsgSource) eSource;
  sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 1 ] = 0;
  sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 2 ] = 0;
  sMsgBuffer.abProtocolMessageBuffer[ eSource ][ 3 ] = 0;
  iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;

  return;
}

// Initialises the Protocol message block.
void vProtocolInit ( void )
{
  vProtocolReset( MSG_SOURCE_UN20_USB );
  vProtocolReset( MSG_SOURCE_PHONE_USB );
  vProtocolReset( MSG_SOURCE_PHONE_BT );
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

#if PACKET_DEBUG
{
  int i;
  uint8 *pcDebugData = pcData;
  printf("PKT(%d): ", eSource);
  for (i = 0; i < iLength; i++)
  {
    printf(" %02X", *pcDebugData++);
  }
  printf("\n");
}
#endif

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
    iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;

    // We make no attempt to re-synchronise the incoming byte stream with protocol packets
    // if we have got out of step.

    return;
  }

  // Add the received bytes to the buffer.
  memcpy ((void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ iFreeSpaceIndex[ eSource ]],
          (void *) pcData,
          iLength );

  // printf("Got 0x%02X\n", sMsgBuffer.abProtocolMessageBuffer[ eSource ][ iFreeSpaceIndex[ eSource ]]);

  iFreeSpaceIndex[ eSource ] += iLength;

  // printf("Free index %d\n",iFreeSpaceIndex[ eSource ]);

  // See if we have enough message to cover the header.
  if (( iFreeSpaceIndex[ eSource ] - MSG_BUFFER_RESERVED ) > sizeof( MsgPacketheader ))
  {
    // Yes.

    // Check the header syncword.
    uiSyncword = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED ])->uMsgHeaderSyncWord;
#if PACKET_DEBUG
    printf("StartSync: %08X\n", uiSyncword);
#endif
    if( uiSyncword != MSG_PACKET_HEADER_SYNC_WORD )
    {
      // Badly-formatted message.

      // Notify the error via a callback.
      if ( pvRegisteredMsgErrorCallback != NULL )
      {
        pvRegisteredMsgErrorCallback( MSG_ERROR_NO_HEADER_SYNCWORD );
      }

      // Empty the buffer (apart from the source marker) ready for the next msg.
      iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;

      // We make no attempt to re-synchronise the incoming byte stream with protocol packets
      // if we have got out of step.

      return;
    }

    // Find the size of this message.
    uiLength = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED ])->iLength;
#if PACKET_DEBUG
    printf("Length: %d\n", uiLength);
#endif
    // Check whether we have received enough bytes.
    if (( iFreeSpaceIndex[ eSource ] - MSG_BUFFER_RESERVED ) < uiLength )
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
  bMsgId = ((MsgPacketheader *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED ])->bMsgId;
#if PACKET_DEBUG
  printf("MsgId: %d\n", bMsgId);
#endif
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
    iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;

    // We make no attempt to re-synchronise the incoming byte stream with protocol packets
    // if we have got out of step.

    return;
  }

  // Check the footer.
  uiSyncword = ( sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED + uiLength + 0 - MSG_HEADER_FOOTER_SIZE ] << 24 ) +
               ( sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED + uiLength + 1 - MSG_HEADER_FOOTER_SIZE ] << 16 ) +
               ( sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED + uiLength + 2 - MSG_HEADER_FOOTER_SIZE ] <<  8 ) +
               ( sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED + uiLength + 3 - MSG_HEADER_FOOTER_SIZE ] <<  0 );
#if PACKET_DEBUG
  printf("EndSync: %08X\n", uiSyncword);
#endif
  if( uiSyncword != MSG_PACKET_FOOTER_SYNC_WORD )
  {
    // Badly-formatted message.

    // Notify the error via a callback.
    if ( pvRegisteredMsgErrorCallback != NULL )
    {
      pvRegisteredMsgErrorCallback( MSG_ERROR_NO_FOOTER_SYNCWORD );
    }

    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;

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
  if ( iFreeSpaceIndex[ eSource ] == ( MSG_BUFFER_RESERVED + uiLength ))
  {
    // The buffer contained one message and no more.
    // Empty the buffer (apart from the source marker) ready for the next msg.
    iFreeSpaceIndex[ eSource ] = MSG_BUFFER_RESERVED;
  }
  else
  {
    // There are more bytes in the buffer. Shift them to the start of the
    // buffer as the beginning of the next message.
    memcpy ((void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED ],
            (void *) &sMsgBuffer.abProtocolMessageBuffer[ eSource ][ MSG_BUFFER_RESERVED + uiLength ],
            iFreeSpaceIndex[ eSource ] - ( MSG_BUFFER_RESERVED + uiLength ));
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

// create a NACK packet for the supplied packet with the given error code
void vSetupNACK( MsgPacket *psPacket, int16 iStatusCode )
{
  psPacket->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  psPacket->Msgheader.iLength = (sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ));
  psPacket->Msgheader.bMsgId |= MSG_REPLY;
  psPacket->Msgheader.bStatus = iStatusCode;
  psPacket->oPayload.DummyPayload.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;
}

// create an ACK packet for the supplied packet
void vSetupACK( MsgPacket *psPacket )
{
  vSetupNACK( psPacket, MSG_STATUS_GOOD );
}

// create an arbitrary length message packet, payload is set and message trailer inserted after payload
void vSetupMessage( MsgPacket *psPacket, int8 bMessage, int8 bStatus, void *pvPayload, int iPayloadSize )
{
  int8 *pdwFooter;
  psPacket->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  psPacket->Msgheader.iLength = (sizeof( MsgPacketheader ) + iPayloadSize/* + sizeof( uint32 )*/);
  psPacket->Msgheader.bMsgId = bMessage;
  psPacket->Msgheader.bStatus = bStatus;

  if ( pvPayload && iPayloadSize )
  {
    memcpy(&psPacket->oPayload, pvPayload, iPayloadSize);
  }
  // horrible special case until we remove the trailer signature from the structure definitions
  if ( iPayloadSize == 0 )
  {
    psPacket->Msgheader.iLength += sizeof( uint32 );
  }

  pdwFooter = &((int8*)psPacket)[psPacket->Msgheader.iLength - sizeof( uint32 )];
  *((uint32*)pdwFooter) = MSG_PACKET_FOOTER_SYNC_WORD;
}
