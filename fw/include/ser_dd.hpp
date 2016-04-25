//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       ser_dd.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Serial port Device Driver
//
//******************************************************************************

#if !defined(_SER_DD_HPP_)
#define _SER_DD_HPP_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

typedef struct tLineCodingTag
{
  dword   dwDTERate;                          // Data terminal rate in bits per second
  byte    bCharFormat;                        // Number of stop bits
  byte    bParityType;                        // Parity bit type
  byte    bDataBits;                          // Number of data bits
} tLineCoding;

class ISerialPort
{
public:
  enum tBlockingMode {
    bmNone,
    bmTransmitOnly,
    bmReceiveOnly,
    bmBoth
  };
                            ISerialPort() {}
  virtual                  ~ISerialPort() {}

  virtual void              vFlush()                                              = 0;

  virtual void              vConfigurePort( tLineCoding const *psSettings,
                                            int iMaxRxBuffSize,
                                            int iMaxTxBuffSize )                  = 0;
  virtual void              vSetBlockingMode( const tBlockingMode eBlockingMode ) = 0;

  virtual int               iHasChars()                                           = 0;

  virtual int               iPutchar( byte c )                                    = 0;
  virtual int               iGetchar()                                            = 0;

  virtual int               iPutBuf( char const *pbBuffer, const int iLength )    = 0;
  virtual int               iGetBuf( char *pbBuffer, const int iMaxLength )       = 0;
};

//******************************************************************************
// Function Prototypes
//******************************************************************************

extern ISerialPort *poSERDDgetPort( const int iPort );

#endif  // <HEADER_DEFINE>
