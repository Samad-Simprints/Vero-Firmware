/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include <string.h>

#include "btmgr.h"
#include "btdisplay.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_oem.h"
#include "cdbt/hci/hci.h"

#define BTLOG_TAG "BTMGR"
#include "btlog_define.h"

#define SIGNATURE                         0x4826

#define FLAG_CHANGED                      0x01
#define FLAG_WRITING                      0x02

static unsigned char mFlags;

static void writePersistentData(void);
static void writePersistentDataCallback(void);


void btmgr_init(void)
{
	mFlags = 0;
}

void btmgr_start(void)
{
	btmgr_PersistentData* data;
	data = btmgr_pal_getPersistentData();
	// Check it is valid
	if (data->signature != SIGNATURE || data->nextSlot > BTMGR_MAX_LINK_KEYS - 1 || data->size != sizeof(btmgr_PersistentData))
	{
		// Initialzie RAM copy if data in info memory is corrupt or not initialized.
		memset(data, 0, sizeof(btmgr_PersistentData));
		data->signature = SIGNATURE;
		data->size = sizeof(btmgr_PersistentData);
	}
	else
	{
		data->lastConnectedProfiles = data->connectedProfiles;
		data->connectedProfiles = 0;
	}
}


bt_bool btmgr_getLastConnectedDevice(bt_bdaddr_t* bdaddr)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	memcpy(bdaddr, &data->lastConnectedDevice, sizeof(bt_bdaddr_t));
	return !bt_bdaddr_is_null(bdaddr);
}


void btmgr_setLastConnectedDevice(const bt_bdaddr_t* bdaddr)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	memcpy(&data->lastConnectedDevice, bdaddr, sizeof(bt_bdaddr_t));
	
	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

void btmgr_setProfileConnected(bt_byte profileId)
{
	if (profileId < 32)
	{
		btmgr_PersistentData* data = btmgr_pal_getPersistentData();

		data->connectedProfiles |= 1 << profileId;
		data->lastConnectedProfiles |= 1 << profileId;
		mFlags |= FLAG_CHANGED;
		writePersistentData();
	}
}

bt_bool btmgr_getProfileWasConnected(bt_byte profileId)
{
	if (profileId < 32)
	{
		btmgr_PersistentData* data = btmgr_pal_getPersistentData();

		return (data->lastConnectedProfiles & (1 << profileId)) ? BT_TRUE : BT_FALSE;
	}
	else
	{
		return 0;
	}
}

void btmgr_setDeviceName(const char* name)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	size_t len = strlen(name);

	if (len > BTMGR_MAX_DEVICE_NAME)
		len = BTMGR_MAX_DEVICE_NAME;

	memcpy(data->deviceName, name, len);
	data->deviceName[len] = 0;
	
	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

const char* btmgr_getDeviceName(void)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	return data->deviceName;
}

void btmgr_clearDeviceInfo(const bt_bdaddr_t* bdaddr)
{
	int i;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();

	for (i = 0; i < BTMGR_MAX_LINK_KEYS; i++)
	{
		if (bt_bdaddrs_are_equal(&data->linkKeys[i].addr, (bt_bdaddr_t*)bdaddr))
		{
			memset(&data->linkKeys[i].addr, 0, sizeof(bt_bdaddr_t));
			memset(&data->linkKeys[i].linkKey, 0, sizeof(bt_linkkey_t));
			mFlags |= FLAG_CHANGED;
			writePersistentData();
			break;
		}
	}
}

void btmgr_clearAllDeviceInfo()
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	memset(data, 0, sizeof(btmgr_PersistentData));
	data->signature = SIGNATURE;
	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

const bt_bdaddr_t* btmgr_getRandomAddress(void)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	return &data->randomAddr;
}

void btmgr_setRandomAddress(const bt_bdaddr_t* bdaddr)
{
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	data->randomAddr = *bdaddr;
	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

static void writePersistentData(void)
{
	if ((mFlags & FLAG_CHANGED) && !(mFlags & FLAG_WRITING))
	{
		mFlags &= ~FLAG_CHANGED;
		mFlags |= FLAG_WRITING;
		btmgr_pal_writePersistentData(&writePersistentDataCallback);
	}
}


static void writePersistentDataCallback(void)
{
	mFlags &= ~FLAG_WRITING;
	
	// It is possible that data was changed while it was being written.
	// In this case we write it again.
	if (mFlags & FLAG_CHANGED)
	{
		writePersistentData();
	}
}

#ifdef BTAPP_USE_SECURITY_MANAGER

// 
// BLE link keys
//--------------------------------------------------------------------
//
void btmgr_saveSmLocalLinkKey(const bt_sm_ltk_t* key)
{
	int i;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	btmgr_SmLocalKey* keyTmp;
	
	// Take the next slot. This may override previously stored link key
	// for another device.
	i = data->nextSmLocalSlot;
	data->nextSmLocalSlot = (data->nextSmLocalSlot + 1) % BTMGR_MAX_LINK_KEYS;

	// Save the link key.
	keyTmp = &data->smLocalLinkKeys[i];
	memcpy(keyTmp->key, key->ltk, SMP_KEY_LEN);
	memcpy(keyTmp->random, key->rand, SMP_RAND_LEN);
	keyTmp->div = key->div;
	keyTmp->keySize = key->ltk_len;

	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

bt_bool btmgr_findSmLocalLinkKey(bt_uint div, bt_sm_ltk_t* key)
{
	int i;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	for (i = 0; i < BTMGR_MAX_LINK_KEYS; i++)
	{
		btmgr_SmLocalKey* keyTmp = &data->smLocalLinkKeys[i];
		
		if (keyTmp->div == div)
		{
			key->div = keyTmp->div;
			key->rand = keyTmp->random;
			key->ltk = keyTmp->key;
			key->ltk_len = keyTmp->keySize;
			return BT_TRUE;
		}
	}
	
	return BT_FALSE;
}

void btmgr_saveSmPeerLinkKey(const bt_sm_evt_peer_keys_notif_t* keys)
{
	int i = 0;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	btmgr_SmPeerKey* keyTmp;
	
	// Look if we already have a key with the remote device.
	// If so, use that slot. Do this only if IRK has been distributed by the peer device and
	// the address is either public (keys->addr_type == 0 and keys->addr is not all zeroes) or 
	// static random (keys->addr_type == 1)
	if ((keys->flags & SM_PEER_KEYS_IRK_VALID) && ((keys->addr_type == 0 && !bt_bdaddr_is_null(keys->addr)) || keys->addr_type == 1))
	{
		for (i = 0; i < BTMGR_MAX_LINK_KEYS; i++)
		{
			if (bt_bdaddrs_are_equal(&data->smPeerLinkKeys[i].addr, keys->addr))
			{
				break;
			}
		}
	}
	
	
	if (i == BTMGR_MAX_LINK_KEYS)
	{
		// Take the next slot. This may override previously stored link key
		// for another device.
		i = data->nextSmPeerSlot;
		data->nextSmPeerSlot = (data->nextSmPeerSlot + 1) % BTMGR_MAX_LINK_KEYS;
	}

	// Save the link key.
	keyTmp = &data->smPeerLinkKeys[i];
	keyTmp->flags = keys->flags;

	keyTmp->div = keys->div;
	memcpy(keyTmp->random, keys->random, SMP_RAND_LEN);
	memcpy(keyTmp->ltk, keys->ltk, SMP_KEY_LEN);
	
	keyTmp->addr = *keys->addr;
	keyTmp->addr_type = keys->addr_type;
	memcpy(keyTmp->irk, keys->irk, SMP_KEY_LEN);
	
	memcpy(keyTmp->csrk, keys->csrk, SMP_KEY_LEN);

	mFlags |= FLAG_CHANGED;
	writePersistentData();
}

#endif

//
// Bluetooth link keys
// -------------------------------------------------------------------
//
void bt_oem_linkkey_notification(bt_linkkey_notification_t* lkn)
{
	int i;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();
	
	// Look if we already have a key with the remote device.
	// If so, use that slot.
	for (i = 0; i < BTMGR_MAX_LINK_KEYS; i++)
	{
		if (bt_bdaddrs_are_equal(&data->linkKeys[i].addr, &lkn->bdaddr_remote))
		{
			break;
		}
	}
	
	if (i == BTMGR_MAX_LINK_KEYS)
	{
		// Take the next slot. This may override previously stored link key
		// for another device.
		i = data->nextSlot;
		data->nextSlot = (data->nextSlot + 1) % BTMGR_MAX_LINK_KEYS;
	}

	// Save the link key.
	data->linkKeys[i].addr = lkn->bdaddr_remote;
	memcpy(&data->linkKeys[i].linkKey, &lkn->key, sizeof(bt_linkkey_t));
	
	// Also save the BD address of the device we just paired with as the last
	// connected device address.
	btmgr_setLastConnectedDevice(&lkn->bdaddr_remote); // this also will call writePersistentData()
	
	btdisplay_setPairedState(BT_TRUE);
	btdisplay_update();

//	static char msg[32] =
//	{
//		"Link key from "
//	};
//	bt_bdaddr_t* bdaddr = &lkn->bdaddr_remote;
//	int len = 14;
//
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_m & 0xF000) >> 12);
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_m & 0x0F00) >> 8);
//	msg[len++] = ':';
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_m & 0x00F0) >> 4);
//	msg[len++] = *_ulong2str(bdaddr->bd_addr_m & 0x000F);
//	msg[len++] = ':';
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0xF0000000) >> 28);
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x0F000000) >> 24);
//	msg[len++] = ':';
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x00F00000) >> 20);
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x000F0000) >> 16);
//	msg[len++] = ':';
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x0000F000) >> 12);
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x00000F00) >> 8);
//	msg[len++] = ':';
//	msg[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x000000F0) >> 4);
//	msg[len++] = *_ulong2str(bdaddr->bd_addr_l & 0x0000000F);
//	msg[len] = 0;
//
//	BTLOG(msg);
}


void bt_oem_linkkey_request(bt_linkkey_request_t* lkr)
{
	int i;
	btmgr_PersistentData* data = btmgr_pal_getPersistentData();

//	static char msg1[40] =
//	{
//		"Link key request from "
//	};
//	bt_bdaddr_t* bdaddr = &lkr->bdaddr_remote;
//	int len = 22;
//
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_m & 0xF000) >> 12);
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_m & 0x0F00) >> 8);
//	msg1[len++] = ':';
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_m & 0x00F0) >> 4);
//	msg1[len++] = *_ulong2str(bdaddr->bd_addr_m & 0x000F);
//	msg1[len++] = ':';
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0xF0000000) >> 28);
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x0F000000) >> 24);
//	msg1[len++] = ':';
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x00F00000) >> 20);
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x000F0000) >> 16);
//	msg1[len++] = ':';
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x0000F000) >> 12);
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x00000F00) >> 8);
//	msg1[len++] = ':';
//	msg1[len++] = *_ulong2str((bdaddr->bd_addr_l & 0x000000F0) >> 4);
//	msg1[len++] = *_ulong2str(bdaddr->bd_addr_l & 0x0000000F);
//	msg1[len] = 0;
//
//	BTLOG(msg1);

	for (i = 0; i < BTMGR_MAX_LINK_KEYS; i++)
	{
		if (bt_bdaddrs_are_equal(&data->linkKeys[i].addr, &lkr->bdaddr_remote))
		{
			break;
		}
	}

	if (i < BTMGR_MAX_LINK_KEYS)
	{
		bt_hci_send_linkkey(&lkr->bdaddr_remote, &data->linkKeys[i].linkKey, NULL);
	}
	else
	{
		bt_hci_send_linkkey(&lkr->bdaddr_remote, NULL, NULL);
	}
}

