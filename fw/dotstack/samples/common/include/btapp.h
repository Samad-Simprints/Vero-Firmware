/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTAPP_H_INCLUDED__
#define __BTAPP_H_INCLUDED__

#include "cdbt/bt/bt_std.h"
#include "cdbt/utils/bufferutils.h"
#include "btapp_config.h"

typedef enum _btapp_Application
{
    BTAPP_APP_SPP,
    BTAPP_APP_HID_MOUSE,
    BTAPP_APP_HID_KEYBOARD,
    BTAPP_APP_HEARTRATE_SENSOR,
    BTAPP_APP_AVRCP_CONTROLLER,
    BTAPP_APP_A2DP_SINK,
    BTAPP_APP_HID_MOUSE_KEYBOARD,
    BTAPP_APP_IAP,
    BTAPP_APP_IAP_HEARTRATE_SENSOR,
    BTAPP_APP_A2DP_SPP_HEARTRATE_SENSOR,
    BTAPP_APP_SPP_HEARTRATE_SENSOR,
    BTAPP_APP_HFP,
    BTAPP_APP_SPEED_TEST,
    BTAPP_APP_A2DP_SINK_AVRCP_CTRL,
    BTAPP_APP_HID_CUSTOM,
    BTAPP_APP_HFP_AG,
    BTAPP_APP_IAP_PLAYER,
    BTAPP_APP_MULTI_IAP,
    BTAPP_APP_HEARTRATE_MONITOR,
    BTAPP_APP_PBAP_HFP,
    BTAPP_APP_OPP,
	BTAPP_APP_A2DP_AVRCP_PBAP_HFP,
    BTAPP_APP_HFP_HSP_BLE,
    BTAPP_APP_LE_SPEED_TEST,
    BTAPP_APP_L2CAP,
    BTAPP_APP_DOTSTACK_DEMO,
} btapp_Application;

typedef enum _btapp_Status
{
    BTAPP_STATUS_IDLE,
    BTAPP_STATUS_INITIALIZED,
    BTAPP_STATUS_STARTED,
} btapp_Status;

#define BTAPP_BUTTON_S1        1
#define BTAPP_BUTTON_S2        2
#define BTAPP_BUTTON_UP        3
#define BTAPP_BUTTON_DOWN      4
#define BTAPP_BUTTON_LEFT      5
#define BTAPP_BUTTON_RIGHT     6
#define BTAPP_BUTTON_SELECT    7
#define BTAPP_BUTTON_S3        8
#define BTAPP_BUTTON_S4        9
#define BTAPP_BUTTON_S5        10
#define BTAPP_BUTTON_S6        11
#define BTAPP_BUTTON_S7        12

#define BTAPP_BUTTON_DIGIT_0   10
#define BTAPP_BUTTON_DIGIT_1   11
#define BTAPP_BUTTON_DIGIT_2   12
#define BTAPP_BUTTON_DIGIT_3   13
#define BTAPP_BUTTON_DIGIT_4   14
#define BTAPP_BUTTON_DIGIT_5   15
#define BTAPP_BUTTON_DIGIT_6   16
#define BTAPP_BUTTON_DIGIT_7   17
#define BTAPP_BUTTON_DIGIT_8   18
#define BTAPP_BUTTON_DIGIT_9   19

#define BTAPP_BUTTON_STAR      20
#define BTAPP_BUTTON_POUND     21

#define BTAPP_MENU_ACTION_SELECT    0
#define BTAPP_MENU_ACTION_CANCEL    1
#define BTAPP_MENU_ACTION_UP        2
#define BTAPP_MENU_ACTION_DOWN      3

typedef void (*btapp_TimerCallback)(void);

typedef enum _btapp_TimerId
{
    BTAPP_TIMER_0,
    BTAPP_TIMER_1,
    BTAPP_TIMER_2,
    BTAPP_TIMER_3,
    BTAPP_TIMER_RECONNECT,
    BTAPP_TIMER_IDLE,
#ifdef BTAPP_ENABLE_STATS
    BTAPP_TIMER_STATS,
#endif
    BTAPP_MAX_TIMERS
} btapp_TimerId;

typedef struct _btapp_Menu btapp_Menu;
typedef bt_bool (*btapp_MenuHandler)(bt_byte action, bt_byte selected);

typedef struct _btapp_MenuItem
{
    char* caption;
    char enabled;
} btapp_MenuItem;

struct _btapp_Menu
{
    btapp_MenuItem* choices;
    char count;
    char current;
	btapp_MenuHandler handler;
};

typedef void (*btapp_CommandHandler)(bt_uint cmd, const char* params[], bt_byte paramsCount);
typedef void (*btapp_CommandCompletionHandler)(bt_uint statis, bt_uint cmd, const char* params[], bt_byte paramsCount);

typedef struct _btapp_Command
{
	bt_uint cmd;
	const char** params;
	bt_byte paramsCount;
	btapp_CommandCompletionHandler completionHandler;
} btapp_Command;


void btapp_init(void);
void btapp_start(void);
btapp_Status btapp_getStatus(void);
void btapp_onButtonDown(bt_uint button, bt_uint repeatCount);
void btapp_onButtonUp(bt_uint button, bt_uint repeatCount);
bt_bool btapp_onHidKeyboardReport(bt_byte* report);
void btapp_hidKeyboardSetLEDs(bt_byte leds);
void btapp_onVolumeChanged(unsigned short volume);
unsigned short btapp_getVolume(void);
void btapp_setVolume(unsigned short volume);
void btapp_restart(void);

#define btapp_hideMenu()                             btapp_hideMenuEx(0)
#define btapp_pushMenu(menu, keepSelected)           btapp_pushMenuEx(menu, keepSelected, 0)
#define btapp_popMenu(menu)                          btapp_popMenuEx(menu, 0)
#define btapp_popToMenu(menu)                        btapp_popToMenuEx(menu, 0)
#define btapp_updateMenu()                           btapp_updateMenuEx(0)
#define btapp_enableMenuItem(menu, index, enable)    btapp_enableMenuItemEx(menu, index, enable, 0)

void btapp_initMenu(void);
void btapp_hideMenuEx(int window);
void btapp_pushMenuEx(btapp_Menu* menu, bt_bool keepSelected, int window);
void btapp_popMenuEx(btapp_Menu* menu, int window);
void btapp_popToMenuEx(btapp_Menu* menu, int window);
void btapp_updateMenuEx(int window);
void btapp_enableMenuItemEx(btapp_Menu* menu, bt_int index, bt_bool enable, int window);
bt_bool btapp_callMenuHandler(bt_byte action);

void btapp_setCommandHandler(btapp_CommandHandler handler);
bt_bool btapp_execCommand(bt_uint cmd, const char* params[], bt_byte paramsCount, btapp_CommandCompletionHandler completionHandler);
void btapp_completeCommand(bt_uint status);

void btapp_onFatalError(void);

bt_ulong btapp_getCurrentTime(void);
void btapp_setTimer(btapp_TimerId timerId, bt_ulong milliseconds, btapp_TimerCallback callback);
void btapp_clearTimer(btapp_TimerId timerId);


#endif // __BTAPP_H_INCLUDED__
