/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTTASK_H_INCLUDED__
#define __BTTASK_H_INCLUDED__

#include "FreeRTOS.h"
#include "cdbt/bt/bt_std.h"

typedef enum _bttask_Signal
{
	BTTASK_SIG_RX            = 0x00000001,
	BTTASK_SIG_TX            = 0x00000002,
	BTTASK_SIG_BTSIGNAL      = 0x00000004,
	BTTASK_SIG_TIMER         = 0x00000008,
	BTTASK_SIG_WAKEUP        = 0x00000010,
	BTTASK_SIG_BUTTONS       = 0x00000020,
	BTTASK_SIG_BUTTON_REPEAT = 0x00000040,
	BTTASK_SIG_KEYBOARD      = 0x00000080,
	BTTASK_SIG_STORAGE       = 0x00000100,
	BTTASK_SIG_AUDIO         = 0x00000200,
	BTTASK_SIG_ACP           = 0x00000400,
	BTTASK_SIG_DISPLAY       = 0x00000800,
	BTTASK_SIG_RECORDER      = 0x00001000,
	BTTASK_SIG_SCOPORT       = 0x00002000,
	BTTASK_SIG_USB           = 0x00004000,
	BTTASK_SIG_VOLUME        = 0x00008000,
	BTTASK_SIG_CMD           = 0x00010000
} bttask_Signal;

typedef void (*bttask_StartCallback)(void);
typedef void (*bttask_SignalHandler)(void);

void bttask_init(void);
void bttask_setSignal(bt_ulong signal);
void bttask_setSignalFromISR(bt_ulong signal, signed portBASE_TYPE* higherPriorityTaskWoken);
void bttask_setSignalFromTickHook(bt_ulong signal);

void bttask_setAudioSignalHandler(bttask_SignalHandler handler);
void bttask_setRecorderSignalHandler(bttask_SignalHandler handler);
void bttask_setDisplaySignalHandler(bttask_SignalHandler handler);
void bttask_setScoPortSignalHandler(bttask_SignalHandler handler);
void bttask_setUsbSignalHandler(bttask_SignalHandler handler);
void bttask_setVolumeSignalHandler(bttask_SignalHandler handler);
void bttask_setAppCmdSignalHandler(bttask_SignalHandler handler);

void bttask_pal_initTimer(void);
void bttask_pal_initButtons(void);
void bttask_pal_initKeyboard(void);
void bttask_pal_initBluetoothPort(void);
void bttask_pal_initStorage(void);
void bttask_pal_initACP(void);
void bttask_pal_initVolumeControl(void);
unsigned short bttask_pal_getVolume(void);

void bttask_pal_startBluetoothPort(bttask_StartCallback callback);
void bttask_pal_restartBluetoothPort(bttask_StartCallback callback);
void bttask_pal_startStorage(bttask_StartCallback callback);
void bttask_pal_startACP(bttask_StartCallback callback);

void bttask_pal_handleWakeupSignal(void);
void bttask_pal_handleRxSignal(void);
void bttask_pal_handleTxSignal(void);
void bttask_pal_handleTimerSignal(void);
void bttask_pal_handleButtonsSignal(void);
void bttask_pal_handleButtonRepeatSignal(void);
void bttask_pal_handleKeyboardSignal(void);
void bttask_pal_handleStorageSignal(void);
void bttask_pal_handleACPSignal(void);
void bttask_pal_handleAppCmdSignal(void);

#endif // __BTTASK_H_INCLUDED__

