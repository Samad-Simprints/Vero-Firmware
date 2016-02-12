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

#include "sgfplib.h"

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

static LPSGFPM  sgfplib = NULL;
static SGFingerInfo fingerInfo;
static SGDeviceInfoParam deviceInfo;
static BYTE *ImageBuffer;

static int port_fd;

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

static int un20_sdk_config()
{
  long err;

  printf("Configuring UN20 SDK\n");

  err = CreateSGFPMObject(&sgfplib);
  if (!sgfplib)
  {
    printf("ERROR - Unable to instantiate FPM object.\n\n");
    return false;
  }

  err = sgfplib->Init(SG_DEV_AUTO);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to initialize device.\n\n");
     return false;
  }

  err = sgfplib->OpenDevice(0);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to open device.\n\n");
     return false;
  }

  err = sgfplib->GetDeviceInfo(&deviceInfo);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to get device information.\n\n");
     return false;
  }

  // display information about the device we are using
  printf("deviceInfo.DeviceID   : %ld\n", deviceInfo.DeviceID);
  printf("deviceInfo.DeviceSN   : %s\n",  deviceInfo.DeviceSN);
  printf("deviceInfo.ComPort    : %ld\n", deviceInfo.ComPort);
  printf("deviceInfo.ComSpeed   : %ld\n", deviceInfo.ComSpeed);
  printf("deviceInfo.ImageWidth : %ld\n", deviceInfo.ImageWidth);
  printf("deviceInfo.ImageHeight: %ld\n", deviceInfo.ImageHeight);
  printf("deviceInfo.Contrast   : %ld\n", deviceInfo.Contrast);
  printf("deviceInfo.Brightness : %ld\n", deviceInfo.Brightness);
  printf("deviceInfo.Gain       : %ld\n", deviceInfo.Gain);
  printf("deviceInfo.ImageDPI   : %ld\n", deviceInfo.ImageDPI);
  printf("deviceInfo.FWVersion  : %04X\n", (unsigned int) deviceInfo.FWVersion);

  ImageBuffer = (BYTE*) malloc(deviceInfo.ImageHeight * deviceInfo.ImageWidth);
  if ( !ImageBuffer )
  {
     printf("ERROR - Unable to alocate image buffer.\n\n");
     return false;
  }

  printf("UN20 SDK initialised\n");

  return true;
}

static int vUN20SerialSend(void *pvData, int iMsglength)
{
//  CLI_PRINT(( "vUN20SerialSend: length %d\n", iMsglength ));

#if 0
  {
    uint8 *pbData = (uint8 *)pvData;
    while ( iMsglength-- > 0 )
    {
      vIncomingBytes( MSG_SOURCE_UN20_USB, pbData, 1 );
      pbData++;
    }
  }
#else
  return write(port_fd, pvData, iMsglength);
#endif
}

static void receiver(int fd)
{
  uint8 bData;
  int res;

  printf("Receiver started\n");

  vUN20SerialSend(&sUn20ReadyPacket, sUn20ReadyPacket.Msgheader.iLength);

  while (1)
  {
    res = read(fd,&bData,1);
#if 0
    // Todo: break out if we get an error
    printf(" %02X\n", bData);
#endif
    // deliver bytes to message handler
    vIncomingBytes( MSG_SOURCE_UN20_USB, &bData, 1 );
  }
}

static DWORD templateSize;
static BYTE *minutiaeBuffer1 = NULL;

// Called when a protocol message has been received from the UN20 or phone.
static void vMessageProcess( MsgInternalPacket *psMsg )
{
  uint8 bSource;
  MsgPacket *psPacket;
  uint16 iMsglength;
  MsgPacketheader *psHeader;
  DWORD quality;
  DWORD templateSizeMax;
  long err;

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

  // Decode the message type (ignoring the Response bit).
  switch ( psMsg->oMsg.Msgheader.bMsgId & ~MSG_REPLY )
  {
    // request for sensor information
    case MSG_UN20_GET_INFO:
      printf("UN20: GetInfo\n");
      sUn20GetInfoPacket.oPayload.UN20Info.iStoreCount = 23;
      sUn20GetInfoPacket.oPayload.UN20Info.iVersion = 0x4321;
      vUN20SerialSend(&sUn20GetInfoPacket, sUn20GetInfoPacket.Msgheader.iLength);
      break;

    // capture an image
    case MSG_CAPTURE_IMAGE:
      err = sgfplib->GetImage(ImageBuffer);
      printf("UN20: Capture Image:(%d)\n", err);
      break;

    // Get image quality of captured image
    case MSG_IMAGE_QUALITY:
      err = sgfplib->GetImageQuality(deviceInfo.ImageWidth, deviceInfo.ImageHeight, ImageBuffer, &quality);
      printf("UN20: Get Image Quality:(%d, %d)\n", err, quality);
      break;

    // generate template for image
    case MSG_GENERATE_TEMPLATE:
      err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
      err |= sgfplib->GetMaxTemplateSize(&templateSizeMax);

      minutiaeBuffer1 = (BYTE*) realloc(minutiaeBuffer1, templateSizeMax);
      fingerInfo.FingerNumber = SG_FINGPOS_UK;
      fingerInfo.ViewNumber = 1;
      fingerInfo.ImpressionType = SG_IMPTYPE_LP;
      fingerInfo.ImageQuality = quality; //0 to 100

      templateSize = 0;
      err |= sgfplib->CreateTemplate(&fingerInfo, ImageBuffer, minutiaeBuffer1);
      err |= sgfplib->GetTemplateSize(minutiaeBuffer1, &templateSize);
      printf("UN20: Generate Template:([0:%d],%d)\n", err, templateSize);
      break;

    // return template for image
    case MSG_RECOVER_TEMPLATE:
      // send minutiaeBuffer1, &templateSize
      break;

    default:
      break;
  }

}

static void vMessageErrorCallback( tMsgError eErrorCode )
{
  CLI_PRINT(( "vMessageErrorCallback: error %d\n", eErrorCode ));

  return;
}

main(int argc, char *argv[])
{
  struct termios oldtio;
  int iExitStatus = EXIT_SUCCESS;

  DEBUG_MODULE_INIT( INDEX_DEFAULTS );

  if ( argc == 2 )
  {
    un20_sdk_config();
    port_fd = serial_config(argv[1], &oldtio);
    vProtocolInit();
    vProtocolMsgNotify( vMessageProcess );
    vProtocolMsgError( vMessageErrorCallback );

    receiver(port_fd);

    /* restore the old port settings */
    tcsetattr(port_fd,TCSANOW,&oldtio);
  }
  else
  {
    printf("usage: %s <serial-port>\n", argv[0]);
    iExitStatus = EXIT_FAILURE;
  }

  return iExitStatus;
}
