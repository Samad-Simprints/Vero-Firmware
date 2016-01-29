/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_BRIDGE_H
#define __ATT_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

struct _bt_att_session_s;
struct _bt_att_client_session_s;

typedef struct _bt_att_bridge_s
{
	struct _bt_att_session_s* server_session;
	struct _bt_att_client_session_s* client_session;
	void* server_session_l2cap_state_changed_param;

} bt_att_bridge_t;


#ifdef __cplusplus
}
#endif

#endif // __ATT_BRIDGE_H
