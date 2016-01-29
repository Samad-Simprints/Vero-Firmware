//******************************************************************************
//
// Project NIMBUS Application source file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       ser_dd.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include "lpc18xx_cgu.h"

#include "ser_dd.hpp"

#include "global.h"

#include "gpio_dd.hpp"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "proj_defs.h"


//******************************************************************************
// Constants
//******************************************************************************

#define UART_WORD_LENGTH_5            ( 0 << 0 )
#define UART_WORD_LENGTH_6            ( 1 << 0 )
#define UART_WORD_LENGTH_7            ( 2 << 0 )
#define UART_WORD_LENGTH_8            ( 3 << 0 )
#define UART_STOP_BITS_1              ( 0 << 2 )
#define UART_STOP_BITS_2              ( 1 << 2 )
#define UART_NO_PARITY                ( 0 << 3 )
#define UART_ODD_PARITY               ( 1 << 3 )
#define UART_EVEN_PARITY              ( 3 << 3 )
#define UART_SPACE_PARITY             ( 5 << 3 )
#define UART_MARK_PARITY              ( 7 << 3 )
#define UART_DATA_FORMAT_N81          ( UART_WORD_LENGTH_8 | UART_STOP_BITS_1 | UART_NO_PARITY )
#define UART_DLAB                     ( 1 << 7 )
#define UART_FIFO_ON                  ( 1 << 0 )
#define UART_CLEAR_FIFO               ( 3 << 1 )
#define FIFO_TRIGGER_LEVEL_1          ( 0 << 6 )
#define FIFO_TRIGGER_LEVEL_4          ( 1 << 6 )
#define FIFO_TRIGGER_LEVEL_8          ( 2 << 6 )
#define FIFO_TRIGGER_LEVEL_14         ( 3 << 6 )

#define UART_INTERRUPT_ENABLES        ( 7 )
#define UART_INTERRUPT_SOURCE_MASK    ( 0x0f )
#define UART_SOURCE_RLS               ( 0x06 )
#define UART_SOURCE_THRE              ( 0x02 )
#define UART_SOURCE_RX_TIMEOUT        ( 0x0c )
#define UART_SOURCE_RX                ( 0x04 )
#define UART_SOURCE_MSR               ( 0x00 )
#define UART_SOURCE_NONE              ( 0x01 )

#define UART_LSR_RDR                  ( 1 << 0 )
#define UART_LSR_OE                   ( 1 << 1 )
#define UART_LSR_PE                   ( 1 << 2 )
#define UART_LSR_FE                   ( 1 << 3 )
#define UART_LSR_BI                   ( 1 << 4 )
#define UART_LSR_THRE                 ( 1 << 5 )
#define UART_LSR_TEMT                 ( 1 << 6 )

//******************************************************************************
// Definitions
//******************************************************************************

typedef struct tFractionalSetupEntryTag
{
  dword dwFRestimate;
  byte  bDivAddVal;
  byte  bMulVal;
} tFractionalSetupEntry;

#if defined(CM_HOSTED)
extern "C" void vSerialInit(const IRQn_Type iSetNVICchannel, const xQueueHandle poRxedChars, const xQueueHandle poCharsForTx);
#endif

class tSerialPort : public ISerialPort
{
public:
  tSerialPort(const IRQn_Type iSetNVICchannel,
                            LPC_USARTn_Type const *psSetUart,
                            tGPIOpin * const poSetTxPin,
                            tGPIOpin * const poSetRxPin);

  void                      vFlush();
  void                      vConfigurePort( tLineCoding const *psLineCoding, int iSetRxBuffSize, int iSetTxBuffSize );
  void                      vSetBlockingMode( const tBlockingMode eValue );

  int                       iHasChars();

  int                       iPutchar( byte c );
  int                       iGetchar();

  int                       iPutBuf( char const *pbBuffer, const int iLength );
  int                       iGetBuf( char *pbBuffer, const int iMaxLength );

  void                      vISR( long &rlHigherPriorityTaskWoken );

private:
  bool                      boInitialised;
  bool                      boBlockTransmit;
  bool                      boBlockReceive;
  int                       iRxBuffSize;        // Rx Buffer size
  int                       iTxBuffSize;        // Tx Buffer size
  xQueueHandle              poRxedChars;        // queue holding bytes received from USB host
  xQueueHandle              poCharsForTx;       // queue holding bytes to be sent to USB host
  xQueueHandle              poCmdQueue;         // used for signaling updates to the line coding or control lines
  tLineCoding               sLineCoding;
  byte                      bHandshakeLines;    // virtual handshake lines
  //
  IRQn_Type                 iNVICchannel;       // channel assigned to the UART in the NVIC
  LPC_USARTn_Type          *psUART;             // actual UART
  tGPIOpin * const          poTxPin;            // transmit pin
  tGPIOpin * const          poRxPin;            // receive pin
};

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

static tSerialPort oCom0( USART0_IRQn, (LPC_USARTn_Type *)LPC_USART0, DEBUG_TXD0, DEBUG_RXD0 );
static tSerialPort oCom1( UART1_IRQn, (LPC_USARTn_Type *)LPC_UART1,   BT_TXD,     BT_RXD );
static tSerialPort oCom2( USART2_IRQn, (LPC_USARTn_Type *)LPC_USART2, UN20B_TXD,  UN20B_RXD );
static tSerialPort oCom3( USART3_IRQn, (LPC_USARTn_Type *)LPC_USART3, 0,          0 );


static const tLineCoding sDefaultLineCoding = {
  /*.dwDTERate              =*/ DEFAULT_BAUD_RATE,      // Data terminal rate in bits per second
  /*.bCharFormat            =*/ 0,                      // Number of stop bits - 0=1stop, 1=2stop
  /*.bParityType            =*/ 0,                      // Parity bit type
  /*.bDataBits              =*/ 8                       // Number of data bits
};

#if !defined(CM_HOSTED)

// this table is taken from NXP UM10360 Table 286 on page 315, used for implementing
// fractional baud rates.  The FRestimate is *1000 to allow us to use fixed point
// arithmetics.
static const tFractionalSetupEntry asFractionalSetup[] = {
  { .dwFRestimate = 1000, .bDivAddVal = 0, .bMulVal = 1 },
  { .dwFRestimate = 1067, .bDivAddVal = 1, .bMulVal = 15 },
  { .dwFRestimate = 1071, .bDivAddVal = 1, .bMulVal = 14 },
  { .dwFRestimate = 1077, .bDivAddVal = 1, .bMulVal = 13 },
  { .dwFRestimate = 1083, .bDivAddVal = 1, .bMulVal = 12 },
  { .dwFRestimate = 1091, .bDivAddVal = 1, .bMulVal = 11 },
  { .dwFRestimate = 1100, .bDivAddVal = 1, .bMulVal = 10 },
  { .dwFRestimate = 1111, .bDivAddVal = 1, .bMulVal = 9 },
  { .dwFRestimate = 1125, .bDivAddVal = 1, .bMulVal = 8 },
  { .dwFRestimate = 1133, .bDivAddVal = 2, .bMulVal = 15 },
  { .dwFRestimate = 1143, .bDivAddVal = 1, .bMulVal = 7 },
  { .dwFRestimate = 1154, .bDivAddVal = 2, .bMulVal = 13 },
  { .dwFRestimate = 1167, .bDivAddVal = 1, .bMulVal = 6 },
  { .dwFRestimate = 1182, .bDivAddVal = 2, .bMulVal = 11 },
  { .dwFRestimate = 1200, .bDivAddVal = 1, .bMulVal = 5 },
  { .dwFRestimate = 1214, .bDivAddVal = 3, .bMulVal = 14 },
  { .dwFRestimate = 1222, .bDivAddVal = 2, .bMulVal = 9 },
  { .dwFRestimate = 1231, .bDivAddVal = 3, .bMulVal = 13 },
  { .dwFRestimate = 1250, .bDivAddVal = 1, .bMulVal = 4 },
  { .dwFRestimate = 1267, .bDivAddVal = 4, .bMulVal = 15 },
  { .dwFRestimate = 1273, .bDivAddVal = 3, .bMulVal = 11 },
  { .dwFRestimate = 1286, .bDivAddVal = 2, .bMulVal = 7 },
  { .dwFRestimate = 1300, .bDivAddVal = 3, .bMulVal = 10 },
  { .dwFRestimate = 1308, .bDivAddVal = 4, .bMulVal = 13 },
  { .dwFRestimate = 1333, .bDivAddVal = 1, .bMulVal = 3 },
  { .dwFRestimate = 1357, .bDivAddVal = 5, .bMulVal = 14 },
  { .dwFRestimate = 1364, .bDivAddVal = 4, .bMulVal = 11 },
  { .dwFRestimate = 1375, .bDivAddVal = 3, .bMulVal = 8 },
  { .dwFRestimate = 1385, .bDivAddVal = 5, .bMulVal = 13 },
  { .dwFRestimate = 1400, .bDivAddVal = 2, .bMulVal = 5 },
  { .dwFRestimate = 1417, .bDivAddVal = 5, .bMulVal = 12 },
  { .dwFRestimate = 1429, .bDivAddVal = 3, .bMulVal = 7 },
  { .dwFRestimate = 1444, .bDivAddVal = 4, .bMulVal = 9 },
  { .dwFRestimate = 1455, .bDivAddVal = 5, .bMulVal = 11 },
  { .dwFRestimate = 1462, .bDivAddVal = 6, .bMulVal = 13 },
  { .dwFRestimate = 1467, .bDivAddVal = 7, .bMulVal = 15 },
  { .dwFRestimate = 1500, .bDivAddVal = 1, .bMulVal = 2 },
  { .dwFRestimate = 1533, .bDivAddVal = 8, .bMulVal = 15 },
  { .dwFRestimate = 1538, .bDivAddVal = 7, .bMulVal = 13 },
  { .dwFRestimate = 1545, .bDivAddVal = 6, .bMulVal = 11 },
  { .dwFRestimate = 1556, .bDivAddVal = 5, .bMulVal = 9 },
  { .dwFRestimate = 1571, .bDivAddVal = 4, .bMulVal = 7 },
  { .dwFRestimate = 1583, .bDivAddVal = 7, .bMulVal = 12 },
  { .dwFRestimate = 1600, .bDivAddVal = 3, .bMulVal = 5 },
  { .dwFRestimate = 1615, .bDivAddVal = 8, .bMulVal = 13 },
  { .dwFRestimate = 1625, .bDivAddVal = 5, .bMulVal = 8 },
  { .dwFRestimate = 1636, .bDivAddVal = 7, .bMulVal = 11 },
  { .dwFRestimate = 1643, .bDivAddVal = 9, .bMulVal = 14 },
  { .dwFRestimate = 1667, .bDivAddVal = 2, .bMulVal = 3 },
  { .dwFRestimate = 1692, .bDivAddVal = 9, .bMulVal = 13 },
  { .dwFRestimate = 1700, .bDivAddVal = 7, .bMulVal = 10 },
  { .dwFRestimate = 1714, .bDivAddVal = 5, .bMulVal = 7 },
  { .dwFRestimate = 1727, .bDivAddVal = 8, .bMulVal = 11 },
  { .dwFRestimate = 1733, .bDivAddVal = 11, .bMulVal = 15 },
  { .dwFRestimate = 1750, .bDivAddVal = 3, .bMulVal = 4},
  { .dwFRestimate = 1769, .bDivAddVal = 10, .bMulVal = 13},
  { .dwFRestimate = 1778, .bDivAddVal = 7, .bMulVal = 9},
  { .dwFRestimate = 1786, .bDivAddVal = 11, .bMulVal = 14},
  { .dwFRestimate = 1800, .bDivAddVal = 4, .bMulVal = 5},
  { .dwFRestimate = 1818, .bDivAddVal = 9, .bMulVal = 11},
  { .dwFRestimate = 1833, .bDivAddVal = 5, .bMulVal = 6},
  { .dwFRestimate = 1846, .bDivAddVal = 11, .bMulVal = 13},
  { .dwFRestimate = 1857, .bDivAddVal = 6, .bMulVal = 7},
  { .dwFRestimate = 1867, .bDivAddVal = 13, .bMulVal = 15},
  { .dwFRestimate = 1875, .bDivAddVal = 7, .bMulVal = 8},
  { .dwFRestimate = 1889, .bDivAddVal = 8, .bMulVal = 9},
  { .dwFRestimate = 1900, .bDivAddVal = 9, .bMulVal = 10},
  { .dwFRestimate = 1909, .bDivAddVal = 10, .bMulVal = 11},
  { .dwFRestimate = 1917, .bDivAddVal = 11, .bMulVal = 12},
  { .dwFRestimate = 1923, .bDivAddVal = 12, .bMulVal = 13},
  { .dwFRestimate = 1929, .bDivAddVal = 13, .bMulVal = 14},
  { .dwFRestimate = 1933, .bDivAddVal = 14, .bMulVal = 15},
};

#endif

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   vIRQHandler
   .......................................................................
   Description  : Called when a UART interrupt occurs.

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
   UART0_IRQHandler
   .......................................................................
   Description  : Interrupt handler for Uart0

                  Function name is not coding standards compliant, as it
                  has to match the weakly-linked identifier in
                  LPC1700_Startup.s

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void USART0_IRQHandler( void )
{
  long lHigherPriorityTaskWoken = pdFALSE;

  oCom0.vISR( lHigherPriorityTaskWoken );

  portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

/* -----------------------------------------------------------------------
   UART1_IRQHandler
   .......................................................................
   Description  : Interrupt handler for Uart1

                  Function name is not coding standards compliant, as it
                  has to match the weakly-linked identifier in
                  LPC1700_Startup.s

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */
#if 0
// is delegated to BLuetooth stack code
extern "C" void UART1_IRQHandler( void )
{
  long lHigherPriorityTaskWoken = pdFALSE;

  oCom1.vISR( lHigherPriorityTaskWoken );

  portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}
#endif

/* -----------------------------------------------------------------------
   UART2_IRQHandler
   .......................................................................
   Description  : Interrupt handler for Uart2

                  Function name is not coding standards compliant, as it
                  has to match the weakly-linked identifier in
                  LPC1700_Startup.s

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void USART2_IRQHandler( void )
{
  long lHigherPriorityTaskWoken = pdFALSE;

  oCom2.vISR( lHigherPriorityTaskWoken );

  portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

/* -----------------------------------------------------------------------
   UART3_IRQHandler
   .......................................................................
   Description  : Interrupt handler for Uart3

                  Function name is not coding standards compliant, as it
                  has to match the weakly-linked identifier in
                  LPC1700_Startup.s

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void USART3_IRQHandler( void )
{
  long lHigherPriorityTaskWoken = pdFALSE;

  oCom3.vISR( lHigherPriorityTaskWoken );

  portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

/* -----------------------------------------------------------------------
   tSerialPort::vISR
   .......................................................................
   Description  : Interrupt service routine for all uarts

   Params : rlHigherPriorityTaskWoken - returns pdTRUE if interrupt caused
                                        a higher priority task to become
                                        ready

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSerialPort::vISR( long &rlHigherPriorityTaskWoken )
{
  static char cChar;
  static byte bIIR;
  static byte bLSR;
  static bool boLoop;
  static int i;

  // What caused the interrupt?
  for ( boLoop = true, i = 0;
        ( i < 4 ) && ( boLoop );
        ++i )
  {
    boLoop = false;
    bIIR = psUART->IIR;
    bLSR = psUART->LSR;
    switch( bIIR & UART_INTERRUPT_SOURCE_MASK )
    {
      case UART_SOURCE_RLS:
        // Clear the interrupt.
        // Discard any data
        if ( bLSR & UART_LSR_RDR )
        {
          cChar = psUART->RBR;
        }
        break;

      case UART_SOURCE_NONE:
        // if nothing caused interrupt, the driver may have tried to start
        // transmissions.  In this case, there will be data in the output
        // buffer waiting to be sent.
        if ( bLSR & UART_LSR_TEMT )
        {
          if( xQueueReceiveFromISR( poCharsForTx, &cChar, &rlHigherPriorityTaskWoken ) == pdPASS )
          {
            psUART->THR = cChar;
            boLoop = true;
          }
        }
        break;
      case UART_SOURCE_THRE:
        // The THRE is empty.  If there is another character in the Tx
        // buffer, send it now.
        if( xQueueReceiveFromISR( poCharsForTx, &cChar, &rlHigherPriorityTaskWoken ) == pdPASS )
        {
          psUART->THR = cChar;
          boLoop = true;
        }
        break;

      case UART_SOURCE_RX:
        boLoop = true;
        // fall through to UART_SOURCE_RX_TIMEOUT
      case UART_SOURCE_RX_TIMEOUT:
        // A character was received.  Place it in the queue of received characters.
        // First check for line errors and break conditions.  If they exist, don't
        // keep the received character.
        if ( bLSR & UART_LSR_RDR )
        {
          cChar = psUART->RBR;
          xQueueSendFromISR( poRxedChars, &cChar, &rlHigherPriorityTaskWoken );
        }
        break;

      default:
        // There is nothing to do, leave the ISR.
        break;
    }
  }

  // Clear the ISR in the NVIC.
  NVIC_ClearPendingIRQ( iNVICchannel );
}

/* -----------------------------------------------------------------------
   tSerialPort::tSerialPort
   .......................................................................
   Description  : Constructor

   Params : iSetNVICchannel - uart channel
            psSetUART - pointer to hardware registers for uart

   Returns : Nothing
   ----------------------------------------------------------------------- */

tSerialPort::tSerialPort( const IRQn_Type iSetNVICchannel,
                          LPC_USARTn_Type const *psSetUart,
                          tGPIOpin * const poSetTxPin,
                          tGPIOpin * const poSetRxPin)
  : //ISerialPort(),
    boInitialised( false ),
    boBlockTransmit( false ),
    boBlockReceive( false ),
    iRxBuffSize( 0 ),
    iTxBuffSize( 0 ),
    iNVICchannel( iSetNVICchannel ),
    psUART( (LPC_USARTn_Type *)psSetUart ),
    poTxPin( poSetTxPin ),
    poRxPin( poSetRxPin )
{
}

/* -----------------------------------------------------------------------
   tSerialPort::vFlush
   .......................................................................
   Description  : flush the serial ports and queues

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSerialPort::vFlush()
{
  // - uart
  xQueueReset( poCharsForTx );
  xQueueReset( poRxedChars );
}

/* -----------------------------------------------------------------------
   tSerialPort::vConfigurePort
   .......................................................................
   Description  : Configure port parameters

   Params : ePort - port to configure
            psLineCoding - pointer to parameter structure

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSerialPort::vConfigurePort( tLineCoding const *psLineCoding, int iSetRxBuffSize, int iSetTxBuffSize )
{
#if !defined(CM_HOSTED)
  dword dwDLest,
        dwFRest,
        dwResFRest;
  int i,
      iRxFactor = 1,
      iTxFactor = 1;
  byte  bDivAddVal,
        bMulVal,
        bDataFormat;
  bool boRunning;
#endif
  // passing NULL initialises to default
  if ( psLineCoding == 0 )
  {
    psLineCoding = &sDefaultLineCoding;
  }

  if ( !boInitialised )
  {
    iRxBuffSize = iSetRxBuffSize;
    iTxBuffSize = iSetTxBuffSize;

#if !defined(CM_HOSTED)
    switch (iNVICchannel)
    {
    case USART0_IRQn:
      // - uart 0
      vGPIODDconfigurePin( poTxPin );
      vGPIODDconfigurePin( poRxPin );

      // enable power to uart 0
      CGU_ConfigPWR(CGU_PERIPHERAL_UART0, ENABLE);
      CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_UART0);
      CGU_EnableEntity(CGU_BASE_UART0, ENABLE);

#if 0
      LPC_SC->PCONP |= PCONP_PCUART0;

      // select clk/1 for uart 0
      LPC_SC->PCLKSEL0 |= ( 1 << 6 );
#endif
      break;
    case UART1_IRQn:
      // - uart 1
      vGPIODDconfigurePin( poTxPin );
      vGPIODDconfigurePin( poRxPin );

      // enable power to uart 1
      CGU_ConfigPWR(CGU_PERIPHERAL_UART1, ENABLE);
      CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_UART1);
      CGU_EnableEntity(CGU_BASE_UART1, ENABLE);
#if 0
      LPC_SC->PCONP |= PCONP_PCUART1;

      // select clk/1 for uart 1
      LPC_SC->PCLKSEL0 |= ( 1 << 8 );
#endif
      break;
    case USART2_IRQn:
      // - uart 2
      vGPIODDconfigurePin( poTxPin );
      vGPIODDconfigurePin( poRxPin );

      // enable power to uart 2
      CGU_ConfigPWR(CGU_PERIPHERAL_UART2, ENABLE);
      CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_UART2);
      CGU_EnableEntity(CGU_BASE_UART2, ENABLE);
#if 0
      LPC_SC->PCONP |= PCONP_PCUART2;

      // select clk/1 for uart 2
      LPC_SC->PCLKSEL1 |= ( 1 << 16 );
#endif
      break;
#if 0
    case USART3_IRQn:
      // - uart 3
      vGPIODDconfigurePin( poTxPin );
      vGPIODDconfigurePin( poRxPin );

      // enable power to uart 3
      CGU_ConfigPWR(CGU_PERIPHERAL_UART3, ENABLE);
      CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_UART3);
      CGU_EnableEntity(CGU_BASE_UART3, ENABLE);
#if 0
      LPC_SC->PCONP |= PCONP_PCUART3;

      // select clk/1 for uart 3
      LPC_SC->PCLKSEL1 |= ( 1 << 18 );
#endif
      break;
#endif
    default:
      PBC_ERROR( "Invalid serial port selection" );
      break;
    }
#endif
    poRxedChars = xQueueCreate( iRxBuffSize, sizeof(char) );
    poCharsForTx = xQueueCreate( iTxBuffSize, sizeof(char) );
    sLineCoding = *psLineCoding;

    if( ( poRxedChars == NULL ) || ( poCharsForTx == NULL ) )
    {
      /* Not enough heap available to create the buffer queues, can't do
      anything so just delete ourselves. */
      vTaskDelete( NULL );
      return;
    }
  }
  boInitialised = true;

#if !defined(CM_HOSTED)
  // implement the fractional baud rate calculation, as detailed in NXP UM10360
  // page 314 (Figure 46).  We use fixed point arithmetic with a *100 scale factor.
  dwDLest = ( ( SystemCoreClock / 4 ) * 25 ) / psLineCoding->dwDTERate;
  // - is DLest an integer?
  if ( ( dwDLest % 100 ) == 0 )
  {
    // simple case, no need for fractional divider
    bDivAddVal = 0;
    bMulVal = 1;
  }
  else
  {
    // work out an approximate fractional divider (1.5)
    dwFRest = 150;
    do
    {
      dwDLest = ( ( SystemCoreClock / 4 ) * 25 ) / ( psLineCoding->dwDTERate * dwFRest );
      dwResFRest = ( ( SystemCoreClock / 4 ) * 25 ) / ( psLineCoding->dwDTERate * dwDLest );
      if ( ( dwResFRest > 110 ) && ( dwResFRest < 190 ) )
      {
        // success.  Find divisor and multiplier in lookup table
        for ( i = 0; i < sizeof( asFractionalSetup ) / sizeof( asFractionalSetup[ 0 ] ); ++i )
        {
          if ( asFractionalSetup[ i ].dwFRestimate >= ( 10 * dwResFRest ) )
          {
            bDivAddVal = asFractionalSetup[ i ].bDivAddVal;
            bMulVal = asFractionalSetup[ i ].bMulVal;
            break;
          }
        }
        break;
      }
      else
      {
        // pick 'another' FRest in the range [1.1 .. 1.9] - no guidance given
        // on what would be a good value.  So we pick one other value, then give up
        if ( dwFRest == 180 )
        {
          // already tried another value, give up
          bDivAddVal = 0;
          bMulVal = 1;
          break;
        }
        else
        {
          dwFRest = 180;
        }
      }
    } while ( 1 );

    bDataFormat = ( ( psLineCoding->bDataBits - 5 ) & 0x03 ) |
                  ( ( psLineCoding->bCharFormat ? 1 : 0 ) << 2 );
    if ( psLineCoding->bParityType > 0 )
    {
      bDataFormat |= ( 1 << 3 ) | ( ( ( psLineCoding->bParityType - 1 ) & 0x03 ) << 4 );
    }

    // -- set the DLAB bit so we can access the divisor.
    psUART->LCR = bDataFormat | UART_DLAB;

    // -- setup the divisor.
    psUART->DLL = ( dwDLest & 0xff );
    psUART->DLM = ( dwDLest >> 8 );
    psUART->FDR = ( bMulVal << 4 ) | bDivAddVal;

    // -- turn on the FIFO's and clear the buffers.
    psUART->FCR = ( UART_FIFO_ON | UART_CLEAR_FIFO );

    // -- setup transmission format. Also clears the DLAB bit.
    psUART->LCR = bDataFormat;

    // -- enable UART interrupts
    NVIC_SetPriority( iNVICchannel, SER_DD_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( iNVICchannel );
    psUART->IER = UART_INTERRUPT_ENABLES;
  }
#else
  vSerialInit(iNVICchannel, poRxedChars, poCharsForTx);
#endif
}

void tSerialPort::vSetBlockingMode( const tBlockingMode eValue )
{
  boBlockTransmit = ( ( eValue == bmTransmitOnly ) || ( eValue == bmBoth ) );
  boBlockReceive = ( ( eValue == bmReceiveOnly ) || ( eValue == bmBoth ) );
}

int tSerialPort::iHasChars()
{
  int iResult = 0;

  if ( uxQueueMessagesWaiting( poRxedChars ) != 0 )
  {
    iResult = !0;
  }

  return iResult;
}

/**
        Writes one character to COM port

        @param [in] c character to write
        @returns character written, or EOF if character could not be written
 */
int tSerialPort::iPutchar( byte c )
{
  char cOut = c;
  int iResult = EOF;

  // check that there is enough room, otherwise don't write anything
  do
  {
    if ( xQueueSend( poCharsForTx, &cOut, 1) == pdPASS )
    {
      // rather than trying to remove bytes from the queue and pass them to the
      // serial transmitter in a re-entry safe manner, we just fire the interrupt
      // which will then be sorted nicely by the NVIC and the ISR
#if !defined(CM_HOSTED)
      NVIC_SetPendingIRQ( iNVICchannel );
#endif
      iResult = c;
    }
  } while ( ( iResult == EOF ) && ( boBlockTransmit ) );

  return iResult;
}

/**
        Reads one character from COM port

        @returns character read, or EOF if character could not be read
 */
int tSerialPort::iGetchar()
{
  int iResult = EOF;
  byte c;

  do
  {
    if ( uxQueueMessagesWaiting( poRxedChars ) != 0 )
    {
      xQueueReceive( poRxedChars, &c, 0 );
      iResult = c;
    }
    else
    if ( boBlockReceive )
    {
      vTaskDelay( 1 );
    }
  } while ( ( iResult == EOF ) && ( boBlockReceive ) );

  return iResult;
}

int tSerialPort::iPutBuf( char const *pbBuffer, int iLength )
{
  int iSent = 0;
  bool boSend = true;

  // check that there is enough room, otherwise don't write anything (unless we're blocking)
  if ( !boBlockTransmit )
  {
    int iSpace = iTxBuffSize - uxQueueMessagesWaiting( poCharsForTx );
    if ( iLength > iSpace )
    {
      boSend = false;
    }
  }

  if ( boSend )
  {
    while ( iSent < iLength )
    {
      if ( xQueueSend( poCharsForTx, pbBuffer, 1) == pdPASS )
      {
        ++iSent;
        ++pbBuffer;
      }
      else
      {
        // kick off some serial transmission
        // rather than trying to remove bytes from the queue and pass them to the
        // serial transmitter in a re-entry safe manner, we just fire the interrupt
        // which will then be sorted nicely by the NVIC and the ISR
#if !defined(CM_HOSTED)
        NVIC_SetPendingIRQ(iNVICchannel);
#endif
        vTaskDelay( 1 );
      }
    }
#if !defined(CM_HOSTED)
    // kick off transmission now
    NVIC_SetPendingIRQ( iNVICchannel );
#endif
  }

  return iSent;
}

int tSerialPort::iGetBuf( char *pbBuffer, int iMaxLength )
{
  int iReceived = 0;

  while ( ( uxQueueMessagesWaiting( poRxedChars ) != 0 ) &&
          ( iReceived < iMaxLength ) )
  {
    if ( xQueueReceive( poRxedChars, pbBuffer, 0 ) == pdPASS )
    {
      ++iReceived;
      ++pbBuffer;
    }
    else
    if ( !boBlockReceive )
    {
      break;
    }
  }

  return iReceived;
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

ISerialPort *poSERDDgetPort( const int iPort )
{
  ISerialPort *poRes = 0;

  switch ( iPort )
  {
  case USART0_IRQn:
    poRes = &oCom0;
    break;
  case UART1_IRQn:
    poRes = &oCom1;
    break;
  case USART2_IRQn:
    poRes = &oCom2;
    break;
  case USART3_IRQn:
    poRes = &oCom3;
    break;
  default:
    PBC_ERROR( "Invalid serial port selected" );
    break;
  }

  return poRes;
}

