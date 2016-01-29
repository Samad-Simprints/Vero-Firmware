#include "FreeRTOS.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_hcitr.h"


static const bt_byte RESET[] = { 0x01, 0x03, 0x0c, 0x00 };
static bt_byte mBuffer[20];

static void sendCallback(void);
static void recvCallback(bt_uint len);


void btport_test(void)
{
	bt_oem_recv(mBuffer, 7, &recvCallback);
	bt_oem_send(RESET, sizeof(RESET), &sendCallback);
}


static void sendCallback(void)
{
}


static void recvCallback(bt_uint len)
{
	bt_oem_recv(mBuffer, 7, &recvCallback);
	bt_oem_send(RESET, sizeof(RESET), &sendCallback);
}

