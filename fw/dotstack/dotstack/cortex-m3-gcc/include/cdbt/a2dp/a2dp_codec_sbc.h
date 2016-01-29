/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __A2DP_CODEC_SBC_H
#define __A2DP_CODEC_SBC_H

#ifdef __cplusplus
extern "C" {
#endif

#define SBC_SAMPLING_FREQUENCY_48000    1
#define SBC_SAMPLING_FREQUENCY_44100    2
#define SBC_SAMPLING_FREQUENCY_32000    4
#define SBC_SAMPLING_FREQUENCY_16000    8
#define SBC_SAMPLING_FREQUENCY_ALL      15

#define SBC_CHANNEL_MODE_JOINT_STEREO   1
#define SBC_CHANNEL_MODE_STEREO         2
#define SBC_CHANNEL_MODE_DUAL_CHANNEL   4
#define SBC_CHANNEL_MODE_MONO           8
#define SBC_CHANNEL_MODE_ALL            15

#define SBC_BLOCK_LENGTH_16    1
#define SBC_BLOCK_LENGTH_12    2
#define SBC_BLOCK_LENGTH_8     4
#define SBC_BLOCK_LENGTH_4     8
#define SBC_BLOCK_LENGTH_ALL   15

#define SBC_SUBBANDS_8    1
#define SBC_SUBBANDS_4    2
#define SBC_SUBBANDS_ALL  3

#define SBC_ALLOCATION_METHOD_LOUDNESS    1
#define SBC_ALLOCATION_METHOD_SNR         2
#define SBC_ALLOCATION_METHOD_ALL         3

typedef struct _bt_a2dp_sbc_config_t
{
	bt_byte sampling_frequency;
	bt_byte channel_mode;
	bt_byte block_length;
	bt_byte subbands;
	bt_byte allocation_method;
	bt_byte min_bitpool_val;
	bt_byte max_bitpool_val;
} bt_a2dp_sbc_config_t;

typedef struct _bt_a2dp_sbc_packet_info_t
{
	bt_byte frames;
	bt_byte l;
	bt_byte s;
	bt_byte f;
	bt_uint frame_size; 
} bt_a2dp_sbc_packet_info_t;

bt_byte bt_a2dp_sbc_codec_handler(bt_avdtp_codec_t* codec, bt_byte opcode, bt_avdtp_codec_op_param_t* op_param, bt_avdtp_mgr_t* mgr);

#ifdef __cplusplus
}
#endif

#endif // __A2DP_CODEC_SBC_H
