
#include "CppUTest/TestHarness.h"
#include "protocol_msg.h"

static MsgPacket *testPacket = nullptr;
typedef uint32 footerSyncWord_t;    // Assumption: the footer sync word is a uint32

TEST_GROUP(protocol)
{
    void setup()
    {
        testPacket = (MsgPacket*)malloc(sizeof( testPacket ));
    }
    void teardown()
    {
        free( testPacket );
    }
};

/*
    Check the status is set
*/
TEST( protocol, setupNACK_status)
{
    vSetupNACK( testPacket, MSG_STATUS_ERROR );
    BYTES_EQUAL( MSG_STATUS_ERROR, testPacket->Msgheader.bStatus);
}

/*
    Check the header sync word is set
*/
TEST( protocol, setupNACK_headerSyncWord)
{
    vSetupNACK( testPacket, MSG_STATUS_ERROR );
    UNSIGNED_LONGS_EQUAL( MSG_PACKET_HEADER_SYNC_WORD, testPacket->Msgheader.uMsgHeaderSyncWord);
}

/*
    Check the dummy payload is used
*/
TEST( protocol, setupNACK_dummyPayload)
{
    vSetupNACK( testPacket, MSG_STATUS_ERROR );
    UNSIGNED_LONGS_EQUAL( MSG_PACKET_FOOTER_SYNC_WORD, testPacket->oPayload.DummyPayload.uMsgFooterSyncWord);
}

/* ToDo msg Id field (Reply??) */ 

/*
    Check the length is correctly set = header + dummy payload
*/
TEST( protocol, setupNACK_messageLength)
{
    vSetupNACK( testPacket, MSG_STATUS_ERROR );
    UNSIGNED_LONGS_EQUAL( sizeof(MsgPacketheader) + sizeof(MsgDummyPayload), testPacket->Msgheader.iLength);
}

/*
    Check the status is set to GOOD
*/
TEST( protocol, setupACK_status)
{
    vSetupACK( testPacket );
    BYTES_EQUAL( MSG_STATUS_GOOD, testPacket->Msgheader.bStatus);
}

/*
    Check the message type is set
*/
TEST( protocol, setupMessage_messageType)
{
    vSetupMessage( testPacket, MSG_CAPTURE_IMAGE, MSG_STATUS_ERROR, NULL, 0 );
    BYTES_EQUAL( MSG_CAPTURE_IMAGE, testPacket->Msgheader.bMsgId);
}

/*
    Check the status is set
*/
TEST( protocol, setupMessage_status)
{
    vSetupMessage( testPacket, MSG_CAPTURE_IMAGE, MSG_STATUS_ERROR, NULL, 0 );
    BYTES_EQUAL( MSG_STATUS_ERROR, testPacket->Msgheader.bStatus);
}

/*
    Check the footer sync word is added to messages with a null payload
    Assumption: the footer sync word is a uint32
    Assumption: the footer sync word is placed after the header NOT in the message type 
                defined position.
*/
TEST( protocol, setupMessage_footerIsAddedToNullPayloads)
{
    vSetupMessage( testPacket, MSG_CAPTURE_IMAGE, MSG_STATUS_ERROR, NULL, 0 );
    BYTES_EQUAL( sizeof( MsgPacketheader ) + sizeof(footerSyncWord_t), testPacket->Msgheader.iLength);
    /* get pointer to start of the payload (just after the header) */
    footerSyncWord_t *start = (footerSyncWord_t *)((int8 *)testPacket + sizeof( MsgPacketheader ));
    UNSIGNED_LONGS_EQUAL( MSG_PACKET_FOOTER_SYNC_WORD, *start);
}
