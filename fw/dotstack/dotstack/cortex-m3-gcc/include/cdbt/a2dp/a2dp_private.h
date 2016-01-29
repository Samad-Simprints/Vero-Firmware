/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __A2DP_PRIVATE_H
#define __A2DP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

void _bt_a2dp_avdtp_mgr_callback(bt_avdtp_mgr_t* mgr, bt_byte evt, bt_avdtp_event_t* evt_param, void* callback_param);

#ifdef __cplusplus
}
#endif

#endif // __A2DP_PRIVATE_H
