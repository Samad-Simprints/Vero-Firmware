/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __A2DP_CODEC_AAC_H
#define __A2DP_CODEC_AAC_H

#ifdef __cplusplus
extern "C" {
#endif

#define AAC_OBJECT_TYPE_MPEG_2_LC           0x80
#define AAC_OBJECT_TYPE_MPEG_4_LC           0x40
#define AAC_OBJECT_TYPE_MPEG_4_LTP          0x20
#define AAC_OBJECT_TYPE_MPEG_4_SCALABLE     0x10

#define MPEG_LAYER_1      0x04
#define MPEG_LAYER_2      0x02
#define MPEG_LAYER_3      0x01
#define MPEG_LAYER_ALL    0x07

#define MPEG_CRC_PROTECTION_NOT_SUPPORTED    0
#define MPEG_CRC_PROTECTION_SUPPORTED        1

#define AAC_SAMPLING_FREQUENCY_8000     0x080
#define AAC_SAMPLING_FREQUENCY_11025    0x040
#define AAC_SAMPLING_FREQUENCY_12000    0x020
#define AAC_SAMPLING_FREQUENCY_16000    0x010
#define AAC_SAMPLING_FREQUENCY_22050    0x008
#define AAC_SAMPLING_FREQUENCY_24000    0x004
#define AAC_SAMPLING_FREQUENCY_32000    0x002
#define AAC_SAMPLING_FREQUENCY_44100    0x001
#define AAC_SAMPLING_FREQUENCY_48000    0x800
#define AAC_SAMPLING_FREQUENCY_64000    0x400
#define AAC_SAMPLING_FREQUENCY_88200    0x200
#define AAC_SAMPLING_FREQUENCY_96000    0x100
#define AAC_SAMPLING_FREQUENCY_ALL      0xFFF

#define AAC_CHANNELS_1      0x2
#define AAC_CHANNELS_2      0x1
#define AAC_CHANNELS_ALL    0x3

#define AAC_VBR_NOT_SUPPORTED    0
#define AAC_VBR_SUPPORTED        1

typedef struct _bt_a2dp_aac_config_t
{
	bt_byte object_type;
	bt_uint sampling_frequency;
	bt_byte channels;
	bt_byte rfa;
	bt_byte vbr;
	bt_ulong bit_rate;
} bt_a2dp_aac_config_t;

bt_byte bt_a2dp_aac_codec_handler(bt_avdtp_codec_t* codec, bt_byte opcode, bt_avdtp_codec_op_param_t* op_param, bt_avdtp_mgr_t* mgr);

#ifdef __cplusplus
}
#endif

#endif // __A2DP_CODEC_AAC_H
