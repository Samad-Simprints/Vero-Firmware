/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include <assert.h>
#include <string.h>

#include "bttimer.h"
#include "bttask.h"
#include "btapp.h"
#include "btdisplay.h"
#ifdef BTAPP_USE_AUDIO
	#include "btaudio.h"
#endif

#define BTLOG_TAG "BTAPP"
#include "btlog_define.h"

#define MAX_MENU_DEPTH    5

typedef struct _btapp_MenuData
{
	btapp_Menu*    currentMenu;
	btapp_Menu*    menuStack[MAX_MENU_DEPTH];
	bt_int         menuDepth;
} btapp_MenuData;

static btapp_MenuData mMenuData[BTDISPLAY_MAX_WINDOWS];

#ifdef BTAPP_USE_AUDIO
static unsigned short mVolume;
#endif

static bt_bool defaultMenuHandler(bt_byte action, bt_byte selected);
static btapp_MenuData* getMenuData(int window);

static void onBluetoothPortReStarted(void);

void btapp_restart(void)
{
	bttask_pal_restartBluetoothPort(&onBluetoothPortReStarted);
}

static void onBluetoothPortReStarted(void)
{
	btapp_start();
}


void btapp_initMenu(void)
{
	int i;

	memset(mMenuData, 0, sizeof(mMenuData));
	for (i = 0; i < BTDISPLAY_MAX_WINDOWS; i++)
		mMenuData[i].menuDepth = -1;
}


void btapp_hideMenuEx(int window)
{
	btapp_MenuData* data = getMenuData(window);

	if (data)
	{
		data->currentMenu = NULL;
		data->menuDepth = -1;

		btdisplay_setMenuEx(data->currentMenu, window);
		btdisplay_update();
	}
}

void btapp_pushMenuEx(btapp_Menu* menu, bt_bool keepSelected, int window)
{
	btapp_MenuData* data = getMenuData(window);

	if (!menu || !data)
		return;


	if (data->currentMenu != menu && data->menuDepth + 1 < MAX_MENU_DEPTH)
	{
		data->currentMenu = menu;
		menu->current = 0;
		data->menuStack[++data->menuDepth] = menu;
	}
	
	btdisplay_setMenuEx(data->currentMenu, window);
	btdisplay_update();
}

void btapp_popMenuEx(btapp_Menu* menu, int window)
{
	btapp_MenuData* data = getMenuData(window);

	if (!menu || !data)
		return;
	
	if (data->currentMenu == menu && data->menuDepth - 1 >= 0)
	{
		data->currentMenu = data->menuStack[--data->menuDepth];
		btdisplay_setMenuEx(data->currentMenu, window);
		btdisplay_update();
	}
}

void btapp_updateMenuEx(int window)
{
	btapp_MenuData* data = getMenuData(window);

	if (data)
	{
		btdisplay_setMenuEx(data->currentMenu, window);
		btdisplay_update();
	}
}

void btapp_popToMenuEx(btapp_Menu* menu, int window)
{
	btapp_MenuData* data = getMenuData(window);

	if (data)
	{
		bt_int i;
	
		for (i = data->menuDepth; i >= 0; i--)
		{
			if (data->menuStack[i] == menu)
			{
				data->menuDepth = i;
				break;
			}
		}
	}
}

void btapp_enableMenuItemEx(btapp_Menu* menu, bt_int index, bt_bool enable, int window)
{
	if (!menu)
		return;

	if (index >= 0 && index < menu->count)
	{
		btapp_MenuData* data = getMenuData(window);

		menu->choices[index].enabled = enable;
		if (data && data->currentMenu == menu)
		{
			btdisplay_setMenuEx(data->currentMenu, window);
			btdisplay_update();
		}
	}
}

bt_bool btapp_callMenuHandler(bt_byte action)
{
	bt_bool result = BT_FALSE;
	btapp_MenuData* data = getMenuData(btdisplay_getVisibleWindow());
	
	if (data && data->currentMenu)
	{	
		if (data->currentMenu->handler)
		{
			result = data->currentMenu->handler(action, data->currentMenu->current);
			if (!result)
				result = defaultMenuHandler(action, data->currentMenu->current);
		}
	}
	
	return result;
}

static bt_bool defaultMenuHandler(bt_byte action, bt_byte selected)
{
	bt_bool result = BT_TRUE;
	int visibleWindow = btdisplay_getVisibleWindow();
	btapp_MenuData* data = getMenuData(visibleWindow);

	if (!data)
		return result;
	
	switch (action)
	{
		case BTAPP_MENU_ACTION_CANCEL:
			btapp_popMenuEx(data->currentMenu, visibleWindow);
			break;
			
		case BTAPP_MENU_ACTION_UP:
			if (!data->currentMenu->current)
				break;
	
			--data->currentMenu->current;
			btdisplay_setMenuEx(data->currentMenu, visibleWindow);
			btdisplay_update();
			break;

		case BTAPP_MENU_ACTION_DOWN:
			if (data->currentMenu->current == data->currentMenu->count - 1)
				break;
			
			++data->currentMenu->current;
			btdisplay_setMenuEx(data->currentMenu, visibleWindow);
			btdisplay_update();
			break;
			
		default:
			result = BT_FALSE;
			break;
	}
	
	return result;
}

static btapp_MenuData* getMenuData(int window)
{
	if (!(window < 0 || window >= BTDISPLAY_MAX_WINDOWS))
		return &mMenuData[window];
	else
		return NULL;
}

#ifdef BTAPP_USE_AUDIO

unsigned short btapp_getVolume(void)
{
	return mVolume;
}

void btapp_setVolume(unsigned short volume)
{
	BTLOG1("btapp_setVolume: volume = %d", volume);
	mVolume = volume;
	btaudio_setVolume(volume, BT_FALSE);
}

#endif
