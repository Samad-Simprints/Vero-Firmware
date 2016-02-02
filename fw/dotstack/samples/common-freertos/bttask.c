/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "config.h"

#include "bttask.h"
#include "btapp.h"
#include "bttimer.h"
#include "btmgr.h"
#include "btpbmgr.h"
#include "btlog.h"
#include "btdisplay.h"
#ifdef BTAPP_ENABLE_STATS
	#include "btstats.h"
#endif

#ifdef BTAPP_ENABLE_HPTASK
	#include "bthptask.h"
#endif

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_signal.h"
#include "cdbt/bt/bt_version.h"

#define BTLOG_TAG "BTTASK"
#include "btlog_define.h"

static volatile bt_ulong     mSignals;
static xSemaphoreHandle      mSignalsSemaphore;
static bttask_SignalHandler  mAudioSignalHandler;
static bttask_SignalHandler  mRecorderSignalHandler;
static bttask_SignalHandler  mDisplaySignalHandler;
static bttask_SignalHandler  mScoPortSignalHandler;
static bttask_SignalHandler  mUsbSignalHandler;
static bttask_SignalHandler  mVolumeSignalHandler;
static bttask_SignalHandler  mAppCmdSignalHandler;

static void bluetoothTaskProc(void* params);
static void onStorageStarted(void);
static void onBluetoothPortStarted(void);
#ifdef BTAPP_USE_IAP
static void onACPStarted(void);
#endif
#ifdef BTAPP_USE_PBAP	
	static void onPhonebookMgrStarted(void);
#endif

void bttask_init(void)
{
  mSignals = 0;
  mAudioSignalHandler = NULL;
  mRecorderSignalHandler = NULL;
  mScoPortSignalHandler = NULL;
  mVolumeSignalHandler = NULL;

  btlog_init();
  btmgr_init();
  btdisplay_init();
  btapp_init();
  btapp_initMenu();
#ifdef BTAPP_ENABLE_STATS
  btstats_init();
#endif

#ifdef BTAPP_USE_PBAP	
  btpbmgr_init();
#endif

  bttask_pal_initTimer();
  bttask_pal_initButtons();
  bttask_pal_initKeyboard();
  bttask_pal_initBluetoothPort();
  bttask_pal_initStorage();
#ifdef BTAPP_USE_IAP
  bttask_pal_initACP();
#endif

#ifdef BTAPP_USE_AUDIO
  bttask_pal_initVolumeControl();
#endif

  vSemaphoreCreateBinary(mSignalsSemaphore);

  xTaskCreate(
                  &bluetoothTaskProc,       // task proc
                  "BT",   // task name
                  configBTTASK_STACK_SIZE,  // stack depth
                  NULL,                     // task parameters
                  configBTTASK_PRIORITY,    // priority
                  NULL);                    // task handle (not used)
}


void bttask_setSignal(bt_ulong signal)
{
  taskENTER_CRITICAL();
  mSignals |= signal;
  taskEXIT_CRITICAL();

  xSemaphoreGive(mSignalsSemaphore);
}


void bttask_setSignalFromISR(bt_ulong signal, signed portBASE_TYPE* higherPriorityTaskWoken)
{
  unsigned portBASE_TYPE savedInterruptStatus;

  savedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
  mSignals |= signal;
  portCLEAR_INTERRUPT_MASK_FROM_ISR(savedInterruptStatus);

  xSemaphoreGiveFromISR(mSignalsSemaphore, higherPriorityTaskWoken);
}


void bttask_setSignalFromTickHook(bt_ulong signal)
{
  signed portBASE_TYPE higherPriorityTaskWoken;
        
  mSignals |= signal;

  xSemaphoreGiveFromISR(mSignalsSemaphore, &higherPriorityTaskWoken);

#if defined (configFREERTOS_VERSION) && configFREERTOS_VERSION >= 70500 && configFREERTOS_VERSION < 70503
  if (higherPriorityTaskWoken)
          taskYIELD();
#endif
}


void bttask_setAudioSignalHandler(bttask_SignalHandler handler)
{
  mAudioSignalHandler = handler;
}

void bttask_setRecorderSignalHandler(bttask_SignalHandler handler)
{
  mRecorderSignalHandler = handler;
}

void bttask_setScoPortSignalHandler(bttask_SignalHandler handler)
{
  mScoPortSignalHandler = handler;
}

void bttask_setUsbSignalHandler(bttask_SignalHandler handler)
{
  mUsbSignalHandler = handler;
}

void bttask_setDisplaySignalHandler(bttask_SignalHandler handler)
{
  mDisplaySignalHandler = handler;
}

void bttask_setVolumeSignalHandler(bttask_SignalHandler handler)
{
  mVolumeSignalHandler = handler;
}

void bttask_setAppCmdSignalHandler(bttask_SignalHandler handler)
{
  mAppCmdSignalHandler = handler;
}

static void bluetoothTaskProc(void* params)
{
  bt_ulong signals;

  btlog_start();
#ifdef BTAPP_ENABLE_STATS
  btstats_start();
#endif
  bttask_pal_startStorage(&onStorageStarted);

  BTLOG("");
  BTLOG(" ===============================================");
  BTLOG1(" dotstack %s", bt_sys_get_version());
  BTLOG(" ===============================================");
  BTLOG("");

  for (;;)
  {
    xSemaphoreTake(mSignalsSemaphore, portMAX_DELAY);
  
    taskENTER_CRITICAL();
    signals = mSignals;
    mSignals = 0;
    taskEXIT_CRITICAL();
  
    if (signals & BTTASK_SIG_WAKEUP)
    {
      bttask_pal_handleWakeupSignal();
    }
  
    if (signals & BTTASK_SIG_BTSIGNAL)
    {
      bt_signal_process_pending();
    }
  
    if (signals & BTTASK_SIG_AUDIO)
    {
      mAudioSignalHandler();
    }

    if (signals & BTTASK_SIG_RECORDER)
    {
      mRecorderSignalHandler();
    }

    if (signals & BTTASK_SIG_SCOPORT)
    {
      mScoPortSignalHandler();
    }
  
    if (signals & BTTASK_SIG_RX)
    {
      bttask_pal_handleRxSignal();
    }
  
    if (signals & BTTASK_SIG_TX)
    {
      bttask_pal_handleTxSignal();
    }

    if (signals & BTTASK_SIG_TIMER)
    {
      bttask_pal_handleTimerSignal();
    }
  
    if (signals & BTTASK_SIG_BUTTONS)
    {
      bttask_pal_handleButtonsSignal();
    }
  
    if (signals & BTTASK_SIG_BUTTON_REPEAT)
    {
      bttask_pal_handleButtonRepeatSignal();
    }
  
    if (signals & BTTASK_SIG_KEYBOARD)
    {
      bttask_pal_handleKeyboardSignal();
    }
  
    if (signals & BTTASK_SIG_USB)
    {
      mUsbSignalHandler();
    }

    if (signals & BTTASK_SIG_STORAGE)
    {
      bttask_pal_handleStorageSignal();
    }

    if (signals & BTTASK_SIG_DISPLAY)
    {
      mDisplaySignalHandler();
    }

    if (signals & BTTASK_SIG_VOLUME)
    {
      mVolumeSignalHandler();
    }

#ifdef BTAPP_USE_IAP
    if (signals & BTTASK_SIG_ACP)
    {
      bttask_pal_handleACPSignal();
    }
#endif

    if (signals & BTTASK_SIG_CMD)
    {
      mAppCmdSignalHandler();
    }
  }
}

static void onStorageStarted(void)
{
  // Start modules that depend on working persistent storage.

  btmgr_start();

#ifdef BTAPP_USE_PBAP	
  btpbmgr_start(&onPhonebookMgrStarted);
#else
  bttask_pal_startBluetoothPort(&onBluetoothPortStarted);
#endif
}

#ifdef BTAPP_USE_IAP
static void onBluetoothPortStarted(void)
{
  bttask_pal_startACP(onACPStarted);
}

static void onACPStarted(void)
{
#ifdef BTAPP_ENABLE_HPTASK
  bthptask_init();
#endif

  btapp_start();
}
#else
static void onBluetoothPortStarted(void)
{
#ifdef BTAPP_ENABLE_HPTASK
  bthptask_init();
#endif
	
  btapp_start();
}
#endif

#ifdef BTAPP_USE_PBAP	
static void onPhonebookMgrStarted(void)
{
  bttask_pal_startBluetoothPort(&onBluetoothPortStarted);
}
#endif

//
// Bluetooth signals
// -------------------------------------------------------------------
//
void bt_oem_schedule_signals(void)
{
  bttask_setSignal(BTTASK_SIG_BTSIGNAL);
}


