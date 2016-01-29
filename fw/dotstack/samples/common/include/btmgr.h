/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTMGR_H_INCLUDED__
#define __BTMGR_H_INCLUDED__

#include "cdbt/bt/bt_std.h"
#include "btapp_config.h"
#ifdef BTAPP_USE_SECURITY_MANAGER
	#include "cdbt/sm/sm.h"
#endif

//-----------------------------------------------------------------------------
//   Public interface.
//-----------------------------------------------------------------------------

#ifndef BTMGR_MAX_LINK_KEYS
#define BTMGR_MAX_LINK_KEYS      2
#endif

#define BTMGR_MAX_DEVICE_NAME    16


#ifdef BTAPP_USE_SECURITY_MANAGER

typedef struct _btmgr_SmLocalKey
{
	bt_byte  key[SMP_KEY_LEN];
	bt_byte  keySize;
	bt_uint  div;
	bt_byte  random[SMP_RAND_LEN];
} btmgr_SmLocalKey;

typedef struct _btmgr_SmPeerKey
{
	bt_byte flags;

	bt_uint div;
	bt_byte random[SMP_RAND_LEN];
	bt_byte ltk[SMP_KEY_LEN];

	bt_bdaddr_t addr;
	bt_byte addr_type;
	bt_byte irk[SMP_KEY_LEN];

	bt_byte csrk[SMP_KEY_LEN];
} btmgr_SmPeerKey;

#endif

typedef struct _btmgr_PersistentData
{
	bt_uint      signature;
        bt_uint      size;
	bt_bdaddr_t  lastConnectedDevice;
        bt_uint      nextSlot;
	struct
	{
		bt_bdaddr_t  addr;
		bt_linkkey_t linkKey;
	} linkKeys[BTMGR_MAX_LINK_KEYS];

#ifdef BTAPP_USE_SECURITY_MANAGER
	
        bt_uint      nextSmLocalSlot;
	btmgr_SmLocalKey  smLocalLinkKeys[BTMGR_MAX_LINK_KEYS];

        bt_uint      nextSmPeerSlot;
	btmgr_SmPeerKey  smPeerLinkKeys[BTMGR_MAX_LINK_KEYS];
#endif
        
        char         deviceName[BTMGR_MAX_DEVICE_NAME + 1];
        bt_ulong     connectedProfiles;
        bt_ulong     lastConnectedProfiles;
		bt_bdaddr_t  randomAddr;

} btmgr_PersistentData;


void btmgr_init(void);

void btmgr_start(void);

bt_bool btmgr_getLastConnectedDevice(bt_bdaddr_t* bdaddr);

void btmgr_setLastConnectedDevice(const bt_bdaddr_t* bdaddr);

void btmgr_setDeviceName(const char* name);

const char* btmgr_getDeviceName(void);

void btmgr_clearDeviceInfo(const bt_bdaddr_t* bdaddr);

void btmgr_clearAllDeviceInfo(void);

void btmgr_initSecurityManager(void);

void btmgr_startSecurityManager(void);

void btmgr_setProfileConnected(bt_byte profileId);

bt_bool btmgr_getProfileWasConnected(bt_byte profileId);

const bt_bdaddr_t* btmgr_getRandomAddress(void);

void btmgr_setRandomAddress(const bt_bdaddr_t* bdaddr);

#ifdef BTAPP_USE_SECURITY_MANAGER

void btmgr_saveSmLocalLinkKey(const bt_sm_ltk_t* key);

bt_bool btmgr_findSmLocalLinkKey(bt_uint div, bt_sm_ltk_t* key);

void btmgr_saveSmPeerLinkKey(const bt_sm_evt_peer_keys_notif_t* keys);

#endif

//-----------------------------------------------------------------------------
//   Platform Abstraction Layer interface.
//-----------------------------------------------------------------------------

typedef void (*btmgr_pal_WritePersistentDataCallback)(void);

void btmgr_pal_initStorage(void);

btmgr_PersistentData* btmgr_pal_getPersistentData(void);

void btmgr_pal_writePersistentData(btmgr_pal_WritePersistentDataCallback callback);

void btmgr_pal_configureSCO(void);

#endif // __BTMGR_H_INCLUDED__
