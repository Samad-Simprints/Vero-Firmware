//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
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
// DESCRIPTION:  Main UN20 server loop
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
#include <time.h>
#include <sys/time.h>

#include "sgfplib.h"
#include "wsq.h"

#include "msg_format.h"
#include "protocol_msg.h"
#include "test_msgs.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int un20_wsq_encode_mem(unsigned char **odata, int *olen, const float r_bitrate,
                   unsigned char *idata, const int w, const int h,
                   const int d, const int ppi, char *comment_text);
#ifdef __cplusplus
}
#endif

#define COMPRESS_IMAGE  0

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600
/* change this definition for the correct port */
//#define MODEMDEVICE "/dev/ttyGS0"
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define CLI_PRINT( str )              printf str

DEBUG_MODULE_DEFINE( INDEX_DEFAULTS ) = {
    "INDEX_DEFAULT", {
    "Error","Hi","Medium","Low" },
    DEBUGZONE(ZONE_DBG_ERROR) | DEBUGZONE(ZONE_DBG_HI) | DEBUGZONE(ZONE_DBG_MED) | DEBUGZONE(ZONE_DBG_LO)
};

#define INDEX_VERSION  QUOTEME(INDEX_REVISION_NUMBER) " : " \
                        "Built: " __DATE__ " " __TIME__
#ifdef __cplusplus
static LPSGFPM  sgfplib = NULL;
#else
static HSGFPM   sgfplib = NULL;
#endif

// image accquisition data
static SGFingerInfo fingerInfo;
static SGDeviceInfoParam deviceInfo;
static BYTE *ImageBuffer;
static int iImageBufferSize;

// template data
static DWORD iTemplateSize;
static BYTE *MinutiaeBuffer = NULL;

// data recovery controls
static bool boImageValid = false;
static bool boTemplateValid = false;

// compression parameters
static float bitrate = 0.75;
static unsigned char *wsq;
static int size;

static bool boAppQuit = false;
static int port_fd;

#if 0
#define TIME_INIT()
#define TIME(res, expr)  (res = expr)
#else
#define TIME_INIT()      struct timeval tstart, tend;
#define TIME(res, expr)  ( gettimeofday(&tstart, NULL), res = expr , gettimeofday(&tend, NULL), print_time(QUOTEME(expr), &tstart, &tend), res )
#endif

static void print_time( char const*expr, struct timeval *tstart, struct timeval *tend )
{
  long ms_start;
  long ms_end;

  ms_start = (long)((tstart->tv_sec*1000LL + tstart->tv_usec/1000));
  ms_end   = (long)((tend->tv_sec*1000LL + tend->tv_usec/1000));

  printf("ms: %06ld [%06ld] (%s)\n", ms_start, (ms_end - ms_start), expr);
}

static int serial_startup(char *port, struct termios *oldtio)
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

int serial_shutdown(int fd, struct termios *oldtio)
{
  /* restore the old port settings */
  tcsetattr(fd,TCSANOW,oldtio);
  close(fd);
}

static int un20_sdk_startup()
{
  long err;

  printf("Configuring UN20 SDK\n");
#ifdef __cplusplus
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
#else
  err = SGFPM_Create(&sgfplib);
  if (!sgfplib)
  {
    printf("ERROR - Unable to instantiate FPM object.\n\n");
    return false;
  }

  err = SGFPM_Init(sgfplib, SG_DEV_AUTO);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to initialize device.\n\n");
     return false;
  }

  err = SGFPM_OpenDevice(sgfplib,0);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to open device.\n\n");
     return false;
  }

  err = SGFPM_GetDeviceInfo(sgfplib,&deviceInfo);
  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to get device information.\n\n");
     return false;
  }
#endif

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

  iImageBufferSize = ( deviceInfo.ImageHeight * deviceInfo.ImageWidth );
  ImageBuffer = (BYTE*) malloc( iImageBufferSize );
  if ( !ImageBuffer )
  {
     printf("ERROR - Unable to alocate image buffer.\n\n");
     return false;
  }

  printf("UN20 SDK initialised\n");

  return true;
}

static int un20_sdk_shutdown()
{
  long err;

  if ( ImageBuffer )
  {
    free( ImageBuffer );
  }

  if ( MinutiaeBuffer )
  {
    free( MinutiaeBuffer );
  }

#ifdef __cplusplus
  if ( sgfplib )
  {
    err = sgfplib->CloseDevice();
    if (err != SGFDX_ERROR_NONE)
    {
       printf("ERROR - Unable to close device\n\n");
       return false;
    }

    err = DestroySGFPMObject(sgfplib);
    if (err != SGFDX_ERROR_NONE)
    {
       printf("ERROR - Unable to destroy FPM object.\n\n");
       return false;
    }
  }
#else
  if ( sgfplib )
  {
    err = SGFPM_CloseDevice(sgfplib);
    if (err != SGFDX_ERROR_NONE)
    {
       printf("ERROR - Unable to close device\n\n");
       return false;
    }

    err = SGFPM_Terminate(sgfplib);
    if (err != SGFDX_ERROR_NONE)
    {
       printf("ERROR - Unable to destroy FPM object.\n\n");
       return false;
    }
  }
#endif

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
      printf(" %02X", *pbData);
      write(port_fd, pbData, 1);
      pbData++;
    }
    printf("\n");
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

  while ( !boAppQuit )
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

static void vSetupReturnFragment( MsgPacket *psPacket, uint8 *pbData, int iSize)
{
  int iStartOffset;
  int iRemaining;
  int iFragmentLength;

  printf("UN20: Fragment request: %d\n", psPacket->oPayload.FragmentRequest.iFragmentNo);

  // use all the data in the input packet as we are about to overwrite them
  iStartOffset = (psPacket->oPayload.DataFragment.iFragmentNo * sizeof(psPacket->oPayload.DataFragment.bFragmentData));

  if ( iStartOffset < iSize )
  {
    iRemaining = ( iSize - iStartOffset );
    iFragmentLength = min(iRemaining, sizeof(psPacket->oPayload.DataFragment.bFragmentData));

    printf("UN20: iStartOffset: %d, iRemaining: %d, iFragmentLength: %d\n", iStartOffset, iRemaining, iFragmentLength);

    psPacket->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
    psPacket->Msgheader.iLength = (sizeof( MsgPacketheader ) + sizeof( MsgFragment ));
    psPacket->Msgheader.bMsgId |= MSG_REPLY;
    psPacket->oPayload.DataFragment.iFragmentNo = psPacket->oPayload.FragmentRequest.iFragmentNo;
    psPacket->oPayload.DataFragment.iFragmentSize = iFragmentLength;
    psPacket->oPayload.DataFragment.iLastFragment = ((iStartOffset + iFragmentLength) >= iSize) ? 1 : 0;
    memcpy(&psPacket->oPayload.DataFragment.bFragmentData, &pbData[iStartOffset], iFragmentLength);
    psPacket->oPayload.DataFragment.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

    printf("UN20: Fragment response: %d, Size: %d, Last: %d\n", 
                          psPacket->oPayload.DataFragment.iFragmentNo,
                          psPacket->oPayload.DataFragment.iFragmentSize,
                          psPacket->oPayload.DataFragment.iLastFragment);
  }
  else
  {
    vSetupNACK( psPacket, MSG_STATUS_ERROR );
  }

}

// create an Image Quality response packet for the supplied packet with the given error code
static void vSetupImageQuality( MsgPacket *psPacket, int16 iScanQuality )
{
  psPacket->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  psPacket->Msgheader.iLength = (sizeof( MsgPacketheader ) + sizeof( MsgImageQuality ));
  psPacket->Msgheader.bMsgId |= MSG_REPLY;
  psPacket->Msgheader.bStatus = 0;
  psPacket->oPayload.ScanQuality.iQuality = iScanQuality;
  psPacket->oPayload.ScanQuality.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;
}

static void vSaveData( char const *path, void *pvData, int iSize)
{
  FILE *raw = fopen(path, "wb");
  if (fwrite(pvData, iSize, 1, raw) != 1)
  {
    printf("UN20: write to %s failed\n", path);
  }
  fclose(raw);
}


// Called when a protocol message has been received from the UN20 or phone.
static void vMessageProcess( MsgInternalPacket *psMsg )
{
  uint8 bSource;
  MsgPacket *psPacket;
  uint16 iMsglength;
  MsgPacketheader *psHeader;
  DWORD quality;
  DWORD templateSizeMax;
  long err, err1;
  bool boSendResponse = true;

  TIME_INIT();

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
      sUn20GetInfoPacket.oPayload.UN20Info.iStoreCount = 5;
      sUn20GetInfoPacket.oPayload.UN20Info.iVersion = 0x4321;
      memcpy(psPacket, &sUn20GetInfoPacket, sUn20GetInfoPacket.Msgheader.iLength);
      break;

    // capture an image
    case MSG_CAPTURE_IMAGE:
      // image and template no longer valid
      boImageValid = false;
      boTemplateValid = false;

      vSetupNACK( psPacket, MSG_STATUS_ERROR );

#ifdef __cplusplus
      err = TIME(err, sgfplib->GetImage(ImageBuffer));
#else
      err = TIME(err, SGFPM_GetImage(sgfplib, ImageBuffer));
#endif

      if ( err == SGFDX_ERROR_NONE )
      {
        vSaveData("/data/un20-image", ImageBuffer, iImageBufferSize);

        boImageValid = true;
        vSetupACK( psPacket );
      }
#if COMPRESS_IMAGE
      //memset(ImageBuffer, 0xff, (deviceInfo.ImageWidth * deviceInfo.ImageHeight));
      err = TIME(err, un20_wsq_encode_mem(&wsq, &size, bitrate, ImageBuffer, deviceInfo.ImageWidth, deviceInfo.ImageHeight, 8, -1, NULL));

      printf("UN20: Raw: %d, Compressed: %d\n", (deviceInfo.ImageWidth * deviceInfo.ImageHeight), size);
#endif
      printf("UN20: Capture Image:(%d)\n", err);
      break;

    // Get image quality of captured image
    case MSG_IMAGE_QUALITY:
      vSetupNACK( psPacket, MSG_STATUS_ERROR );

      if ( boImageValid )
      {
#ifdef __cplusplus
        err = TIME(err, sgfplib->GetImageQuality(deviceInfo.ImageWidth, deviceInfo.ImageHeight, ImageBuffer, &quality));
#else
        err = TIME(err, SGFPM_GetImageQuality(sgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, ImageBuffer, &quality));
#endif

        if ( err == SGFDX_ERROR_NONE )
        {
          vSetupImageQuality( psPacket, (int16)quality );
        }
        printf("UN20: Get Image Quality:(%d, %d)\n", err, quality);
      }
      break;

    // generate template for image
    case MSG_GENERATE_TEMPLATE:
      vSetupNACK( psPacket, MSG_STATUS_ERROR );

      // cannot generate a template without an image
      if ( boImageValid )
      {
#ifdef __cplusplus
        err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
        err |= sgfplib->GetMaxTemplateSize(&templateSizeMax);
#else
        err = SGFPM_SetTemplateFormat(sgfplib, TEMPLATE_FORMAT_ISO19794);
        err |= SGFPM_GetMaxTemplateSize(sgfplib, &templateSizeMax);
#endif

        // allocate space for the template
        MinutiaeBuffer = (BYTE*) realloc(MinutiaeBuffer, templateSizeMax);

        // create finger description record
        fingerInfo.FingerNumber = SG_FINGPOS_UK;
        fingerInfo.ViewNumber = 1;
        fingerInfo.ImpressionType = SG_IMPTYPE_LP;
        fingerInfo.ImageQuality = quality; //0 to 100

        iTemplateSize = 0;
#ifdef __cplusplus
        err |= TIME(err1, sgfplib->CreateTemplate(&fingerInfo, ImageBuffer, MinutiaeBuffer));
        err |= sgfplib->GetTemplateSize(MinutiaeBuffer, &iTemplateSize);
#else
        err |= TIME(err1, SGFPM_CreateTemplate(sgfplib, &fingerInfo, ImageBuffer, MinutiaeBuffer));
        err |= SGFPM_GetTemplateSize(sgfplib, MinutiaeBuffer, &iTemplateSize);
#endif
        if ( err == SGFDX_ERROR_NONE )
        {
          vSaveData("/data/un20-template", MinutiaeBuffer, iTemplateSize);

          boTemplateValid = true;
          vSetupACK( psPacket );
        }
        printf("UN20: Generate Template:([0:%d],%d)\n", err, iTemplateSize);
      }
      break;

  case MSG_GET_IMAGE_FRAGMENT:
      printf("UN20: Get Image Fragment (%d):\n", psMsg->oMsg.oPayload.FragmentRequest.iFragmentNo);

      if ( boImageValid )
      {
        vSetupReturnFragment( psPacket, ImageBuffer, iImageBufferSize );
      }
      else
      {
        vSetupNACK( psPacket, MSG_STATUS_ERROR );
      }
      break;

  case MSG_GET_TEMPLATE_FRAGMENT:
      printf("UN20: Get Template Fragment:(%d)\n", psMsg->oMsg.oPayload.FragmentRequest.iFragmentNo);

      if ( boTemplateValid )
      {
        vSetupReturnFragment( psPacket, MinutiaeBuffer, iTemplateSize );
      }
      else
      {
        vSetupNACK( psPacket, MSG_STATUS_ERROR );
      }
      break;

    case MSG_UN20_SHUTDOWN:
      // Quit the server app, causing the controlling shell to shutdown the system
      printf("*** UN20 server shutdown requested ***\n");
      vSetupACK( psPacket );
      boAppQuit = true;
      break;

    default:
      printf( "*** Rejecting unknown message: %d\n", psMsg->oMsg.Msgheader.bMsgId & ~MSG_REPLY );
      vSetupNACK( psPacket, MSG_STATUS_ERROR );
      break;
  }

  if ( boSendResponse )
  {
    // send response
    vUN20SerialSend( psPacket, psPacket->Msgheader.iLength );
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

  CLI_PRINT(("\nINDEX - SW: " INDEX_VERSION "\n"));

  if ( argc == 2 )
  {
    un20_sdk_startup();
    port_fd = serial_startup(argv[1], &oldtio);

    vProtocolInit();
    vProtocolMsgNotify( vMessageProcess );
    vProtocolMsgError( vMessageErrorCallback );

#if 0
    printf("Please place finger on sensor and press <ENTER> ");
    getc(stdin);
    vIncomingBytes(MSG_SOURCE_UN20_USB, (uint8*)&sCaptureImagePacket, sCaptureImagePacket.Msgheader.iLength);
#else
    receiver(port_fd);
#endif
    un20_sdk_shutdown();
    serial_shutdown(port_fd, &oldtio);
  }
  else
  {
    printf("usage: %s <serial-port>\n", argv[0]);
    iExitStatus = EXIT_FAILURE;
  }

  return iExitStatus;
}
