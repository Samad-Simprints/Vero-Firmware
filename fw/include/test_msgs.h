//******************************************************************************
//
// Project Index source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// MODULE: test_msgs.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Kells header file
//
//******************************************************************************

#if !defined(_TEST_MSGS__H_)
#define _TEST_MSGS__H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************


//******************************************************************************
// Definitions
//******************************************************************************

extern MsgPacket sGetSensorInfoPacket;
extern MsgPacket sSetuiPacket;
extern MsgPacket sSensorConfigPacket;
extern MsgPacket sShutdownUn20Packet;
extern MsgPacket sWakeupUn20Packet;
extern MsgPacket sUn20ReadyPacket;
extern MsgPacket sUn20GetInfoPacket;
extern MsgPacket sCaptureImagePacket;
extern MsgPacket sCaptureProgressPacket;
extern MsgPacket sGetQualityPacket;
extern MsgPacket sGetGenerateTemplatePacket;

extern MsgPacket sGetImageFragment;
extern MsgPacket sGetTemplateFragment;

#ifdef __cplusplus
}
#endif

#endif  // _TEST_MSGS__H_
