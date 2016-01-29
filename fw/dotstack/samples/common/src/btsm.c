/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"
#include "cdbt/sm/sm.h"

#include "btmgr.h"

#define BTLOG_TAG "SM"
#include "btlog_define.h"

const char* ER =   "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
const char* DHK =  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01";
const char* IRK =  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02";
const char* CSRK = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03";

static bt_sm_listener_t mListener;

static void smMgrCallback(bt_int evt, bt_sm_event_t* evt_param, void* param);

void btmgr_initSecurityManager(void)
{
	bt_sm_init();
	bt_sm_set_er(ER);
	bt_sm_set_dhk(DHK);
	bt_sm_set_irk(IRK);
	bt_sm_set_csrk(CSRK);
}

void btmgr_startSecurityManager(void)
{
	bt_sm_start();
	
	mListener.callback = &smMgrCallback;
	mListener.callback_param = NULL;
	bt_sm_register_listener(&mListener);
}

static void smMgrCallback(bt_int evt, bt_sm_event_t* evt_param, void* param)
{
	switch (evt)
	{
		case SM_EVENT_CONNECTED:
			break;

		case SM_EVENT_DISCONNECTED:
			break;

		case SM_EVENT_PAIRING_REQUEST:
		{
			bt_sm_pairing_features_t features;

			features.io_capability = SMP_IO_CAPABILITY_NOINPUT_NOOUTPUT;//SMP_IO_CAPABILITY_KEYBOARD_DISPLAY;// 
			features.oob_data_flag = SMP_OOB_DATA_NOT_PRESENT;
			features.auth_req = SMP_AUTHENTICATION_REQUIREMENTS_BONDING | SMP_AUTHENTICATION_REQUIREMENTS_MITM_REQUIRED;
			features.min_key_size = 0;//SMP_KEY_LEN;
			features.max_key_size = SMP_KEY_LEN;
			features.ikd = SMP_KEY_DISTRIBUTION_ENC_KEY | SMP_KEY_DISTRIBUTION_ID_KEY | SMP_KEY_DISTRIBUTION_SIGN_KEY;//0;
			features.rkd = SMP_KEY_DISTRIBUTION_ENC_KEY | SMP_KEY_DISTRIBUTION_ID_KEY | SMP_KEY_DISTRIBUTION_SIGN_KEY;

			bt_sm_set_pairing_features(evt_param->pairing_request.session, SMP_PAIRING_ERROR_SUCCESS, &features);

			break;
		}

		case SM_EVENT_PAIRING_COMPLETE:
		{
			BTLOG1("Pairing completed with status: %d", evt_param->pairing_complete.status);

			break;
		}

		case SM_EVENT_PASSKEY_REQUEST:
		{

			break;
		}

		case SM_EVENT_GENERATE_LTK:
		{
			bt_sm_generate_ltk(evt_param->generate_ltk.session, evt_param->generate_ltk.key_size);
			break;
		}

		case SM_EVENT_LTK_GENERATED:
		{
			btmgr_saveSmLocalLinkKey(&evt_param->ltk_generated.ltk);
			bt_sm_distribute_ltk(evt_param->ltk_generated.session, &evt_param->ltk_generated.ltk);
			
			break;
		}

		case SM_EVENT_LOCAL_LTK_REQUEST:
		{
			bt_sm_ltk_t ltk;
			
			if (btmgr_findSmLocalLinkKey(evt_param->slave_ltk_request.div, &ltk))
				bt_sm_send_ltk(evt_param->slave_ltk_request.session, &ltk);
			else
				bt_sm_send_ltk(evt_param->slave_ltk_request.session, NULL);
			
			break;
		}

		case SM_EVENT_PEER_LTK_REQUEST:
			break;

		case SM_EVENT_PEER_KEYS_NOTIFICATION:
			btmgr_saveSmPeerLinkKey(&evt_param->peer_keys);
			
			break;
	}
}

