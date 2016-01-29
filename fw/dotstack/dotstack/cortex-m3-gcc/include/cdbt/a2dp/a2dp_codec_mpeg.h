/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __A2DP_CODEC_MPEG_H
#define __A2DP_CODEC_MPEG_H

#ifdef __cplusplus
extern "C" {
#endif

#define MPEG_LAYER_1      0x04
#define MPEG_LAYER_2      0x02
#define MPEG_LAYER_3      0x01
#define MPEG_LAYER_ALL    0x07

#define MPEG_CRC_PROTECTION_NOT_SUPPORTED    0
#define MPEG_CRC_PROTECTION_SUPPORTED        1

#define MPEG_SAMPLING_FREQUENCY_48000    0x01
#define MPEG_SAMPLING_FREQUENCY_44100    0x02
#define MPEG_SAMPLING_FREQUENCY_32000    0x04
#define MPEG_SAMPLING_FREQUENCY_24000    0x08
#define MPEG_SAMPLING_FREQUENCY_22050    0x10
#define MPEG_SAMPLING_FREQUENCY_16000    0x20
#define MPEG_SAMPLING_FREQUENCY_ALL      0x3F

#define MPEG_CHANNEL_MODE_JOINT_STEREO   1
#define MPEG_CHANNEL_MODE_STEREO         2
#define MPEG_CHANNEL_MODE_DUAL_CHANNEL   4
#define MPEG_CHANNEL_MODE_MONO           8
#define MPEG_CHANNEL_MODE_ALL            15

#define MPEG_MPF_1    0
#define MPEG_MPF_2    1

#define MPEG_VBR_NOT_SUPPORTED    0
#define MPEG_VBR_SUPPORTED        1

#define MPEG_BITRATE_1000    0x0001
#define MPEG_BITRATE_1001    0x0002
#define MPEG_BITRATE_1010    0x0004
#define MPEG_BITRATE_1011    0x0008
#define MPEG_BITRATE_1100    0x0010
#define MPEG_BITRATE_1101    0x0020
#define MPEG_BITRATE_1110    0x0040
#define MPEG_BITRATE_0000    0x0080
#define MPEG_BITRATE_0001    0x0100
#define MPEG_BITRATE_0010    0x0200
#define MPEG_BITRATE_0011    0x0400
#define MPEG_BITRATE_0100    0x0800
#define MPEG_BITRATE_0101    0x1000
#define MPEG_BITRATE_0110    0x2000
#define MPEG_BITRATE_0111    0x4000
#define MPEG_BITRATE_ALL     0x7FFF

typedef struct _bt_a2dp_mpeg_config_t
{
	bt_byte layer;
	bt_byte crc;
	bt_byte channel_mode;
	bt_byte rfa;
	bt_byte mpf;
	bt_byte sampling_frequency;
	bt_byte vbr;
	bt_uint bit_rate;
} bt_a2dp_mpeg_config_t;

bt_byte bt_a2dp_mpeg_codec_handler(bt_avdtp_codec_t* codec, bt_byte opcode, bt_avdtp_codec_op_param_t* op_param, bt_avdtp_mgr_t* mgr);

#ifdef __cplusplus
}
#endif

#endif // __A2DP_CODEC_MPEG_H
