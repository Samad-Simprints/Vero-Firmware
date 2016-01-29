/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTAUDIO_H_INCLUDED__
#define __BTAUDIO_H_INCLUDED__

#include "cdbt/bt/bt_std.h"

#define BTAUDIO_EVENT_PLAYBACK_STARTED         1
#define BTAUDIO_EVENT_PLAYBACK_STOPPED         2
#define BTAUDIO_EVENT_PACKET_PLAYED            3
#define BTAUDIO_EVENT_SOUND_STARTED            4
#define BTAUDIO_EVENT_SOUND_STOPPED            5
#define BTAUDIO_EVENT_PACKET_ANALIZED          6
#define BTAUDIO_EVENT_PLAYBACK_PAUSED          7

#define BTAUDIO_EVENT_RECORDING_STARTED        4
#define BTAUDIO_EVENT_RECORDING_STOPPED        5
#define BTAUDIO_EVENT_PACKET_RECORDED          6
#define BTAUDIO_EVENT_RECORDING_PAUSED         7

#define BTAUDIO_CODEC_TYPE_A2DP                0
#define BTAUDIO_CODEC_TYPE_HFP                 1
#define BTAUDIO_CODEC_TYPE_HSP                 2
#define BTAUDIO_CODEC_TYPE_LE                 3

#define BTAUDIO_SAMPLE_SOURCE_HOST             0
#define BTAUDIO_SAMPLE_SOURCE_EXTERNAL         1

#define BTAUDIO_SAMPLE_SINK_HOST               0
#define BTAUDIO_SAMPLE_SINK_EXTERNAL           1

#define BTAUDIO_BT_IFACE_HCI                   0
#define BTAUDIO_BT_IFACE_PCM                   1

#define BTAUDIO_SRC_PROCESSOR_EXT              0
#define BTAUDIO_SRC_PROCESSOR_HOST             1
#define BTAUDIO_SNK_PROCESSOR_EXT              0
#define BTAUDIO_SNK_PROCESSOR_HOST             2
#define BTAUDIO_SRC_PROCESSOR_MASK             1
#define BTAUDIO_SNK_PROCESSOR_MASK             2
#define BTAUDIO_PROCESSOR_MASK                 3
#define BTAUDIO_PROCESSOR_HOST                 (BTAUDIO_SNK_PROCESSOR_HOST | BTAUDIO_SRC_PROCESSOR_HOST)
#define BTAUDIO_PROCESSOR_EXT                  0


typedef struct _btaudio_CodecConfig
{
	bt_byte codecType;
	bt_byte btAudioInterface;
	bt_byte audioProcessor;
	
	union
	{
		struct
		{
			bt_byte codecType;
			void* codecConfig;
		} a2dp;
		
		struct
		{
			bt_byte codecType;
            bt_ulong samplingRate;
		} hfp;
		
		struct
		{
			bt_byte codecType;
            bt_ulong samplingRate;
			
			union
			{
				struct
				{
					bt_int minStepSize;
					bt_int maxStepSize;
					bt_int stepDecay;
					bt_int accumNegMin;
					bt_int accumPosMax;
					bt_int accumDecay;
					bt_int J;
					bt_int K;
				} cvsd;
			} codecConfig;
		} le;
	} config;
} btaudio_CodecConfig;

typedef struct _btaudio_MediaPacket btaudio_MediaPacket;
struct _btaudio_MediaPacket 
{
	btaudio_MediaPacket* nextPacket;
	
	bt_byte* data;
	bt_uint dataLen;
	bt_uint maxDataLen;
};

typedef struct
{
    bt_bool silence;

} btaudio_StreamAnalisys;

typedef void (*btaudio_Callback)(bt_int evt, void* param);

void btaudio_init(btaudio_Callback callback, btaudio_Callback recorderCallback);
bt_bool btaudio_configurePlayback(btaudio_CodecConfig* config);
bt_bool btaudio_configureAnalizer(btaudio_CodecConfig* config);
void btaudio_startPlayback(void);
void btaudio_stopPlayback(void);
void btaudio_pausePlayback(void);
void btaudio_resumePlayback(void);
void btaudio_playPacket(btaudio_MediaPacket* packet);
void btaudio_analizeStream(btaudio_MediaPacket* packet);
void btaudio_setVolume(bt_uint volume, bt_bool fromISR);
void btaudio_setVolumeEx(bt_uint volume, bt_bool fromISR);

bt_bool btaudio_configureRecorder(btaudio_CodecConfig* config);
void btaudio_startRecorder(void);
void btaudio_stopRecorder(void);
void btaudio_pauseRecorder(void);
void btaudio_recordPacket(btaudio_MediaPacket* packet);

void btaudio_pal_init(btaudio_Callback callback);
void btaudio_pal_configurePlayback(bt_ulong samplingRate, bt_byte codecType, void* codecConfig, bt_byte btAudioInterface, bt_byte audioProcessor);
void btaudio_pal_configureAnalizer(bt_ulong samplingRate, bt_byte codecType, void* codecConfig);
void btaudio_pal_startPlayback(void);
void btaudio_pal_stopPlayback(void);
void btaudio_pal_pausePlayback(void);
void btaudio_pal_resumePlayback(void);
void btaudio_pal_playPacket(btaudio_MediaPacket* packet);
void btaudio_pal_analizeStream(btaudio_MediaPacket* packet);
void btaudio_pal_setVolume(bt_uint volume, bt_bool fromISR);

void btaudio_pal_configureRecorder(bt_ulong samplingRate, bt_byte codecType, void* codecConfig, bt_byte btAudioInterface, bt_byte audioProcessor);
void btaudio_pal_initRecorder(btaudio_Callback callback);
void btaudio_pal_startRecorder(void);
void btaudio_pal_stopRecorder(void);
void btaudio_pal_pauseRecorder(void);
void btaudio_pal_recordPacket(btaudio_MediaPacket* packet);

#endif // __BTAUDIO_H_INCLUDED__
