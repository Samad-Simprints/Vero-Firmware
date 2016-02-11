//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX UN20 application
// MODULE:       lpcapp.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************
// Indicate we have local debug zones. Disable the default legacy ones
//#define DBG_LOCAL_DEFS

//******************************************************************************
// Includes
//******************************************************************************

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg_format.h"
#include "protocol_msg.h"

extern "C" {
  extern MsgPacket sGetSensorInfoPacket;
  extern MsgPacket sSetuiPacket;
  extern MsgPacket sSensorConfigPacket;
  extern MsgPacket sShutdownUn20Packet;
  extern MsgPacket sWakeupUn20Packet;
  extern MsgPacket sUn20ReadyPacket;
  extern MsgPacket sUn20GetInfoPacket;
  extern MsgPacket sCaptureImagePacket;
  extern MsgPacket sCaptureProgressPacket;
}

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600            
/* change this definition for the correct port */
//#define MODEMDEVICE "/dev/ttyGS0"
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#define CLI_PRINT( str )              printf str

DEBUG_MODULE_DEFINE( INDEX_DEFAULTS ) = {
    "INDEX_DEFAULT", {
    "Error","Hi","Medium","Low" },
    DEBUGZONE(ZONE_DBG_ERROR) | DEBUGZONE(ZONE_DBG_HI) | DEBUGZONE(ZONE_DBG_MED) | DEBUGZONE(ZONE_DBG_LO)
};

static int serial_config(char *port, struct termios *oldtio)
{
  int fd,c, res;
  struct termios newtio;

  printf("Configuring: %s\n", port);

  /* 
  Open modem device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
  */
  fd = open(port, O_RDWR | O_NOCTTY ); 
  if (fd <0) {perror(port); exit(-1); }

  tcgetattr(fd,oldtio); /* save current serial port settings */
  bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

  /* 
  BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
  CRTSCTS : output hardware flow control (only used if the cable has
            all necessary lines. See sect. 7 of Serial-HOWTO)
  CS8     : 8n1 (8bit,no parity,1 stopbit)
  CLOCAL  : local connection, no modem contol
  CREAD   : enable receiving characters
  */
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
 
  /*
  IGNPAR  : ignore bytes with parity errors
  ICRNL   : map CR to NL (otherwise a CR input on the other computer
            will not terminate input)
  otherwise make device raw (no other input processing)
  */
  newtio.c_iflag = IGNPAR;
 
  /*
  Raw output.
  */
  newtio.c_oflag = 0;
 
  /*
  ICANON  : enable canonical input
  disable all echo functionality, and don't send signals to calling program
  */
 //newtio.c_lflag = ICANON;
 
  /* 
  initialize all control characters 
  default values can be found in /usr/include/termios.h, and are given
  in the comments, but we don't need them here
  */
  newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
  newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
  newtio.c_cc[VERASE]   = 0;     /* del */
  newtio.c_cc[VKILL]    = 0;     /* @ */
  newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
  newtio.c_cc[VSWTC]    = 0;     /* '\0' */
  newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
  newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
  newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
  newtio.c_cc[VEOL]     = 0;     /* '\0' */
  newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
  newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
  newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
  newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
  newtio.c_cc[VEOL2]    = 0;     /* '\0' */

  /* 
  now clean the modem line and activate the settings for the port
  */
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  return fd;
}

void sender(int fd)
{
  int i;
  int res;
  unsigned char c;

  printf("Sender\n");

  for (i = 0; i < 256; i++)
  {
    c = (char)i;
//    printf(" >%02X\n", c);
    write(fd, &c, 1);
    c = ~c;

    res = read(fd,&c,1);
    if (res)
    {
      if (c != i)
      {
        printf("Mismatch: %02X:%02X\n", i, c);
      }
      printf(" <%02X\n", (c & 0x0ff));
    }
  }
}

void receiver(int fd)
{
  uint8 bData;
  int res;

  printf("Receiver started\n");

#if 1
  printf("Sizeof: %d\n", sShutdownUn20Packet.Msgheader.iLength );
  for (int i = 0; i < sShutdownUn20Packet.Msgheader.iLength; i++)
  {
    uint8 *data = (uint8*)&sShutdownUn20Packet;
    printf(" %02X\n", data[i]);
  }
  for (int i = 0; i < sShutdownUn20Packet.Msgheader.iLength; i++)
  {
    uint8 *data = (uint8*)&sShutdownUn20Packet;
    getc(stdin);
    printf(" %02X\n", data[i]);
    // deliver bytes to message handler
    vIncomingBytes( MSG_SOURCE_PHONE_BT, &data[i], 1 );
  }
#else
  while (1)
  {
    res = read(fd,&bData,1);
    // Todo: break out if we get an error

    // deliver bytes to message handler
    vIncomingBytes( MSG_SOURCE_PHONE_BT, &bData, 1 );
  }
#endif
}

// Called when a protocol message has been received from the UN20 or phone.
static void vMessageProcess( MsgInternalPacket *psMsg )
{
  uint8 bSource;
  MsgPacket *psPacket;
  uint16 iMsglength;
  MsgPacketheader *psHeader;

  // By the time this callback is called, we know that the message:
  // - has a valid source
  // - has valid header and footer syncwords
  // - has a valid length
  // - has a valid message-id

  bSource = psMsg->eSource;
  psPacket = &(psMsg->oMsg);
  psHeader = &(psMsg->oMsg.Msgheader);
  iMsglength = psHeader->iLength;

  CLI_PRINT(( "vMessageCompleteCallback: From %s, length %d, MsgId 0x%02X\n",
              ( bSource == MSG_SOURCE_PHONE_BT ) ? "HostBT" : ( bSource == MSG_SOURCE_PHONE_USB ) ? "HostUSB" : "UN20USB",
              iMsglength, psMsg->oMsg.Msgheader.bMsgId ));
}

static void vMessageErrorCallback( tMsgError eErrorCode )
{
  CLI_PRINT(( "vMessageErrorCallback: error %d\n", eErrorCode ));

  return;
}

main(int argc, char *argv[])
{
  struct termios oldtio;
  int fd;
  int iExitStatus = EXIT_SUCCESS;

  DEBUG_MODULE_INIT( INDEX_DEFAULTS );

  if ( argc == 2 )
  {
    fd = serial_config(argv[1], &oldtio);
    vProtocolInit();
    vProtocolMsgNotify( vMessageProcess );
    vProtocolMsgError( vMessageErrorCallback );

    receiver(fd);

    /* restore the old port settings */
    tcsetattr(fd,TCSANOW,&oldtio);
  }
  else
  {
    printf("usage: %s <serial-port>\n", argv[0]);
    iExitStatus = EXIT_FAILURE;
  }

  return iExitStatus;
}
