/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTDISPLAY_H_INCLUDED__
#define __BTDISPLAY_H_INCLUDED__

#include "btapp.h"
#include "cdbt/bt/bt_std.h"

#ifndef BTDISPLAY_MAX_WINDOWS
	#define BTDISPLAY_MAX_WINDOWS             1
#endif

#define BTDISPLAY_FLAG_APPLICATION            0x00000001
#define BTDISPLAY_FLAG_CONNECTED              0x00000002
#define BTDISPLAY_FLAG_CONNECTING             0x00000004
#define BTDISPLAY_FLAG_PAIRED                 0x00000008
#define BTDISPLAY_FLAG_KEY_MODIFIERS          0x00000010
#define BTDISPLAY_FLAG_PIN                    0x00000020
#define BTDISPLAY_FLAG_SEARCHING              0x00000040
#define BTDISPLAY_FLAG_TRACK_NAME             0x00000080
#define BTDISPLAY_FLAG_PLAY_STATUS            0x00000100
#define BTDISPLAY_FLAG_TRACK_NUMBER           0x00000200
#define BTDISPLAY_FLAG_MENU                   0x00000400
#define BTDISPLAY_FLAG_TRACK_POS              0x00000800
#define BTDISPLAY_FLAG_ALBUM_NAME             0x00001000
#define BTDISPLAY_FLAG_HID_REPORT             0x00002000
#define BTDISPLAY_FLAG_AUDIO_CODEC            0x00004000
#define BTDISPLAY_FLAG_SERVICE_STATE          0x00008000
#define BTDISPLAY_FLAG_ROAMING_STATE          0x00010000
#define BTDISPLAY_FLAG_SIGNAL_STRENGTH        0x00020000
#define BTDISPLAY_FLAG_BATTERY_LEVEL          0x00040000
#define BTDISPLAY_FLAG_SPEAKER_VOLUME         0x00080000
#define BTDISPLAY_FLAG_MICROPHONE_GAIN        0x00100000
#define BTDISPLAY_FLAG_CALL_NUMBER            0x00200000
#define BTDISPLAY_FLAG_IN_BAND_RING           0x00400000
#define BTDISPLAY_FLAG_DEVICE_NAME            0x00800000
#define BTDISPLAY_FLAG_MANUFACTURER_NAME      0x01000000
#define BTDISPLAY_FLAG_SENSOR_LOCATION        0x02000000
#define BTDISPLAY_FLAG_HEART_RATE             0x04000000
#define BTDISPLAY_FLAG_UPDATING               0x08000000
#define BTDISPLAY_FLAG_OPERATOR_NAME          0x10000000
#define BTDISPLAY_FLAG_PAIRING                0x20000000
#define BTDISPLAY_FLAG_REPEAT_MODE            0x40000000
#define BTDISPLAY_FLAG_SHUFFLE_MODE           0x80000000

#define BTDISPLAY_FLAG2_CHARACTERISTIC_VALUE  0x00000001
#define BTDISPLAY_FLAG2_PERIPH_BATTERY_LEVEL  0x00000002

#define BTDISPLAY_PLAY_STATUS_STOPPED         0
#define BTDISPLAY_PLAY_STATUS_PLAYING         1
#define BTDISPLAY_PLAY_STATUS_PAUSED          2
#define BTDISPLAY_PLAY_STATUS_FW_SEEK         3
#define BTDISPLAY_PLAY_STATUS_REV_SEEK        4
#define BTDISPLAY_PLAY_STATUS_ERROR           0xFF

#define BTDISPLAY_CUSTOM_HID_REPORT_LEN       16

#define BTDISPLAY_CODEC_TYPE_SBC              0x00
#define BTDISPLAY_CODEC_TYPE_MPEG1_2_AUDIO    0x01
#define BTDISPLAY_CODEC_TYPE_MPEG2_4_AAC      0x02
#define BTDISPLAY_CODEC_TYPE_ATRAC            0x04
#define BTDISPLAY_CODEC_TYPE_NON_A2DP         0xFF

typedef struct _btdisplay_Data
{
	char application;
	char connected;
	char connecting;
	char searching;
	char paired;
	char pairing;
	char updating;
	char keyModifiers;
	const char* pin;
	const char* albumName;
	const char* trackName;
	const char* trackNumber;
	char playStatus;
	unsigned long trackPosition;
	unsigned long trackLength;
	btapp_Menu menu;
	unsigned char menuWidth;
	bt_ulong modified;
	bt_ulong displayed;
	bt_ulong modified2;
	bt_ulong displayed2;
	bt_bool redraw;
	const char* customHIDReport;
	unsigned char codec;
	const char* operatorName;
	unsigned char serviceState;
	unsigned char roamingState;
	unsigned char signalStrength;
	unsigned char batteryLevel;
	unsigned char speakerVolume;
	unsigned char microphoneGain;
	unsigned char inBandRing;
	const char* callNumber;
	unsigned char callStatus;
	const char* deviceName;
	const char* manufacturerName;
	unsigned char heartRate;
	unsigned char sensorLocation;
	unsigned char repeatMode;
	unsigned char shuffleMode;
	const bt_byte* charValue;
	bt_uint charValueLen;
	unsigned char periphBatteryLevel;
} btdisplay_Data;

#define btdisplay_setApplication(application)      btdisplay_setApplicationEx(application, 0)
#define btdisplay_setConnectedState(connected)     btdisplay_setConnectedStateEx(connected, 0)
#define btdisplay_setConnectingState(connecting)   btdisplay_setConnectingStateEx(connecting, 0)
#define btdisplay_setSearchingState(searching)     btdisplay_setSearchingStateEx(searching, 0)
#define btdisplay_setPairedState(paired)           btdisplay_setPairedStateEx(paired, 0)
#define btdisplay_setPairingState(pairing)         btdisplay_setPairingStateEx(pairing, 0)
#define btdisplay_setUpdatingState(updating)       btdisplay_setUpdatingStateEx(updating, 0)
#define btdisplay_setKeyModifiers(keyModifiers)    btdisplay_setKeyModifiersEx(keyModifiers, 0)
#define btdisplay_setPIN(pin)                      btdisplay_setPINEx(pin, 0)
#define btdisplay_setAlbumName(name)               btdisplay_setAlbumNameEx(name, 0)
#define btdisplay_setTrackName(name)               btdisplay_setTrackNameEx(name, 0)
#define btdisplay_setTrackNumber(number)           btdisplay_setTrackNumberEx(number, 0)
#define btdisplay_setTrackPosition(pos, len)       btdisplay_setTrackPositionEx(pos, len, 0)
#define btdisplay_setPlayStatus(status)            btdisplay_setPlayStatusEx(status, 0)
#define btdisplay_setMenu(menu)                    btdisplay_setMenuEx(menu, 0)
#define btdisplay_setMenuWidth(width)              btdisplay_setMenuWidthEx(width, 0)
#define btdisplay_setCustomHIDReport(report)       btdisplay_setCustomHIDReportEx(report, 0)
#define btdisplay_setCodec(codec)                  btdisplay_setCodecEx(codec, 0)
#define btdisplay_setOperatorName(name)            btdisplay_setOperatorNameEx(name, 0)
#define btdisplay_setServiceState(serviceState)    btdisplay_setServiceStateEx(serviceState, 0)
#define btdisplay_setRoamingState(serviceState)    btdisplay_setRoamingStateEx(serviceState, 0)
#define btdisplay_setBatteryLevel(level)           btdisplay_setBatteryLevelEx(level, 0)
#define btdisplay_setSpeakerVolume(volume)         btdisplay_setSpeakerVolumeEx(volume, 0)
#define btdisplay_setMicrophoneGain(gain)          btdisplay_setMicrophoneGainEx(gain, 0)
#define btdisplay_setSignalStrength(signal)        btdisplay_setSignalStrengthEx(signal, 0)
#define btdisplay_setCall(number, status)          btdisplay_setCallEx(number, status, 0)
#define btdisplay_setInBandRing(inband)            btdisplay_setInBandRingEx(inband, 0)
#define btdisplay_setDeviceName(name)              btdisplay_setDeviceNameEx(name, 0)
#define btdisplay_setManufacturerName(name)        btdisplay_setManufacturerNameEx(name, 0)
#define btdisplay_setHeartRate(rate)               btdisplay_setHeartRateEx(rate, 0)
#define btdisplay_setSensorLocation(location)      btdisplay_setSensorLocationEx(location, 0)
#define btdisplay_setRepeatMode(mode)              btdisplay_setRepeatModeEx(mode, 0)
#define btdisplay_setShuffleMode(mode)             btdisplay_setShuffleModeEx(mode, 0)
#define btdisplay_setCharValue(value, len)         btdisplay_setCharValueEx(value, len, 0)
#define btdisplay_setPeriphBatteryLevel(level)     btdisplay_setPeriphBatteryLevelEx(level, 0)

void btdisplay_init(void);
void btdisplay_showWindow(int window);
int  btdisplay_getVisibleWindow(void);
void btdisplay_setApplicationEx(btapp_Application application, int window);
void btdisplay_setConnectedStateEx(bt_bool connected, int window);
void btdisplay_setConnectingStateEx(bt_bool connecting, int window);
void btdisplay_setSearchingStateEx(bt_bool searching, int window);
void btdisplay_setPairedStateEx(bt_bool paired, int window);
void btdisplay_setPairingStateEx(bt_bool pairing, int window);
void btdisplay_setUpdatingStateEx(bt_bool updating, int window);
void btdisplay_setKeyModifiersEx(bt_byte keyModifiers, int window);
void btdisplay_setPINEx(const char* pin, int window);
void btdisplay_setAlbumNameEx(const char* name, int window);
void btdisplay_setTrackNameEx(const char* name, int window);
void btdisplay_setTrackNumberEx(const char* number, int window);
void btdisplay_setTrackPositionEx(unsigned long pos, unsigned long len, int window);
void btdisplay_setPlayStatusEx(char status, int window);
void btdisplay_setMenuEx(btapp_Menu* menu, int window);
void btdisplay_setMenuWidthEx(unsigned char width, int window);
void btdisplay_setCustomHIDReportEx(const char* report, int window);
void btdisplay_setCodecEx(unsigned char codec, int window);
void btdisplay_setOperatorNameEx(const char* name, int window);
void btdisplay_setServiceStateEx(bt_bool serviceState, int window);
void btdisplay_setRoamingStateEx(bt_bool serviceState, int window);
void btdisplay_setBatteryLevelEx(bt_byte level, int window);
void btdisplay_setSpeakerVolumeEx(bt_byte volume, int window);
void btdisplay_setMicrophoneGainEx(bt_byte gain, int window);
void btdisplay_setSignalStrengthEx(bt_byte signal, int window);
void btdisplay_setCallEx(const char* number, bt_byte status, int window);
void btdisplay_setInBandRingEx(bt_bool inband, int window);
void btdisplay_setDeviceNameEx(const char* name, int window);
void btdisplay_setManufacturerNameEx(const char* name, int window);
void btdisplay_setHeartRateEx(unsigned char rate, int window);
void btdisplay_setSensorLocationEx(unsigned char location, int window);
void btdisplay_setRepeatModeEx(unsigned char mode, int window);
void btdisplay_setShuffleModeEx(unsigned char mode, int window);
void btdisplay_setCharValueEx(const bt_byte* value, bt_uint len, int window);
void btdisplay_setPeriphBatteryLevelEx(bt_byte level, int window);
void btdisplay_update(void);

void btdisplay_clear(void);
void btdisplay_onUpdate(const btdisplay_Data* data);

#endif // __BTDISPLAY_H_INCLUDED__
