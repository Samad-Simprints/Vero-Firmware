/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*
* SEARAN LLC is the exclusive licensee and developer of dotstack with
* all its modifications and enhancements.
*
* Contains proprietary and confidential information of CandleDragon and
* may not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2009, 2010, 2011 CandleDragon. All Rights Reserved.
*******************************************************************************/

#ifndef __OPP_PRIVATE_H
#define __OPP_PRIVATE_H

#include "cdbt/obex/obex.h"
#include "cdbt/sdp/sdp.h"

#ifdef __cplusplus
extern "C" {
#endif

void bt_opp_init_sessions(void);
bt_opp_session* bt_opp_find_session(bt_obex_session* obex_session);
bt_bool bt_opp_find_server(bt_opp_session* session, bt_bdaddr_t* deviceAddress, bt_opp_find_server_callback_pf callback);

#ifdef __cplusplus
}
#endif

#endif // __OPP_PRIVATE_H
