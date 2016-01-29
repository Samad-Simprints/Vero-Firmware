/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "btdisplay.h"
#include <string.h>


static btdisplay_Data mData[BTDISPLAY_MAX_WINDOWS];
static int            mVisibleWindow;

static char isWindowValid(int window);

void btdisplay_init(void)
{
	memset(&mData, 0, sizeof(mData));
	mVisibleWindow = 0;
}

void btdisplay_showWindow(int window)
{
	if (window != mVisibleWindow && isWindowValid(window))
	{
		btdisplay_Data* data = &mData[window];

		mVisibleWindow = window;
		data->modified |= data->displayed;
		data->modified2 |= data->displayed2;
		data->redraw = BT_TRUE;
		btdisplay_clear();
		btdisplay_update();
	}
}

int  btdisplay_getVisibleWindow(void)
{
	return mVisibleWindow;
}

void btdisplay_setApplicationEx(btapp_Application application, int window)
{
	if (isWindowValid(window))
	{
		mData[window].application = (char) application;
		mData[window].modified |= BTDISPLAY_FLAG_APPLICATION;
		mData[window].displayed |= BTDISPLAY_FLAG_APPLICATION;
	}
}


void btdisplay_setConnectedStateEx(bt_bool connected, int window)
{
	if (isWindowValid(window))
	{
		mData[window].connected = (char) connected;
		mData[window].modified |= BTDISPLAY_FLAG_CONNECTED;
		mData[window].displayed |= BTDISPLAY_FLAG_CONNECTED;
	}
}


void btdisplay_setConnectingStateEx(bt_bool connecting, int window)
{
	if (isWindowValid(window))
	{
		mData[window].connecting = (char) connecting;
		mData[window].modified |= BTDISPLAY_FLAG_CONNECTING;
		mData[window].displayed |= BTDISPLAY_FLAG_CONNECTING;
	}
}

void btdisplay_setSearchingStateEx(bt_bool searching, int window)
{
	if (isWindowValid(window))
	{
		mData[window].searching = (char) searching;
		mData[window].modified |= BTDISPLAY_FLAG_SEARCHING;
		mData[window].displayed |= BTDISPLAY_FLAG_SEARCHING;
	}
}

void btdisplay_setPairedStateEx(bt_bool paired, int window)
{
	if (isWindowValid(window))
	{
		mData[window].paired = (char) paired;
		mData[window].modified |= BTDISPLAY_FLAG_PAIRED;
		mData[window].displayed |= BTDISPLAY_FLAG_PAIRED;
	}
}

void btdisplay_setPairingStateEx(bt_bool pairing, int window)
{
	if (isWindowValid(window))
	{
		mData[window].pairing = (char) pairing;
		mData[window].modified |= BTDISPLAY_FLAG_PAIRING;
		mData[window].displayed |= BTDISPLAY_FLAG_PAIRING;
	}
}

void btdisplay_setUpdatingStateEx(bt_bool updating, int window)
{
	if (isWindowValid(window))
	{
		mData[window].updating = (char) updating;
		mData[window].modified |= BTDISPLAY_FLAG_UPDATING;
		mData[window].displayed |= BTDISPLAY_FLAG_UPDATING;
	}
}

void btdisplay_setKeyModifiersEx(bt_byte keyModifiers, int window)
{
	if (isWindowValid(window))
	{
		mData[window].keyModifiers = keyModifiers;
		mData[window].modified |= BTDISPLAY_FLAG_KEY_MODIFIERS;
		mData[window].displayed |= BTDISPLAY_FLAG_KEY_MODIFIERS;
	}
}


void btdisplay_setPINEx(const char* pin, int window)
{
	if (isWindowValid(window))
	{
		mData[window].pin = pin;
		mData[window].modified |= BTDISPLAY_FLAG_PIN;
		mData[window].displayed |= BTDISPLAY_FLAG_PIN;
	}
}

void btdisplay_setAlbumNameEx(const char* name, int window)
{
	if (isWindowValid(window))
	{
		mData[window].albumName = name;
		mData[window].modified |= BTDISPLAY_FLAG_ALBUM_NAME;
		mData[window].displayed |= BTDISPLAY_FLAG_ALBUM_NAME;
	}
}

void btdisplay_setTrackNameEx(const char* name, int window)
{
	if (isWindowValid(window))
	{
		mData[window].trackName = name;
		mData[window].modified |= BTDISPLAY_FLAG_TRACK_NAME;
		mData[window].displayed |= BTDISPLAY_FLAG_TRACK_NAME;
	}
}

void btdisplay_setTrackNumberEx(const char* number, int window)
{
	if (isWindowValid(window))
	{
		mData[window].trackNumber = number;
		mData[window].modified |= BTDISPLAY_FLAG_TRACK_NUMBER;
		mData[window].displayed |= BTDISPLAY_FLAG_TRACK_NUMBER;
	}
}

void btdisplay_setPlayStatusEx(char status, int window)
{
	if (isWindowValid(window))
	{
		if (mData[window].playStatus != status)
		{
			mData[window].playStatus = status;
			mData[window].modified |= BTDISPLAY_FLAG_PLAY_STATUS;
			mData[window].displayed |= BTDISPLAY_FLAG_PLAY_STATUS;
		}
	}
}

void btdisplay_setTrackPositionEx(unsigned long pos, unsigned long len, int window)
{
	if (isWindowValid(window))
	{
		if (mData[window].trackPosition != pos || mData[window].trackLength != len)
		{
			mData[window].trackPosition = pos;
			mData[window].trackLength = len;
			mData[window].modified |= BTDISPLAY_FLAG_TRACK_POS;
			mData[window].displayed |= BTDISPLAY_FLAG_TRACK_POS;
		}
	}
}

void btdisplay_setMenuEx(btapp_Menu* menu, int window)
{
	if (isWindowValid(window))
	{
		if (menu)
			mData[window].menu = *menu;
		else
			mData[window].menu.choices = NULL;
		mData[window].modified |= BTDISPLAY_FLAG_MENU;
		mData[window].displayed |= BTDISPLAY_FLAG_MENU;
	}
}

void btdisplay_setMenuWidthEx(unsigned char width, int window)
{
	if (isWindowValid(window))
	{
		if (mData[window].menuWidth != width)
		{
			mData[window].redraw = BT_TRUE;
			mData[window].modified |= mData[window].displayed;
			btdisplay_clear();
		}
		
		mData[window].menuWidth = width;
		mData[window].modified |= BTDISPLAY_FLAG_MENU;
		mData[window].displayed |= BTDISPLAY_FLAG_MENU;
	}
}

void btdisplay_setCustomHIDReportEx(const char* report, int window)
{
	if (isWindowValid(window))
	{
		mData[window].customHIDReport = report;
		mData[window].modified |= BTDISPLAY_FLAG_HID_REPORT;
		mData[window].displayed |= BTDISPLAY_FLAG_HID_REPORT;
	}
}

void btdisplay_setCodecEx(unsigned char codec, int window)
{
	if (isWindowValid(window))
	{
		mData[window].codec = codec;
		mData[window].modified |= BTDISPLAY_FLAG_AUDIO_CODEC;
		mData[window].displayed |= BTDISPLAY_FLAG_AUDIO_CODEC;
	}
}

void btdisplay_setOperatorNameEx(const char* name, int window)
{
	if (isWindowValid(window))
	{
		mData[window].operatorName = name;
		mData[window].modified |= BTDISPLAY_FLAG_OPERATOR_NAME;
		mData[window].displayed |= BTDISPLAY_FLAG_OPERATOR_NAME;
	}
}

void btdisplay_setServiceStateEx(bt_bool serviceState, int window)
{
	if (isWindowValid(window))
	{
		mData[window].serviceState = serviceState;
		mData[window].modified |= BTDISPLAY_FLAG_SERVICE_STATE;
		mData[window].displayed |= BTDISPLAY_FLAG_SERVICE_STATE;
	}
}

void btdisplay_setRoamingStateEx(bt_bool roamingState, int window)
{
	if (isWindowValid(window))
	{
		mData[window].roamingState = roamingState;
		mData[window].modified |= BTDISPLAY_FLAG_ROAMING_STATE;
		mData[window].displayed |= BTDISPLAY_FLAG_ROAMING_STATE;
	}
}	

void btdisplay_setBatteryLevelEx(bt_byte level, int window)
{
	if (isWindowValid(window))
	{
		mData[window].batteryLevel = level;
		mData[window].modified |= BTDISPLAY_FLAG_BATTERY_LEVEL;
		mData[window].displayed |= BTDISPLAY_FLAG_BATTERY_LEVEL;
	}
}

void btdisplay_setSpeakerVolumeEx(bt_byte volume, int window)
{
	if (isWindowValid(window))
	{
		mData[window].speakerVolume = volume;
		mData[window].modified |= BTDISPLAY_FLAG_SPEAKER_VOLUME;
		mData[window].displayed |= BTDISPLAY_FLAG_SPEAKER_VOLUME;
	}
}

void btdisplay_setMicrophoneGainEx(bt_byte gain, int window)
{
	if (isWindowValid(window))
	{
		mData[window].microphoneGain = gain;
		mData[window].modified |= BTDISPLAY_FLAG_MICROPHONE_GAIN;
		mData[window].displayed |= BTDISPLAY_FLAG_MICROPHONE_GAIN;
	}
}

void btdisplay_setSignalStrengthEx(bt_byte strength, int window)
{
	if (isWindowValid(window))
	{
		mData[window].signalStrength = strength;
		mData[window].modified |= BTDISPLAY_FLAG_SIGNAL_STRENGTH;
		mData[window].displayed |= BTDISPLAY_FLAG_SIGNAL_STRENGTH;
	}
}

void btdisplay_setCallEx(const char* number, bt_byte status, int window)
{
	if (isWindowValid(window))
	{
		mData[window].callNumber = number;
		mData[window].callStatus = status;
		mData[window].modified |= BTDISPLAY_FLAG_CALL_NUMBER;
		mData[window].displayed |= BTDISPLAY_FLAG_CALL_NUMBER;
	}
}

void btdisplay_setInBandRingEx(bt_bool inband, int window)
{
	if (isWindowValid(window))
	{
		mData[window].inBandRing = inband;
		mData[window].modified |= BTDISPLAY_FLAG_IN_BAND_RING;
		mData[window].displayed |= BTDISPLAY_FLAG_IN_BAND_RING;
	}
}

void btdisplay_setDeviceNameEx(const char* name, int window)
{
	if (isWindowValid(window))
	{
		mData[window].deviceName = name;
		mData[window].modified |= BTDISPLAY_FLAG_DEVICE_NAME;
		mData[window].displayed |= BTDISPLAY_FLAG_DEVICE_NAME;
	}
}

void btdisplay_setManufacturerNameEx(const char* name, int window)
{
	if (isWindowValid(window))
	{
		mData[window].manufacturerName = name;
		mData[window].modified |= BTDISPLAY_FLAG_MANUFACTURER_NAME;
		mData[window].displayed |= BTDISPLAY_FLAG_MANUFACTURER_NAME;
	}
}

void btdisplay_setHeartRateEx(unsigned char rate, int window)
{
	if (isWindowValid(window))
	{
		mData[window].heartRate = rate;
		mData[window].modified |= BTDISPLAY_FLAG_HEART_RATE;
		mData[window].displayed |= BTDISPLAY_FLAG_HEART_RATE;
	}
}

void btdisplay_setSensorLocationEx(unsigned char location, int window)
{
	if (isWindowValid(window))
	{
		mData[window].sensorLocation = location;
		mData[window].modified |= BTDISPLAY_FLAG_SENSOR_LOCATION;
		mData[window].displayed |= BTDISPLAY_FLAG_SENSOR_LOCATION;
	}
}

void btdisplay_setRepeatModeEx(unsigned char mode, int window)
{
	if (isWindowValid(window))
	{
		mData[window].repeatMode = mode;
		mData[window].modified |= BTDISPLAY_FLAG_REPEAT_MODE;
		mData[window].displayed |= BTDISPLAY_FLAG_REPEAT_MODE;
	}
}

void btdisplay_setShuffleModeEx(unsigned char mode, int window)
{
	if (isWindowValid(window))
	{
		mData[window].shuffleMode = mode;
		mData[window].modified |= BTDISPLAY_FLAG_SHUFFLE_MODE;
		mData[window].displayed |= BTDISPLAY_FLAG_SHUFFLE_MODE;
	}
}

void btdisplay_setCharValueEx(const bt_byte* value, bt_uint len, int window)
{
	if (isWindowValid(window))
	{
		mData[window].charValue = value;
		mData[window].charValueLen = len;
		mData[window].modified2 |= BTDISPLAY_FLAG2_CHARACTERISTIC_VALUE;
		mData[window].displayed2 |= BTDISPLAY_FLAG2_CHARACTERISTIC_VALUE;
	}
}

void btdisplay_setPeriphBatteryLevelEx(bt_byte level, int window)
{
	if (isWindowValid(window))
	{
		mData[window].periphBatteryLevel = level;
		mData[window].modified2 |= BTDISPLAY_FLAG2_PERIPH_BATTERY_LEVEL;
		mData[window].displayed2 |= BTDISPLAY_FLAG2_PERIPH_BATTERY_LEVEL;
	}
}

void btdisplay_update(void)
{
	if (isWindowValid(mVisibleWindow))
	{
		btdisplay_onUpdate(&mData[mVisibleWindow]);
		mData[mVisibleWindow].modified = 0;
		mData[mVisibleWindow].modified2 = 0;
		mData[mVisibleWindow].redraw = BT_FALSE;
	}
}

static char isWindowValid(int window)
{
	return !(window < 0 || window >= BTDISPLAY_MAX_WINDOWS);
}
