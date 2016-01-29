//******************************************************************************
//
// Project NIMBUS Application source file
//
// (c) Fen Technology Ltd. 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       win32_dd.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION: Win32 hosted serial interface functions.
//              Connects CM serial ports to real resources on Win32
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************
// Indicate we have local debug zones. Disable the default legacy ones
#define DBG_LOCAL_DEFS

#define CM_HOSTED_ALM_SIMULATION  ( 1 )   // ALM communication with simulator
#define CM_HOSTED_ALM_SERIAL      ( 0 )   // ALM communication over serial port

//******************************************************************************
// Includes
//******************************************************************************

#include "ser_dd.hpp"

#include "global.h"
#include "cli.h"

#include "gpio_dd.hpp"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "proj_defs.h"

//******************************************************************************
// Definitions
//******************************************************************************
enum {
  ZONE_ERROR,
  ZONE_COMMANDS,
  ZONE_DATA,
  ZONE_NETWORK,
  ZONE_STATE
};


DEBUG_MODULE_DEFINE( W32_FD ) =
{
  "W32_FD",
  { "Error", "Commands", "Data", "Network", "State", 0 },
#ifdef NDEBUG
  0
#else
  DEBUGZONE( ZONE_NETWORK ) | DEBUGZONE( ZONE_STATE )
#endif
};

static bool boPerformCREG( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformCGREG( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformPDP( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformRSSI( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformConfig( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformApply( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformALMState( char **papzArgs, int iInstance, int iNumArgs );
static bool boPerformNoCarrier( char **papzArgs, int iInstance, int iNumArgs );

tParserEntry asW32CLI[ ] =
{
  CLICMD( "creg",  "Set CREG state",  2, "", boPerformCREG, 0 ),
  CLICMD( "cgreg", "Set CGREG state", 2, "", boPerformCGREG, 0 ),
  CLICMD( "rssi",  "Set RSSI value",  2, "", boPerformRSSI, 0 ),
  CLICMD( "pdp",   "Set PDP state",   2, "", boPerformPDP, 0 ),
  CLICMD( "nocarrier", "Set no carrier status", 2, "", boPerformNoCarrier, 0 ),
  CLICMD( "config", "Show status", 1, "", boPerformConfig, 0 ),
  CLICMD( "apply", "Apply configuration", 1, "", boPerformApply, 0 ),
  CLICMD( "alm-state", "Set ALM Sleep state", 2, "", boPerformALMState, 0 ),
};

tParserEntry asW32MainCLI =
{
  DEBUG_MODULE_REF( W32_FD ), "w32", "Win32 simulator commands", "", 1, 0, 0, asW32CLI, ELEMENTSOF( asW32CLI )
};


static int iALMSleepCount = 0;

struct tParams
{
  IRQn_Type    iNVICchannel;
  xQueueHandle poRxedChars;
  xQueueHandle poCharsForTx;
};

#if (CM_HOSTED_ALM_SIMULATION)
//
// Implement a bridge to the ALM simulator
//
#include "ALMPort.h"
#include "ALMSim.h"

class tALMSIMBridge : public tALMport
{
private:
  xQueueHandle              poRxedChars;        // queue holding bytes received from USB host
  xQueueHandle              poCharsForTx;       // queue holding bytes to be sent to USB host

public:
  tALMSIMBridge::tALMSIMBridge(xQueueHandle poRxChars, xQueueHandle poTxChars);
  ~tALMSIMBridge();
  int WriteChar(byte ch);
  int ReadCharTimed(int iTimeout);
  int ReadChar();
};

tALMSIMBridge::tALMSIMBridge(xQueueHandle poRxChars, xQueueHandle poTxChars)
: poRxedChars(poRxChars), poCharsForTx(poTxChars)
{
}
tALMSIMBridge::~tALMSIMBridge()
{
}

int tALMSIMBridge::WriteChar(byte ch)
{
  if ( iALMSleepCount == 0 )
  {
    xQueueSend( poRxedChars, &ch, 0 );
  }
  // need to sleep here to give the UART handler chance to empty the queue
  vTaskDelay( MS_TO_TICKS( 1 ) );

  return 0;
}

int tALMSIMBridge::ReadCharTimed(int iTimeout)
{
  int sRes = EOF;
  byte c;

  vTaskDelay( MS_TO_TICKS( 1 ) );

  if ( iALMSleepCount == 0 )
  {
    if (uxQueueMessagesWaiting(poCharsForTx) != 0)
    {
      xQueueReceive(poCharsForTx, &c, 0);
      sRes = c;
    }
  }
  return sRes;
}

int tALMSIMBridge::ReadChar()
{
  return ReadCharTimed(0);
}

void vALMTask(void *params)
{
  tParams *psParams = (tParams*)params;
  tALMsimulator sSIM;
  tALMSIMBridge sBridge(psParams->poRxedChars, psParams->poCharsForTx);

  // quit if not the ALM channel
  if (psParams->iNVICchannel != UART2_IRQn)
  {
    return;
  }

  while (true)
  {
    (void)sSIM.boALMSim(sBridge, ".\\alm.ini", 1);
  }
}

bool boGetALMSleepStateHosted( void )
{
  bool boALMState = true;

  if ( iALMSleepCount > 0 )
  {
    //iALMSleepCount--;
    boALMState = false;
  }

  return boALMState;
}

static bool boPerformALMState( char **papzArgs, int iInstance, int iNumArgs )
{
  iALMSleepCount = atoi( papzArgs[ 1 ] );

  return true;
}

#endif

#if (CM_HOSTED_ALM_SERIAL)
//
// Implement a link to a serial interface for talking to a real ALM
//
#include "commlib.h"

void vALMTask(void *params)
{
  PORT *poSerialPort;
  sParams *tParams = (sParams*)params;

  // for now just open ALM channel
  if (tParams->iNVICchannel != UART2_IRQn)
  {
    return;
  }

  poSerialPort = PortOpen(0, 15, 115200, 'N', 8, 2);

  if (poSerialPort->error != ASSUCCESS)
    return;

  SetDtr(poSerialPort, true);
  vTaskDelay(MS_TO_TICKS(300));
  SetDtr(poSerialPort, false);

  while (true)
  {
    char c;
    int iCh;

    if (uxQueueMessagesWaiting(tParams->poCharsForTx) != 0)
    {
      xQueueReceive(tParams->poCharsForTx, &c, 0);
      WriteChar(poSerialPort, c);
    }
    iCh = ReadCharTimed(poSerialPort, 1);
    if (iCh != ASBUFREMPTY)
    {
      c = (char)iCh;
      xQueueSend(tParams->poRxedChars, &c, 0);
    }

    vTaskDelay(MS_TO_TICKS(1));
  }
}
#endif

//
// Implement the debug io channel.
// Input/Output uses the Win32 console functions NOT stdio (it will go recursive)
//
// these defined in main.cpp (required by embedded runtime)
extern "C" int __getchar(void);
extern "C" int __putchar(int iChar);

// local implementation of _flsbuf that writes to the serial port queue
int _flsbuf(int c, FILE *stream)
{
  return __putchar(c);
}

// local implementation of getchar() that reads from the serial port queue
extern "C" int getchar(void)
{
  int ch = EOF;

  while (ch == EOF)
  {
    vTaskDelay(1);
    ch = __getchar();
  }

  return ch;
}

void vDebugTask(void *params)
{
  tParams *psParams = (tParams*)params;
  DWORD dwWritten;
  HANDLE hConsole;
  HANDLE hConsoleInput;
  COORD newSize;
  DWORD dwEventCount;
  DWORD dwRead;
  INPUT_RECORD sEvent;

  // quit if not the Debug channel
  if (psParams->iNVICchannel != UART0_IRQn)
  {
    return;
  }

  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

  newSize.X = 100;
  newSize.Y = 1000;
  SetConsoleScreenBufferSize(hConsole, newSize);
  SetConsoleTitle("NIMBUS CM simulator");

  while (true)
  {
    char c;

    // process characters for output
    while (uxQueueMessagesWaiting(psParams->poCharsForTx) != 0)
    {
      xQueueReceive(psParams->poCharsForTx, &c, 0);
      WriteConsole(hConsole, &c, sizeof(c), &dwWritten, NULL);
    }

    // process incoming characters from the console
    GetNumberOfConsoleInputEvents(hConsoleInput, &dwEventCount);

    while (dwEventCount-- > 0)
    {
      ReadConsoleInput(hConsoleInput, &sEvent, 1, &dwRead);

      if (dwRead &&
          sEvent.EventType == KEY_EVENT &&
          sEvent.Event.KeyEvent.bKeyDown &&
          sEvent.Event.KeyEvent.uChar.AsciiChar != 0)
      {
        while (sEvent.Event.KeyEvent.wRepeatCount-- > 0)
        {
          xQueueSend(psParams->poRxedChars, &sEvent.Event.KeyEvent.uChar.AsciiChar, 0);
        }
      }
    }

    vTaskDelay(MS_TO_TICKS(1));
  }
}

//
// Implement a minimal GPRS modem simulation for the hosted CM.
// It translates AT commands to raw UDP socket IO
// Implememts no more than is needed for a best case code path
//
#pragma comment(lib, "Ws2_32.lib")

struct sSocket {
  int     iSocketNumber;
  SOCKET  tSocket;
  struct sockaddr_in tAddr;
  // fields for listening sockets
  bool    boListening;
  bool    boJustOpened;
  bool    boShortRingAck;
  struct sockaddr_in tAddrClient;
  int iAddrClientLength;
};

static void vSocketAllocate(sSocket *psSocket, char *pzHost, char *pzPort)
{
  BOOL boReuse = true;
  u_long boNonBlocking = 1;
  int iError = 0;

  //PBC_ASSERT(psSocket->tSocket == INVALID_SOCKET);
  printf("W32:Allocating socket: %d\n", psSocket->iSocketNumber);

  psSocket->boListening = true;
  psSocket->boJustOpened = true;
  psSocket->boShortRingAck = false;

  if ( ( psSocket->tSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == INVALID_SOCKET )
  {
    printf("W32:Meh! Allocating socket %d FAILED!\n", psSocket->iSocketNumber);
  }
  psSocket->tAddr.sin_family = AF_INET;
  psSocket->tAddr.sin_addr.s_addr = inet_addr(pzHost);
  psSocket->tAddr.sin_port = htons(atoi(pzPort));
  psSocket->iAddrClientLength = sizeof(psSocket->tAddrClient);

  if (setsockopt(psSocket->tSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&boReuse, sizeof(boReuse)) == SOCKET_ERROR)
  {
    iError = WSAGetLastError();
  }

  ioctlsocket(psSocket->tSocket, FIONBIO, &boNonBlocking);

  POSTCOND(iError == 0);
}

static void vSocketFree(sSocket *psSocket)
{
  // asserting here is a bit harsh, as we might close sockets that haven't been opened.  The GPRS modem just
  // ignores the request, and so should we.
//  PBC_ASSERT(psSocket->tSocket != INVALID_SOCKET);
  if ( psSocket->tSocket != INVALID_SOCKET )
  {
    printf("W32:Freeing socket: %d\n", psSocket->iSocketNumber);

    closesocket(psSocket->tSocket);
  }

  psSocket->tSocket = INVALID_SOCKET;
  psSocket->boListening = false;
  psSocket->boJustOpened = false;
  psSocket->boShortRingAck = false;
}

#define BINARY_BUFFER_SIZE            ( LLP_MAX_MESSAGE_SIZE + 20 )
#define HEX_BUFFER_SIZE               ( 2 * BINARY_BUFFER_SIZE )

#define GSM_NOTIFY_CREG               (1 << 0)
#define GSM_NOTIFY_CGREG              (1 << 1)
#define GSM_NOTIFY_PDP_LOSS           (1 << 2)
#define GSM_NOTIFY_NO_CARRIER         (1 << 3)

struct sModemStatus
{
  int  iFlags;
  int  iGSMreg;
  int  iGPRSreg;
  bool boPDPup;
  int  iRSSI;
  bool boNoCarrier;
};
typedef struct sModemStatus tModemStatus;

static tModemStatus sActiveStatus = { 0, 5, 5, false, -80, false };
static tModemStatus sShadowStatus = { 0, 0, 0, false, -80, false };

#define CME_ERROR                     ( "+CME ERROR:" )

// We need to insert a <CR> before every <LF> as the gprs_fd line handler now defines a
// line as being terminated with <CR><LF> (to allow for embedded <LF> in text messages
void vQueueResponseCommon( xQueueHandle hQueue, char *bReply, bool boTerminate )
{
  int i;

  //  printf(">>>%s", bReply);

  for ( i = 0; bReply[ i ]; i++ )
  {
    if ( bReply[ i ] == '\n' )
    {
      char cCR = '\r';
      xQueueSend( hQueue, &cCR, 1000 );
    }
    xQueueSend( hQueue, &bReply[ i ], 1000 );
  }
  if ( boTerminate )
  {
    bReply[ 0 ] = 0;
  }
}

void vQueueResponseConst( xQueueHandle hQueue, char *bReply )
{
  vQueueResponseCommon(hQueue, bReply, false);
}
void vQueueResponse( xQueueHandle hQueue, char *bReply )
{
  vQueueResponseCommon( hQueue, bReply, true );
}

void vModemTask(void *params)
{
  tParams *psParams = (tParams*)params;
  // text buffers for modem interface
  char bBuffer[HEX_BUFFER_SIZE];
  char bReply[HEX_BUFFER_SIZE];
  char bUnsolicted[ HEX_BUFFER_SIZE ];
  // binary buffers for socket transfer
  char bOutput[BINARY_BUFFER_SIZE];
  char bInput[BINARY_BUFFER_SIZE];
  int iFlags;
  int iSMSDelay = 10;

  WSADATA wsaData;
  sSocket sSockets[7];
  sSocket *psServerSocket = NULL;

  // quit if not the modem channel
  if (psParams->iNVICchannel != UART3_IRQn)
  {
    return;
  }

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    printf("W32:Failed to initialise socket layer\n");
    return;
  }

  for ( int iSocketIndex = 0; iSocketIndex < ELEMENTSOF( sSockets ); iSocketIndex++ )
  {
    sSocket *psSkt = &sSockets[ iSocketIndex ];
    psSkt->iSocketNumber = iSocketIndex;
    psSkt->tSocket = INVALID_SOCKET;
    psSkt->boListening = false;
    psSkt->boJustOpened = false;
  }

  int iIndex = 0;
  bool boCollectData = false;
  bReply[ 0 ] = '\0';

  while (true)
  {
    char c;
    int i;

    if (uxQueueMessagesWaiting(psParams->poCharsForTx) != 0)
    {
      xQueueReceive(psParams->poCharsForTx, &c, 0);
      bBuffer[iIndex++] = c;
      bBuffer[iIndex] = '\0';

      if (boCollectData)
      {
        char azBuf[3] = "  ";

        switch (c)
        {
        case 0x08:
          // delete the last character
          iIndex = min(--iIndex, 0);
          break;
        case 0x1a:
          // send the buffered data
          for (i = 0; i < iIndex; i += 2)
          {
            azBuf[0] = bBuffer[i + 0];
            azBuf[1] = bBuffer[i + 1];
            bOutput[i / 2] = (byte)strtol(azBuf, 0, 16);
          }

          if (sendto(psServerSocket->tSocket, bOutput, iIndex / 2, 0, (struct sockaddr *)&psServerSocket->tAddr, sizeof(psServerSocket->tAddr)) == SOCKET_ERROR)
          {
            printf("W32:Meh! Socket send (errno=%d)\n", WSAGetLastError());
          }
          // fall thru to common cleanup
        case 0x1b:
          // throw the data away and start over
//printf( "Done collecting data\n" );
          boCollectData = false;
          iIndex = 0;
          psServerSocket = NULL;
          sprintf(bReply, "OK\n");
          break;
        }
      }
      else
      {
        if (c == 13)
        {
          if ( strncasecmp( bBuffer, "AT+CREG=2", 9 ) == 0 )
          {
            sprintf( bReply, "OK\n+CREG: %d,\"1234\",\"5678\"\n", sActiveStatus.iGSMreg );
          }
          else
          if ( strncasecmp( bBuffer, "AT+CGREG=2", 10 ) == 0 )
          {
            sprintf( bReply, "OK\n+CGREG: %d,\"1234\",\"5678\"\n", sActiveStatus.iGPRSreg );
          }
          else
          if ( strncasecmp( bBuffer, "AT+CREG?", 8 ) == 0 )
          {
            sprintf( bReply, "+CREG: 2,%d,\"1234\",\"5678\"\nOK\n", sActiveStatus.iGSMreg );
          }
          else
          if ( strncasecmp( bBuffer, "AT+CGREG?", 9 ) == 0 )
          {
            sprintf( bReply, "+CGREG: 2,%d,\"1234\",\"5678\"\nOK\n", sActiveStatus.iGPRSreg );
          }
          else
          if (strncasecmp(bBuffer, "AT+cops=?", 9) == 0)
          {
            sprintf(bReply, "+COPS: (2,\"vodafone UK\",,\"23415\"),(3,\"O2 - UK\",,\"23410\"),(3,\"EE\",,\"23430\"),(1,\"Orange\",,\"23433\"),,(0-4),(0,2)\nOK\n");
          }
          else
          if ( strncasecmp( bBuffer, "AT+csq", 6 ) == 0 )
          {
            sprintf( bReply, "+CSQ: %d,0\nOK\n", ((sActiveStatus.iRSSI + 113) / 2) );
          }
          else
          if ( strncasecmp( bBuffer, "AT#servinfo", 11 ) == 0 )
          {
            sprintf( bReply, "#SERVINFO: 628,%d,\"EE\",\"23430\",51,0336,04,1,,\"II\",01,6\nOK\n", sActiveStatus.iRSSI );
          }
          else
          if (strncasecmp(bBuffer, "AT#qss?", 7) == 0)
          {
            sprintf(bReply, "#QSS: 0,1\nOK\n");
          }
          else
          if (strncasecmp(bBuffer, "AT#ccid", 7) == 0)
          {
            //                      ----51--- special to select local host in ALM code
            sprintf(bReply, "#CCID: 8944511605135201649\nOK\n");
          }
          else
          if (strncasecmp(bBuffer, "AT+cgatt=1", 10) == 0)
          { // attach GPRS
            sprintf(bReply, "OK\n");
          }
          else
          if (strncasecmp(bBuffer, "AT+cgdcont=1", 12) == 0)
          { // define PDP context
            sprintf(bReply, "OK\n");
          }
          else
          if (strncasecmp(bBuffer, "AT#sgact=1,1", 12) == 0)
          { // activate pdp context
            sActiveStatus.boPDPup = true;
            sprintf(bReply, "#SGACT: 172.25.124.164\nOK\n");
          }
          else
          if ( strncasecmp( bBuffer, "AT#sgact=1,0", 12 ) == 0 )
          { // disable pdp context
            sActiveStatus.boPDPup = false;
            sprintf( bReply, "OK\n" );
          }
          else
          if (strncasecmp(bBuffer, "AT#sgact?", 9) == 0)
          { // report pdp context state
            sprintf( bReply, "#SGACT: 1,%c\nOK\n", (sActiveStatus.boPDPup ? '1' : '0') );
          }
          else
          if ( strncasecmp( bBuffer, "AT#qdns=", 8 ) == 0 )
          {
            char *pzHostAddr = strtok( &bBuffer[ 8 ], "," );
            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
            sprintf( bReply, "#QDNS: \"%s\",\"%s\"\nOK\n", pzHostAddr, pzHostAddr );
          }
          else
          if (strncasecmp(bBuffer, "AT#sludp=", 9) == 0)
          {
            char *pzSocketId = strtok(&bBuffer[9], ",");
            char *pzMode = strtok(NULL, ",");
            char *pzPort = strtok(NULL, ",");

            sSocket *psSkt = &sSockets[atoi(pzSocketId)];

            if (atoi(pzMode) == 1)
            {
              //vSocketAllocate(psSkt, "127.0.0.1", pzPort);
              vSocketAllocate( psSkt, "0.0.0.0", pzPort );

              if (bind(psSkt->tSocket, (struct sockaddr *)&psSkt->tAddr, sizeof(psSkt->tAddr)) < 0)
              {
                printf("bind! - 0x%x\n", WSAGetLastError());
              }
            }
            else
            {
              vSocketFree(psSkt);
            }
            // open a listening socket
            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
            sprintf( bReply, "OK\n" );
          }
          else
          if (strncasecmp(bBuffer, "AT#sa=", 6) == 0)
          {
            char *pzSocketId = strtok(&bBuffer[6], ",");
            sSocket *psSkt = &sSockets[atoi(pzSocketId)];

            psSkt->boShortRingAck = true;

            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
            sprintf(bReply, "OK\n");
          }
          else
          if (strncasecmp(bBuffer, "AT#sd=", 6) == 0)
          {
            //AT#sd = 2, 1, 5000, "10.0.10.77", 0, 5002, 1
            char *pzSocketId = strtok(&bBuffer[6], ",");
            char *pzProtocol = strtok(NULL, ",");
            char *pzPort = strtok(NULL, ",");
            char *pzHost = strtok(NULL, ",\"");

            sSocket *psSkt = &sSockets[atoi(pzSocketId)];

            vSocketAllocate(psSkt, pzHost, pzPort);

            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
            sprintf( bReply, "OK\n" );
          }
          else
          if (strncasecmp(bBuffer, "AT#sh=", 6) == 0)
          {
            //printf( "[%s]\n", bBuffer );
            char *pzSocketId = strtok(&bBuffer[6], ",");

            sSocket *psSkt = &sSockets[atoi(pzSocketId)];

            vSocketFree(psSkt);

            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
            sprintf( bReply, "OK\n" );
          }
          else
          if (strncasecmp(bBuffer, "AT#ssend=", 9) == 0)
          {
            // send on a socket
            char *pzSocketId = strtok(&bBuffer[9], ",");
            psServerSocket = &sSockets[atoi(pzSocketId)];
            if ( psServerSocket->tSocket == INVALID_SOCKET )
            {
              sprintf(bReply, "ERROR\n");
            }
            else
            {
              sprintf(bReply, "> \n");
              boCollectData = true;
            }
            sActiveStatus.iFlags |= GSM_NOTIFY_PDP_LOSS;
          }
          else
          if ( strncasecmp( bBuffer, "AT#SS", 5 ) == 0 )
          {
            for ( int iSocket = 1; iSocket < ELEMENTSOF( sSockets ); iSocket++ )
            {
              sSocket *psSkt = &sSockets[ iSocket ];
              sprintf(bReply, "#SS: %d,%d\n", psSkt->iSocketNumber, 0);
              vQueueResponse( psParams->poRxedChars, bReply );
            }
            sprintf( bReply, "OK\n" );
          }
          else
          if ( strncasecmp( bBuffer, "AT+CMGL=\"ALL\"", 13 ) == 0 )
          {
            if ( --iSMSDelay <= 0  )
            {
              //1.  vQueueResponseConst( psParams->poRxedChars, "+CMGL: 1, \"REC READ\", \"+447924864514\", \"\", \"15/03/17,17:20:44+00\"\n");
              //2.a vQueueResponseConst( psParams->poRxedChars, "  NETWORK-NUDGE Ignore This\n" );
              //2.b vQueueResponseConst( psParams->poRxedChars, "  RESET Ignore This\n" );
            }
            sprintf( bReply, "OK\n" );
          }
          else
          if ( strncasecmp( bBuffer, "AT+CMGD=1,0", 11 ) == 0 )
          {
            iSMSDelay = 10;
            sprintf( bReply, "OK\n" );
          }
          else
          {
            // default is OK
            sprintf(bReply, "OK\n");
          }
        }
      }

      // if we have changed configuration deliver any necessary notification codes.
      if ( (iFlags = sActiveStatus.iFlags) != 0 )
      {
        sActiveStatus.iFlags = 0;

        // if requested generate a NO CARRIER response
        if ( (iFlags & GSM_NOTIFY_NO_CARRIER) )
        {
          sprintf( bUnsolicted, "NO CARRIER\n");
          vQueueResponse( psParams->poRxedChars, bUnsolicted );
        }
        
        // if command was PDP related and its down report a CME ERROR
        if ( ( iFlags & GSM_NOTIFY_PDP_LOSS ) && !sActiveStatus.boPDPup )
        {
          sprintf( bUnsolicted, "+CME ERROR:%d\n", 556 );
          vQueueResponse( psParams->poRxedChars, bUnsolicted );
        }
        // if CREG status has changed generate a notification
        if ( iFlags & GSM_NOTIFY_CREG )
        {
          sprintf( bUnsolicted, "+CREG:%d\n", sActiveStatus.iGSMreg);
          vQueueResponse( psParams->poRxedChars, bUnsolicted );
        }
        // if CGREG status has changed generate a notification
        if ( iFlags & GSM_NOTIFY_CGREG )
        {
          sprintf( bUnsolicted, "+CGREG:%d\n", sActiveStatus.iGPRSreg);
          vQueueResponse( psParams->poRxedChars, bUnsolicted );
        }
      }

      // send a response if one is available
      if (bReply[0])
      {
        vQueueResponse( psParams->poRxedChars, bReply );
        bReply[0] = 0;
        iIndex = 0;
      }
    }

    // now poll listening sockets for data
    for (int iSocket = 0; iSocket < ELEMENTSOF(sSockets); iSocket++)
    {
      sSocket *psSkt = &sSockets[iSocket];
      bReply[0] = 0;

      if (psSkt->boListening)
      {
        int iByteCount;

        // sniff to see if there's any data there, so we avoid blocking
        iByteCount = recvfrom(psSkt->tSocket, bInput, sizeof(bInput), MSG_PEEK, (struct sockaddr *)&psSkt->tAddrClient, &psSkt->iAddrClientLength);

        if (iByteCount >= 0)
        {
          if (iByteCount != 0)
          {
            if (psSkt->boJustOpened)
            {
              sprintf(bReply, "SRING: %d\n", psSkt->iSocketNumber);
              psSkt->boJustOpened = false;
              psSkt->boShortRingAck = false;

              // send reply
              vQueueResponse( psParams->poRxedChars, bReply );
            }
            else
            {
              char *pzResponse;

              if (psSkt->boShortRingAck)
              {
                // now get the data for real
                iByteCount = recvfrom(psSkt->tSocket, bInput, sizeof(bInput), 0, (struct sockaddr *)&psSkt->tAddrClient, &psSkt->iAddrClientLength);

                // we are only allowed to send 64 bytes (128 hex chars) for every SRING, so break things up a bit here
                int iRemaining = iByteCount;
                int iChunkStart = 0;
                while ( iRemaining > 0 )
                {
                  int iCount = min( 64, iRemaining );

                  pzResponse = bReply;
                  pzResponse += sprintf(bReply, "SRING: \"%d.%d.%d.%d\",%d,%d,%d,%d,",
                    psSkt->tAddrClient.sin_addr.S_un.S_un_b.s_b1,
                    psSkt->tAddrClient.sin_addr.S_un.S_un_b.s_b2,
                    psSkt->tAddrClient.sin_addr.S_un.S_un_b.s_b3,
                    psSkt->tAddrClient.sin_addr.S_un.S_un_b.s_b4,
                    ntohs(psSkt->tAddrClient.sin_port), 
                    psSkt->iSocketNumber, 
                    iCount, 
                    // "remaining" means after this SRING
                    iRemaining - iCount);

                  // max 64 bytes at a time
                  for (int i = 0; i < iCount; i++)
                  {
                    byte bData = bInput[i + iChunkStart];
                    pzResponse += sprintf( pzResponse, "%2.2x", bData );
                  }
                  pzResponse += sprintf( pzResponse, "\n" );

                  iRemaining -= iCount;
                  iChunkStart += iCount;

                  // send reply
//printf(">>%s", bReply);
                  vQueueResponse( psParams->poRxedChars, bReply );

                  // implement inter-SRING delay
                  vTaskDelay( MS_TO_TICKS( 30 ) );
                }
              }
            }
          }
          // only handle data from one socket at a time
          break;
        }
      }
    }

    vTaskDelay(MS_TO_TICKS(1));

  }
}

//
// create the appropriate task based on the NVIC channel
//
extern "C" void vSerialInit(const IRQn_Type iSetNVICchannel, const xQueueHandle poSetRxedChars, const xQueueHandle poSetCharsForTx)
{
  static bool boInit = false;

  tParams *psParam = (tParams*)malloc(sizeof(tParams));
  INVARIANT( psParam != 0 );
  
  if ( !boInit )
  {
    boInit = true;
    DEBUG_MODULE_INIT( W32_FD );
    boCLIregisterEntry( &asW32MainCLI );
  }

  psParam->iNVICchannel = iSetNVICchannel;
  psParam->poCharsForTx = poSetCharsForTx;
  psParam->poRxedChars = poSetRxedChars;

  switch (iSetNVICchannel)
  {
    // debug channel
    case UART0_IRQn:
      // Create the worker thread
      xTaskCreate(vDebugTask, (const char *)"w32_dbg", ALM_TASK_STACK_SIZE, psParam, CLI_TASK_PRIORITY, NULL);
      break;
      // ALM channel
    case UART2_IRQn:
      xTaskCreate(vALMTask, (const char *)"w32_alm", ALM_TASK_STACK_SIZE, psParam, CLI_TASK_PRIORITY, NULL);
      break;
      // gprs modem
    case UART3_IRQn:
      xTaskCreate(vModemTask, (const char *)"w32_gprs", ALM_TASK_STACK_SIZE, psParam, CLI_TASK_PRIORITY, NULL);
      break;
  }
}

void vWriteBlockHosted( byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen )
{
  DWORD dwAddr = (((pabCommand[ 1 ] << 16 | pabCommand[ 2 ] << 8 | pabCommand[ 3 ]) >> 10) * 528);
  static byte abFlashBuffer[ 528 ];
  DWORD dwWritten;
  HANDLE hFlashFile;
  static byte abEraseBuffer[ 528 ];
  int iScan;

  switch ( pabCommand[ 0 ] )
  {
  case 0x84:
    // copy data to flash buffer
    memcpy( abFlashBuffer, pabData, iDataLen );
    break;
  case 0x83:
    // write buffer data to flash
    hFlashFile = CreateFile( "FLASH.BIN", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    SetFilePointer( hFlashFile, dwAddr, NULL, FILE_BEGIN );
    WriteFile( hFlashFile, abFlashBuffer, sizeof(abFlashBuffer), &dwWritten, NULL );
    CloseHandle( hFlashFile );
    break;
  case 0x81:
    // erase page in flash
    memset( abEraseBuffer, 0xff, sizeof(abEraseBuffer) );
    hFlashFile = CreateFile( "FLASH.BIN", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    SetFilePointer( hFlashFile, dwAddr, NULL, FILE_BEGIN );
    WriteFile( hFlashFile, abEraseBuffer, sizeof(abEraseBuffer), &dwWritten, NULL );
    CloseHandle( hFlashFile );
    break;
  case 0x50:
    // erase block of 8 pages in flash
    memset( abEraseBuffer, 0xff, sizeof(abEraseBuffer) );
    hFlashFile = CreateFile( "FLASH.BIN", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    SetFilePointer( hFlashFile, dwAddr, NULL, FILE_BEGIN );
    for ( iScan = 0; iScan < 8; ++iScan )
    {
      WriteFile( hFlashFile, abEraseBuffer, sizeof(abEraseBuffer), &dwWritten, NULL );
    }
    CloseHandle( hFlashFile );
    break;
  default:
    PBC_ERROR( "Unknown SSP command for WriteBlock\n" );
    break;
  }
}

void vReadBlockHosted( byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen )
{
  DWORD dwAddr = (((pabCommand[ 1 ] << 16 | pabCommand[ 2 ] << 8 | pabCommand[ 3 ]) >> 10) * 528);
  DWORD dwRead;
  HANDLE hFlashFile;

  // fill result incase the read fails (will look like erased flash)
  memset( pabData, 0xff, iDataLen );

  switch ( pabCommand[ 0 ] )
  {
  case 0xD7:
    // return device ready status
    *pabData = 0x80;
    break;
  case 0x0B:
    // read data from device
    hFlashFile = CreateFile( "FLASH.BIN", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    SetFilePointer( hFlashFile, dwAddr, NULL, FILE_BEGIN );
    ReadFile( hFlashFile, pabData, iDataLen, &dwRead, NULL );
    CloseHandle( hFlashFile );
    break;
  default:
    PBC_ERROR( "Unknown SSP command for ReadBlock\n" );
    break;
  }
}


static bool boPerformCREG( char **papzArgs, int iInstance, int iNumArgs )
{
  sShadowStatus.iGSMreg = atoi( papzArgs[ 1 ] );
  return true;
}

static bool boPerformCGREG( char **papzArgs, int iInstance, int iNumArgs )
{
  sShadowStatus.iGPRSreg = atoi( papzArgs[ 1 ] );
  return true;
}

static bool boPerformPDP( char **papzArgs, int iInstance, int iNumArgs )
{
  sShadowStatus.boPDPup = (atoi( papzArgs[ 1 ] ) ? true : false);
  return true;
}

static bool boPerformRSSI( char **papzArgs, int iInstance, int iNumArgs )
{
  sShadowStatus.iRSSI = atoi( papzArgs[ 1 ] );
  return true;
}

static bool boPerformNoCarrier( char **papzArgs, int iInstance, int iNumArgs )
{
  sActiveStatus.boNoCarrier = ( atoi( papzArgs[ 1 ] ) ? true : false );
  return true;
}

static bool boPerformConfig( char **papzArgs, int iInstance, int iNumArgs )
{
  printf( "Active: CREG:%d, CGREG:%d, RSSI:%d, PDP:%s, Carrier:%d\n",
            sActiveStatus.iGSMreg, sActiveStatus.iGPRSreg, sActiveStatus.iRSSI, (sActiveStatus.boPDPup ? "up" : "down"), sActiveStatus.boNoCarrier );
  printf( "Shadow: CREG:%d, CGREG:%d, RSSI:%d, PDP:%s, Carrier:%d\n",
            sShadowStatus.iGSMreg, sShadowStatus.iGPRSreg, sShadowStatus.iRSSI, (sShadowStatus.boPDPup ? "up" : "down"), sShadowStatus.boNoCarrier );
  printf( "ALM: %s\n", (iALMSleepCount > 0) ? "Asleep" : "Awake" );

  return true;
}

static bool boPerformApply( char **papzArgs, int iInstance, int iNumArgs )
{
  tModemStatus sTemp;
  
  taskDISABLE_INTERRUPTS();

  if ( sActiveStatus.boNoCarrier != sShadowStatus.boNoCarrier )
  {
    sShadowStatus.iFlags |= GSM_NOTIFY_NO_CARRIER;
  }

  if ( sActiveStatus.iGSMreg != sShadowStatus.iGSMreg )
  {
    sShadowStatus.iFlags |= GSM_NOTIFY_CREG;
  }

  if ( sActiveStatus.iGPRSreg != sShadowStatus.iGPRSreg )
  {
    sShadowStatus.iFlags |= GSM_NOTIFY_CGREG;
  }

  sTemp = sActiveStatus;
  sActiveStatus = sShadowStatus;
  sShadowStatus = sTemp;

  taskENABLE_INTERRUPTS();

  return true;
}

