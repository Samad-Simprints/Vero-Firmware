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
//#include "wsq.h"

#include "msg_format.h"
#include "protocol_msg.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int un20_wsq_encode_mem(unsigned char **odata, int *olen, const float r_bitrate,
                   unsigned char *idata, const int w, const int h,
                   const int d, const int ppi, char *comment_text);
#ifdef __cplusplus
}
#endif

// set to 1 to enable image compression
#define COMPRESS_IMAGE  1

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
//#define BAUDRATE 115200
#define BAUDRATE 230400

/* change this definition for the correct port */
//#define MODEMDEVICE "/dev/ttyGS0"
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define CLI_PRINT( str )              printf str

#define DEBUG_PRINT( str )            ( boDebug ? printf str : 0 )

DEBUG_MODULE_DEFINE( INDEX_DEFAULTS ) = {
    "INDEX_DEFAULT", {
    "Error","Hi","Medium","Low" },
    DEBUGZONE(ZONE_DBG_ERROR) | DEBUGZONE(ZONE_DBG_HI) | DEBUGZONE(ZONE_DBG_MED) | DEBUGZONE(ZONE_DBG_LO)
};

#define INDEX_VERSION  QUOTEME(INDEX_REVISION_NUMBER) " : " \
                        "Built: " __DATE__ " " __TIME__

// SDK object pointer
static LPSGFPM  sgfplib = NULL;

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
static bool boQualityValid = false;

// compression parameters
static float bitrate = 0.75;
static unsigned char *wsq;
static int size;

static bool boAppQuit = false;
static int port_fd;
static bool boDebug = false;

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

  DEBUG_PRINT(("UN20: ms: %06ld [%06ld] (%s)\n", ms_start, (ms_end - ms_start), expr));
}

static int serial_startup(char *port, struct termios *oldtio, int iBaudRate)
{
  int fd,c, res;
  struct termios newtio;
  tcflag_t eBaudRate;

  CLI_PRINT(("Configuring: %s (%dbps)\n", port, iBaudRate));

  switch ( iBaudRate )
  {
    case 9600:
      eBaudRate = B9600;
      break;
    case 115200:
      eBaudRate = B115200;
      break;
    case 230400:
      eBaudRate = B230400;
      break;
    default:
      CLI_PRINT(("Unsupported baud rate\n"));
      return -__LINE__;
  }

  /*
  Open modem device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
  */
  fd = open(port, O_RDWR | O_NOCTTY );
  if ( fd < 0 )
  {
    perror(port);
    return -1;
  }

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
  newtio.c_cflag = eBaudRate | CS8 | CLOCAL | CREAD;

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

  CLI_PRINT(("Configuring UN20 SDK\n"));

  err = CreateSGFPMObject(&sgfplib);
  if (!sgfplib)
  {
    CLI_PRINT(("ERROR - Unable to instantiate FPM object.\n\n"));
    return false;
  }

  err = sgfplib->Init(SG_DEV_AUTO);
  if (err != SGFDX_ERROR_NONE)
  {
     CLI_PRINT(("ERROR - Unable to initialize device(%d).\n\n", err));
     return false;
  }

  err = sgfplib->OpenDevice(0);
  if (err != SGFDX_ERROR_NONE)
  {
     CLI_PRINT(("ERROR - Unable to open device.\n\n"));
     return false;
  }

  err = sgfplib->GetDeviceInfo(&deviceInfo);
  if (err != SGFDX_ERROR_NONE)
  {
     CLI_PRINT(("ERROR - Unable to get device information.\n\n"));
     return false;
  }

  // display information about the device we are using
  CLI_PRINT(("deviceInfo.DeviceID   : %ld\n", deviceInfo.DeviceID));
  CLI_PRINT(("deviceInfo.DeviceSN   : %s\n",  deviceInfo.DeviceSN));
  CLI_PRINT(("deviceInfo.ComPort    : %ld\n", deviceInfo.ComPort));
  CLI_PRINT(("deviceInfo.ComSpeed   : %ld\n", deviceInfo.ComSpeed));
  CLI_PRINT(("deviceInfo.ImageWidth : %ld\n", deviceInfo.ImageWidth));
  CLI_PRINT(("deviceInfo.ImageHeight: %ld\n", deviceInfo.ImageHeight));
  CLI_PRINT(("deviceInfo.Contrast   : %ld\n", deviceInfo.Contrast));
  CLI_PRINT(("deviceInfo.Brightness : %ld\n", deviceInfo.Brightness));
  CLI_PRINT(("deviceInfo.Gain       : %ld\n", deviceInfo.Gain));
  CLI_PRINT(("deviceInfo.ImageDPI   : %ld\n", deviceInfo.ImageDPI));
  CLI_PRINT(("deviceInfo.FWVersion  : %04X\n", (unsigned int) deviceInfo.FWVersion));

  iImageBufferSize = ( deviceInfo.ImageHeight * deviceInfo.ImageWidth );
  ImageBuffer = (BYTE*) malloc( iImageBufferSize );
  if ( !ImageBuffer )
  {
     CLI_PRINT(("ERROR - Unable to alocate image buffer.\n\n"));
     return false;
  }

  CLI_PRINT(("UN20: SDK initialised\n"));

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

  if ( sgfplib )
  {
    err = sgfplib->CloseDevice();
    if (err != SGFDX_ERROR_NONE)
    {
       CLI_PRINT(("ERROR - Unable to close device\n\n"));
       return false;
    }

    err = DestroySGFPMObject(sgfplib);
    if (err != SGFDX_ERROR_NONE)
    {
       CLI_PRINT(("ERROR - Unable to destroy FPM object.\n\n"));
       return false;
    }
  }

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
      CLI_PRINT((" %02X", *pbData));
      write(port_fd, pbData, 1);
      pbData++;
    }
    CLI_PRINT(("\n"));
  }
#else
  return write(port_fd, pvData, iMsglength);
#endif
}

static DWORD setFingerCheck( bool on )
{
  const unsigned char index = 0;
  const unsigned char data = on? 1:0;
  return sgfplib->WriteData( index, data);
}
 
static void receiver(int fd)
{
  uint8 bData;
  int res;
  MsgPacket sUN20Ready;

  CLI_PRINT(("UN20: Receiver started\n"));

  // Send message to indicate UN20 server is running
  vSetupMessage( &sUN20Ready, MSG_UN20_READY, MSG_STATUS_GOOD, NULL, 0 );
  vUN20SerialSend(&sUN20Ready, sUN20Ready.Msgheader.iLength);

  while ( !boAppQuit )
  {
    res = read(fd,&bData,1);
#if 0
    // Todo: break out if we get an error
    CLI_PRINT((" %02X\n", bData));
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

  DEBUG_PRINT(("UN20: Fragment request: %d\n", psPacket->oPayload.FragmentRequest.iFragmentNo));

  // use all the data in the input packet as we are about to overwrite them
  iStartOffset = (psPacket->oPayload.DataFragment.iFragmentNo * sizeof(psPacket->oPayload.DataFragment.bFragmentData));

  if ( (iStartOffset >= 0) && (iStartOffset < iSize) )
  {
    iRemaining = ( iSize - iStartOffset );
    iFragmentLength = min(iRemaining, sizeof(psPacket->oPayload.DataFragment.bFragmentData));

    DEBUG_PRINT(("UN20: iStartOffset: %d, iRemaining: %d, iFragmentLength: %d\n", iStartOffset, iRemaining, iFragmentLength));

    psPacket->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
    psPacket->Msgheader.iLength = (sizeof( MsgPacketheader ) + sizeof( MsgFragment ));
    psPacket->Msgheader.bMsgId |= MSG_REPLY;
    psPacket->oPayload.DataFragment.iFragmentNo = psPacket->oPayload.FragmentRequest.iFragmentNo;
    psPacket->oPayload.DataFragment.iFragmentSize = iFragmentLength;
    psPacket->oPayload.DataFragment.iLastFragment = ((iStartOffset + iFragmentLength) >= iSize) ? 1 : 0;
    memcpy(&psPacket->oPayload.DataFragment.bFragmentData, &pbData[iStartOffset], iFragmentLength);
    psPacket->oPayload.DataFragment.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

    DEBUG_PRINT(("UN20: Fragment response: %d, Size: %d, Last: %d\n",
                          psPacket->oPayload.DataFragment.iFragmentNo,
                          psPacket->oPayload.DataFragment.iFragmentSize,
                          psPacket->oPayload.DataFragment.iLastFragment));
  }
  else
  {
    vSetupNACK( psPacket, MSG_STATUS_ERROR );
  }
}

// save data to flash
static int iSaveData( char const *pzPath, char *pzFileName, void *pvData, int iSize)
{
  char acFileName[200];
  FILE *fRaw = NULL;
  int iCount;
  int iRes = -1;

  // make sure the path exists
  mkdir(pzPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  // create the full filename, if its truncated do not proceed
  iCount = snprintf(acFileName, sizeof(acFileName), "%s/%s", pzPath, pzFileName);

  DEBUG_PRINT(("UN20: Saving data to [%s]\n", acFileName));

  if ( (iCount > 0) && (iCount < sizeof(acFileName)) )
  {
    if ((fRaw = fopen(acFileName, "wb")) != NULL)
    {
      if (fwrite(pvData, iSize, 1, fRaw) == 1)
      {
        iRes = 0;
      }
      fclose(fRaw);
    }
  }

  DEBUG_PRINT(("UN20: Save %s\n", ( iRes < 0 ? "Error" : "Ok")));

  return iRes;
}

static int iSaveDataFixed( char const *pzPath, const char *pzFileName, void *pvData, int iSize)
{
  return iSaveData(pzPath, (char*)pzFileName, pvData, iSize);
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
  MsgUN20Info sUN20Info;

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

  DEBUG_PRINT(( "UN20: vMessageCompleteCallback: From %s, length %d, MsgId 0x%02X\n",
                ( bSource == MSG_SOURCE_PHONE_BT ) ? "HostBT" : ( bSource == MSG_SOURCE_PHONE_USB ) ? "HostUSB" : "UN20USB",
                iMsglength, psMsg->oMsg.Msgheader.bMsgId ));

  // Decode the message type (ignoring the Response bit).
  switch ( psMsg->oMsg.Msgheader.bMsgId & ~MSG_REPLY )
  {
    // request for sensor information
    case MSG_UN20_GET_INFO:
      DEBUG_PRINT(("UN20: GetInfo\n"));
      sUN20Info.iStoreCount = 5;
      sUN20Info.iVersion = 0x4321;
      vSetupMessage( psPacket, (MSG_UN20_GET_INFO | MSG_REPLY), MSG_STATUS_GOOD, &sUN20Info, sizeof(sUN20Info) );
      break;

    // capture an image
    case MSG_CAPTURE_IMAGE:
      // image and template no longer valid
      boImageValid = false;
      boTemplateValid = false;
      boQualityValid = false;


      err = TIME(err, sgfplib->GetImage(ImageBuffer));

      if ( err == SGFDX_ERROR_NONE )
      {
        iSaveDataFixed("/data/debug", "un20-image", ImageBuffer, iImageBufferSize);

        #if COMPRESS_IMAGE
          err1 = TIME(err1, un20_wsq_encode_mem(&wsq, &size, bitrate, ImageBuffer, deviceInfo.ImageWidth, deviceInfo.ImageHeight, 8, -1, NULL));
          iSaveDataFixed("/data/debug", "un20-image-wsq", wsq, size);

          DEBUG_PRINT(("UN20: Compress Image: %d, Raw: %d, Compressed: %d\n", err1, (deviceInfo.ImageWidth * deviceInfo.ImageHeight), size));
        #endif
        boImageValid = true;
        vSetupACK( psPacket );
      }
      else
      {
        int msg_status = MSG_STATUS_SDK_ERROR;
        switch( err )
        {
          case SGFDX_ERROR_WRONG_IMAGE:
            msg_status = MSG_STATUS_SDK_WRONG_IMAGE;
            break;

          case SGFDX_ERROR_INVALID_PARAM:
            msg_status = MSG_STATUS_SDK_INVALID_PARAM;
            break;

          case SGFDX_ERROR_LINE_DROPPED:
            msg_status = MSG_STATUS_SDK_LINE_DROPPED;
            break;

          default:
            break;
        }
        vSetupNACK( psPacket, msg_status );
      }

      DEBUG_PRINT(("UN20: Capture Image:(%d)\n", err));
      break;

    // Get image quality of captured image
    case MSG_IMAGE_QUALITY:
      vSetupNACK( psPacket, MSG_STATUS_NO_IMAGE );

      if ( boImageValid )
      {
        err = TIME(err, sgfplib->GetImageQuality(deviceInfo.ImageWidth, deviceInfo.ImageHeight, ImageBuffer, &quality));

        if ( err == SGFDX_ERROR_NONE )
        {
          MsgImageQuality sImageQuality;
          sImageQuality.iQuality = (int16)quality;
          vSetupMessage( psPacket, (MSG_IMAGE_QUALITY | MSG_REPLY), MSG_STATUS_GOOD, &sImageQuality, sizeof(sImageQuality) );
          boQualityValid = true;
        }
        else
        {
          vSetupNACK( psPacket, MSG_STATUS_SDK_ERROR );
        }
        DEBUG_PRINT(("UN20: Get Image Quality:(%d, %d)\n", err, quality));
      }
      break;

    // generate template for image
    case MSG_GENERATE_TEMPLATE:
    {
      int16 iStatus = MSG_STATUS_NO_IMAGE;

      // cannot generate a template without an image
      if ( boImageValid )
      {
        iStatus = MSG_STATUS_NO_QUALITY;

        if ( boQualityValid )
        {
          iStatus = MSG_STATUS_SDK_ERROR;

          err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
          err |= sgfplib->GetMaxTemplateSize(&templateSizeMax);

          // allocate space for the template
          MinutiaeBuffer = (BYTE*) realloc(MinutiaeBuffer, templateSizeMax);

          // create finger description record
          fingerInfo.FingerNumber = SG_FINGPOS_UK;
          fingerInfo.ViewNumber = 1;
          fingerInfo.ImpressionType = SG_IMPTYPE_LP;
          fingerInfo.ImageQuality = quality; //0 to 100

          iTemplateSize = 0;
          err |= TIME(err1, sgfplib->CreateTemplate(&fingerInfo, ImageBuffer, MinutiaeBuffer));
          err |= sgfplib->GetTemplateSize(MinutiaeBuffer, &iTemplateSize);
          if ( err == SGFDX_ERROR_NONE )
          {
            iSaveDataFixed("/data/debug","un20-template", MinutiaeBuffer, iTemplateSize);

            boTemplateValid = true;
            iStatus = MSG_STATUS_GOOD;
          }
          DEBUG_PRINT(("UN20: Generate Template:([0:%d],%d)\n", err, iTemplateSize));
        }
      }
      vSetupNACK( psPacket, iStatus );

      break;
  }
  case MSG_GET_IMAGE_FRAGMENT:
      DEBUG_PRINT(("UN20: Get Image Fragment (%d):\n", psMsg->oMsg.oPayload.FragmentRequest.iFragmentNo));

      if ( boImageValid )
      {
        vSetupReturnFragment( psPacket, ImageBuffer, iImageBufferSize );
      }
      else
      {
        vSetupNACK( psPacket, MSG_STATUS_NO_IMAGE );
      }
      break;

  case MSG_GET_TEMPLATE_FRAGMENT:
      DEBUG_PRINT(("UN20: Get Template Fragment:(%d)\n", psMsg->oMsg.oPayload.FragmentRequest.iFragmentNo));

      if ( boTemplateValid )
      {
        vSetupReturnFragment( psPacket, MinutiaeBuffer, iTemplateSize );
      }
      else
      {
        vSetupNACK( psPacket, MSG_STATUS_NO_TEMPLATE );
      }
      break;

    case MSG_STORE_IMAGE:
    {
      int16 iStatus;
      int iLength = min(psMsg->oMsg.oPayload.StoreScan.bFilenameLength, sizeof(psMsg->oMsg.oPayload.StoreScan.bFileName));
      psMsg->oMsg.oPayload.StoreScan.bFileName[ iLength ] = 0;

      DEBUG_PRINT(("UN20: Store Image:[%s]:\n", psMsg->oMsg.oPayload.StoreScan.bFileName));

      iStatus = MSG_STATUS_NO_IMAGE;

      if ( boImageValid )
      {
        iStatus = MSG_STATUS_SAVE_ERROR;

        if ( iLength )
        {
          if (iSaveData("/data/capture", (char*)psMsg->oMsg.oPayload.StoreScan.bFileName, ImageBuffer, iImageBufferSize) >= 0)
          {
            iStatus = MSG_STATUS_GOOD;
          }
        }
      }

      vSetupNACK( psPacket, iStatus );
      break;
    }
    case MSG_UN20_SHUTDOWN_NO_ACK:
      // suppress the generation of the ACK response
      boSendResponse = false;
      // fallthru
    case MSG_UN20_SHUTDOWN:
      // Quit the server app, causing the controlling shell to shutdown the system
      CLI_PRINT(("UN20: *** UN20 server shutdown requested (%s) ***\n", (boSendResponse ? "ACK" : "NOACK")));
      vSetupACK( psPacket );
      boAppQuit = true;
      break;

    case MSG_ENABLE_FINGER_CHECK:
    case MSG_DISABLE_FINGER_CHECK:
      vSetupNACK( psPacket, MSG_STATUS_SDK_ERROR );
      if( setFingerCheck((psHeader->bMsgId & ~MSG_REPLY) == MSG_ENABLE_FINGER_CHECK) == SGFDX_ERROR_NONE)
        vSetupACK( psPacket );
      break;

    default:
      CLI_PRINT(( "UN20: *** Rejecting unknown message: %d\n", psMsg->oMsg.Msgheader.bMsgId & ~MSG_REPLY ));
      vSetupNACK( psPacket, MSG_STATUS_UNSUPPORTED );
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
  DEBUG_PRINT(( "UN20: vMessageErrorCallback: error %d\n", eErrorCode ));

  return;
}

main(int argc, char *argv[])
{
  struct termios oldtio;
  int iExitStatus = EXIT_FAILURE;

  DEBUG_MODULE_INIT( INDEX_DEFAULTS );

  CLI_PRINT(("\nINDEX - SW: " INDEX_VERSION "\n"));

  if ( argc == 3 )
  {
    boDebug = true;
  }

  if ( argc >= 2 )
  {
    port_fd = serial_startup(argv[1], &oldtio, BAUDRATE);
    if ( port_fd >= 0 )
    {
      if ( un20_sdk_startup() )
      {
        vProtocolInit();
        vProtocolMsgNotify( vMessageProcess );
        vProtocolMsgError( vMessageErrorCallback );

#if 0
        CLI_PRINT(("Please place finger on sensor and press <ENTER> "));
        getc(stdin);
        vIncomingBytes(MSG_SOURCE_UN20_USB, (uint8*)&sCaptureImagePacket, sCaptureImagePacket.Msgheader.iLength);
#else
        receiver(port_fd);
#endif
        un20_sdk_shutdown();
        serial_shutdown(port_fd, &oldtio);
        iExitStatus = EXIT_SUCCESS;
      }
      else
      {
        serial_shutdown(port_fd, &oldtio);
      }
    }
  }
  else
  {
    CLI_PRINT(("usage: %s <serial-port> [debug]\n", argv[0]));
  }

  return iExitStatus;
}
