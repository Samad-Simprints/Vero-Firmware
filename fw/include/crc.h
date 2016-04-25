//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2009, 2010. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       crc.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Defines CRC checking API
//
//******************************************************************************

#if !defined(_CRC_H_)
#define _CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************

//
// The polynomial used for the CRC is 2^16 + 2^15 + 2^2 + 1 (0x8005),
// otherwise known as ANSI X3.28
//
#define CRC_POLYNOMIAL                ( 0x8005 )
#define CRC_SEED                      ( 0xfffful )

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Function Prototypes
//******************************************************************************

extern word wCRCgenerateCRC(word wCRC, byte * pabBuffer, int iLength);
extern void vCRCinit( void );

#ifdef __cplusplus
}
#endif

#endif  // _CRC_H_

