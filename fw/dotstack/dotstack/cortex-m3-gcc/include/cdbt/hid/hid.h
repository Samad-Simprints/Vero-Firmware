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

#ifndef __HID_H
#define __HID_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* \defgroup hid HID Profile (HIDP)
*/

#define HID_ITEM_SIZE_0	0x0
#define HID_ITEM_SIZE_1	0x1
#define HID_ITEM_SIZE_2	0x2
#define HID_ITEM_SIZE_4	0x3

#define HID_ITEM_TYPE_MAIN		0x00
#define HID_ITEM_TYPE_GLOBAL	0x04
#define HID_ITEM_TYPE_LOCAL		0x08
#define HID_ITEM_TYPE_RESERVED	0x0C

#define HID_ITEM_MAIN_TAG_INPUT			0x80
#define HID_ITEM_MAIN_TAG_OUTPUT		0x90
#define HID_ITEM_MAIN_TAG_FEATURE		0xB0
#define HID_ITEM_MAIN_TAG_COLLECTION	0xA0
#define HID_ITEM_MAIN_TAG_COLLECTIONEND	0xC0

#define HID_ITEM_GLOBAL_TAG_USAGEPAGE		0x00
#define HID_ITEM_GLOBAL_TAG_LOGICAL_MIN		0x10
#define HID_ITEM_GLOBAL_TAG_LOGICAL_MAX		0x20
#define HID_ITEM_GLOBAL_TAG_PHYSICAL_MIN	0x30
#define HID_ITEM_GLOBAL_TAG_PHYSICAL_MAX	0x40
#define HID_ITEM_GLOBAL_TAG_UNIT_EXPONENT	0x50
#define HID_ITEM_GLOBAL_TAG_UNIT			0x60
#define HID_ITEM_GLOBAL_TAG_REPORTSIZE		0x70
#define HID_ITEM_GLOBAL_TAG_REPORTID		0x80
#define HID_ITEM_GLOBAL_TAG_REPORTCOUNT		0x90
#define HID_ITEM_GLOBAL_TAG_PUSH			0xA0
#define HID_ITEM_GLOBAL_TAG_POP				0xB0

#define HID_ITEM_LOCAL_TAG_USAGE			0x00
#define HID_ITEM_LOCAL_TAG_USAGEMIN			0x10
#define HID_ITEM_LOCAL_TAG_USAGEMAX			0x20
#define HID_ITEM_LOCAL_TAG_DESIGNATORINDEX	0x30
#define HID_ITEM_LOCAL_TAG_DESIGNATORMIN	0x40
#define HID_ITEM_LOCAL_TAG_DESIGNATORMAX	0x50
#define HID_ITEM_LOCAL_TAG_STRINGINDEX		0x70
#define HID_ITEM_LOCAL_TAG_STRINGMIN		0x80
#define HID_ITEM_LOCAL_TAG_STRINGMAX		0x90
#define HID_ITEM_LOCAL_TAG_DELIMITER		0xA0

#define HID_ITEM_FLAG_DATA		0x000
#define HID_ITEM_FLAG_CONST		0x001
#define HID_ITEM_FLAG_ARRAY		0x000
#define HID_ITEM_FLAG_VAR		0x002
#define HID_ITEM_FLAG_ABS		0x000
#define HID_ITEM_FLAG_REL		0x004
#define HID_ITEM_FLAG_NOWRAP	0x000
#define HID_ITEM_FLAG_WRAP		0x008
#define HID_ITEM_FLAG_LINEAR	0x000
#define HID_ITEM_FLAG_NONLINEAR	0x010
#define HID_ITEM_FLAG_PREF		0x000
#define HID_ITEM_FLAG_NOPREF	0x020
#define HID_ITEM_FLAG_NONULL	0x000
#define HID_ITEM_FLAG_NULL		0x040
#define HID_ITEM_FLAG_BITFILED	0x000
#define HID_ITEM_FLAG_BYTES		0x100

#define HID_COLLECTION_TYPE_PHYSICAL	0x00
#define HID_COLLECTION_TYPE_APPLICATION	0x01
#define HID_COLLECTION_TYPE_LOGICAL		0x02
#define HID_COLLECTION_TYPE_REPORT		0x03
#define HID_COLLECTION_TYPE_NAMEDARRAY	0x04
#define HID_COLLECTION_TYPE_USAGESWITCH	0x05
#define HID_COLLECTION_TYPE_USAGEMOD	0x06

#define HID_ITEM(tag, type, size)		(char)(tag | type | size),

#define HID_ITEM_MAIN(tag, flags)		HID_ITEM(tag, HID_ITEM_TYPE_MAIN, HID_ITEM_SIZE_1)		(char)flags,
#define HID_ITEM_INPUT(flags)			HID_ITEM_MAIN(HID_ITEM_MAIN_TAG_INPUT, flags)
#define HID_ITEM_OUTPUT(flags)			HID_ITEM_MAIN(HID_ITEM_MAIN_TAG_OUTPUT, flags)
#define HID_ITEM_FEATURE(flags)			HID_ITEM_MAIN(HID_ITEM_MAIN_TAG_FEATURE, flags)

#define HID_ITEM_GLOBAL(tag, value)		HID_ITEM(tag, HID_ITEM_TYPE_GLOBAL, HID_ITEM_SIZE_1)	(char)value,
#define HID_ITEM_GLOBAL2(tag, value)	HID_ITEM(tag, HID_ITEM_TYPE_GLOBAL, HID_ITEM_SIZE_2)	(char)(value & 0xFF), (char)((value & 0xFF00) >> 8),
#define HID_ITEM_USAGEPAGE(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_USAGEPAGE, value)
#define HID_ITEM_USAGEPAGE2(value)		HID_ITEM_GLOBAL2(HID_ITEM_GLOBAL_TAG_USAGEPAGE, value)
#define HID_ITEM_LOGICAL_MIN(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_LOGICAL_MIN, value)
#define HID_ITEM_LOGICAL_MAX(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_LOGICAL_MAX, value)
#define HID_ITEM_LOGICAL_MIN2(value)	HID_ITEM_GLOBAL2(HID_ITEM_GLOBAL_TAG_LOGICAL_MIN, value)
#define HID_ITEM_LOGICAL_MAX2(value)	HID_ITEM_GLOBAL2(HID_ITEM_GLOBAL_TAG_LOGICAL_MAX, value)
#define HID_ITEM_PHYSICAL_MIN(value)	HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_PHYSICAL_MIN, value)
#define HID_ITEM_PHYSICAL_MAX(value)	HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_PHYSICAL_MAX, value)
#define HID_ITEM_UNIT_EXPONENT(value)	HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_UNIT_EXPONENT, value)
#define HID_ITEM_UNIT(value)			HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_UNIT, value)
#define HID_ITEM_REPORTSIZE(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_REPORTSIZE, value)
#define HID_ITEM_REPORTID(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_REPORTID, value)
#define HID_ITEM_REPORTCOUNT(value)		HID_ITEM_GLOBAL(HID_ITEM_GLOBAL_TAG_REPORTCOUNT, value)
#define HID_ITEM_REPORTCOUNT2(value)	HID_ITEM_GLOBAL2(HID_ITEM_GLOBAL_TAG_REPORTCOUNT, value)
#define HID_ITEM_PUSH()					HID_ITEM(HID_ITEM_GLOBAL_TAG_PUSH, HID_ITEM_TYPE_GLOBAL, HID_ITEM_SIZE_0)
#define HID_ITEM_POP()					HID_ITEM(HID_ITEM_GLOBAL_TAG_POP, HID_ITEM_TYPE_GLOBAL, HID_ITEM_SIZE_0)

#define HID_ITEM_LOCAL(tag, value)			HID_ITEM(tag, HID_ITEM_TYPE_LOCAL, HID_ITEM_SIZE_1)		(char)value,
#define HID_ITEM_USAGE(value)				HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_USAGE, value)
#define HID_ITEM_USAGEMIN(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_USAGEMIN, value)
#define HID_ITEM_USAGEMAX(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_USAGEMAX, value)
#define HID_ITEM_DESIGNATORINDEX(value)		HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_DESIGNATORINDEX, value)
#define HID_ITEM_DESIGNATORMIN(value)		HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_DESIGNATORMIN, value)
#define HID_ITEM_DESIGNATORMAX(value)		HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_DESIGNATORMAX, value)
#define HID_ITEM_STRINGINDEX(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_STRINGINDEX, value)
#define HID_ITEM_STRINGMIN(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_STRINGMIN, value)
#define HID_ITEM_STRINGMAX(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_STRINGMAX, value)
#define HID_ITEM_DELIMITER(value)			HID_ITEM_LOCAL(HID_ITEM_LOCAL_TAG_DELIMITER, value)

#define HID_BEGIN_COLLECTION(type)	HID_ITEM(HID_ITEM_MAIN_TAG_COLLECTION, HID_ITEM_TYPE_MAIN, HID_ITEM_SIZE_1) (char)type,
#define HID_END_COLLECTION()	HID_ITEM(HID_ITEM_MAIN_TAG_COLLECTIONEND, HID_ITEM_TYPE_MAIN, HID_ITEM_SIZE_0)

#define HID_BEGIN_REPORT(id)	const char id[] = {
#define HID_END_REPORT()		};


#define HID_USAGEPAGE_GENERIC_DESKTOP_CONTROLS	0x01
#define HID_USAGEPAGE_SIMULATION_CONTROLS		0x02
#define HID_USAGEPAGE_VR_CONTROLS				0x03
#define HID_USAGEPAGE_SPORT_CONTROLS			0x04
#define HID_USAGEPAGE_GAME_CONTROLS				0x05
#define HID_USAGEPAGE_GENERIC_DEVICE_CONTROLS	0x06
#define HID_USAGEPAGE_KEYBOARD					0x07
#define HID_USAGEPAGE_LED						0x08
#define HID_USAGEPAGE_BUTTON					0x09
#define HID_USAGEPAGE_ORDINAL					0x0A
#define HID_USAGEPAGE_TELEPHONY					0x0B
#define HID_USAGEPAGE_CONSUMER					0x0C
#define HID_USAGEPAGE_DIGITIZER					0x0D
#define HID_USAGEPAGE_PID						0x0F
#define HID_USAGEPAGE_UNICODE					0x10
#define HID_USAGEPAGE_ALPHANUMERIC_DISPLAY		0x14
#define HID_USAGEPAGE_MEDICAL_INSTRUMENTS		0x40
#define HID_USAGEPAGE_BARCODE_SCANNER			0x8C
#define HID_USAGEPAGE_SCALE						0x8D
#define HID_USAGEPAGE_MSR						0x8E // Magnetic stripe reading devices
#define HID_USAGEPAGE_POINT_OF_SALE				0x8F

#define HID_USAGE_GENERIC_POINTER	0x01
#define HID_USAGE_GENERIC_MOUSE		0x02
#define HID_USAGE_GENERIC_KEYBOARD	0x06
#define HID_USAGE_GENERIC_MULTIAXIS	0x08
#define HID_USAGE_GENERIC_X			0x30
#define HID_USAGE_GENERIC_Y			0x31


#define HID_DESCRIPTOR_TYPE_REPORT		0x22
#define HID_DESCRIPTOR_TYPE_PHYSICAL	0x23

#define HID_REPORTID_NONE	0

#define HID_CHANNEL_CTL		0
#define HID_CHANNEL_INT		1


#define HID_TRANTYPE_HANDSHAKE		0x00
#define HID_TRANTYPE_HID_CONTROL	0x10
#define HID_TRANTYPE_GET_REPORT		0x40
#define HID_TRANTYPE_SET_REPORT		0x50
#define HID_TRANTYPE_GET_PROTOCOL	0x60
#define HID_TRANTYPE_SET_PROTOCOL	0x70
#define HID_TRANTYPE_GET_IDLE		0x80
#define HID_TRANTYPE_SET_IDLE		0x90
#define HID_TRANTYPE_DATA			0xA0
#define HID_TRANTYPE_DATC			0xB0

#define HID_HANDSHAKE_SUCCESS				0x00	
#define HID_HANDSHAKE_NOT_READY				0x01
#define HID_HANDSHAKE_ERR_INVALID_REPORT_ID	0x02
#define HID_HANDSHAKE_ERR_UNSUPPORTED_REQ	0x03
#define HID_HANDSHAKE_ERR_INVALID_PARAMETER	0x04
#define HID_HANDSHAKE_ERR_UNKNOWN			0x0E
#define HID_HANDSHAKE_ERR_FATAL				0x0F

#define HID_HID_CONTROL_NOP						0x00
#define HID_HID_CONTROL_HARD_RESET				0x01
#define HID_HID_CONTROL_SOFT_RESET				0x02
#define HID_HID_CONTROL_SUSPEND					0x03
#define HID_HID_CONTROL_EXIT_SUSPEND			0x04
#define HID_HID_CONTROL_VIRTUAL_CABLE_UNPLUG	0x05

#define HID_GET_REPORT_WITH_BUFFER_SIZE	0x08

#define HID_REPORT_TYPE_OTHER	0x00
#define HID_REPORT_TYPE_INPUT	0x01
#define HID_REPORT_TYPE_OUTPUT	0x02
#define HID_REPORT_TYPE_FEATURE	0x03

#define HID_SESSION_STATE_FREE			0
#define HID_SESSION_STATE_CONNECTED 	1
#define HID_SESSION_STATE_DISCONNECTED 	2

#define HID_PROTOCOL_BOOT	0x00
#define HID_PROTOCOL_REPORT	0x01

#define HID_HEADER_LEN		1

typedef struct _bt_hid_report_t
{
	bt_byte type;
	bt_byte id;
	bt_int  length;
} bt_hid_report_t;


typedef struct _bt_hid_session_t bt_hid_session_t;
typedef bt_hid_session_t* bt_hid_session_p;

typedef bt_bool (*bt_hid_get_report_callback_fp)(bt_byte type, bt_byte id, void* param);
typedef bt_bool (*bt_hid_set_report_callback_fp)(bt_byte type, bt_byte id, bt_byte_p buffer, bt_int len, void* param);
typedef void (*bt_hid_send_report_callback_fp)(bt_hid_session_p psess, bt_byte* data, bt_int len, void* param);
typedef void (*bt_hid_state_callback_fp)(bt_hid_session_p psess, void* param);

#include "cdbt/hid/hid_packet.h"

typedef struct _hid_pdu 
{
	struct _hid_pdu* next_pdu;
	bt_hid_packet_t packet;
} bt_hid_pdu_t;
typedef bt_hid_pdu_t* bt_hid_pdu_p;

struct _bt_hid_session_t 
{
	bt_byte state;
	bt_l2cap_channel_t* pch_ctl;
	bt_l2cap_channel_t* pch_int;
	bt_bool restore_connection;
	bt_int report_count;
	bt_hid_report_t* reports;
	bt_queue_element_t* queue;
	bt_hid_pdu_p pdu_cur;
	bt_l2cap_mgr_p l2cap_mgr;
	bt_bdaddr_t bdaddr_remote;
	bt_byte protocol;

	bt_hid_get_report_callback_fp get_report_cb;
	bt_hid_set_report_callback_fp set_report_cb;
	bt_hid_send_report_callback_fp send_cb;
	bt_hid_state_callback_fp state_cb;
	void* callback_param;
};


/**
 * \brief Initialize HID layer
 * \ingroup hid
 *
 * \details This function initializes the HID layer of the stack. It must be called prior to any other
 * HID function can be called.
 */
bt_bool bt_hid_init(void);

/**
 * \brief Listen for incoming connections
 * \ingroup hid
 *
 * \details This function enables incoming connections on the specified HID session. Changes in
 * the session state are reported through a callback function. 
 *
 * \param session HID session.
 * \param callback The callback function that is called when session state changes.
 * \param get_report_callback The callback function that is called when a remote device requests an INPUT report.
 * \param set_report_callback The callback function that is called when a remote device sends and OUTPUT report.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The callback function is not called in this case. 
 */
bt_bool bt_hid_listen(
	bt_hid_session_p session, 
	bt_hid_state_callback_fp callback,
	bt_hid_get_report_callback_fp get_report_callback,
	bt_hid_set_report_callback_fp set_report_callback);

/**
 * \brief Connect to a remote device
 * \ingroup hid
 *
 * \details This function establishes a HID connection with a remote device.
 * Changes in the session state are reported through a callback function.
 *
 * \param session HID session.
 * \param remote_addr Address of the remote device.
 * \param callback The callback function that is called when session state changes.
 * \param get_report_callback The callback function that is called when a remote device requests an INPUT report.
 * \param set_report_callback The callback function that is called when a remote device sends and OUTPUT report.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The callback function is not called in this case. 
 */
bt_bool bt_hid_connect(
	bt_hid_session_p session, bt_bdaddr_p remote_addr, 
	bt_hid_state_callback_fp callback,
	bt_hid_get_report_callback_fp get_report_callback,
	bt_hid_set_report_callback_fp set_report_callback);

/**
 * \brief Close connection
 * \ingroup hid
 *
 * \details This function closes a HID connection with a remote device.
 * Changes in the session state are reported through a callback function set when connection was created by calling bt_hid_connect.
 *
 * \param session HID session.
 */
bt_bool bt_hid_disconnect(bt_hid_session_p session);

/**
 * \brief Unplug device
 * \ingroup hid
 *
 * \details This function unplugs a virtually-cabled device
 * Changes in the session state are reported through a callback function set when connection was created by calling bt_hid_connect.
 *
 * \param session HID session.
 */
void bt_hid_unplug(bt_hid_session_p session);

/**
 * \brief Allocate HID session
 * \ingroup hid
 *
 * \details This function allocates a new HID session.
 *
 * \param l2cap_mgr The L2CAP manager on which the HID session is to be created.
 *
 * \return
 *         \li A pointer to the new HID session structure if the function succeeds.
 *         \li \c NULL otherwise.
 */
bt_hid_session_p bt_hid_allocate_session(bt_l2cap_mgr_p l2cap_mgr, void* callback_param);

/**
 * \brief Release HID session
 * \ingroup hid
 *
 * \details This function deallocated the specified HID session.
 * This function does not disconnect the session. It just frees the memory used by the bt_hid_session structure.
 * The session has to be disconnected by calling bt_hid_disconnect or bt_hid_unplug first.
 *
 * \param session The HID session to be deallocated.
 *
 */
void bt_hid_free_session(bt_hid_session_p session);

/**
 * \brief Add report definition to local HID device
 * \ingroup hid
 *
 * \details This function add a report definitions to the specified HID session.
 *
 * \param session The HID session which a report definition to be added to.
 * \param report The report definition to be added.
 *
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_hid_add_report(bt_hid_session_p session, bt_hid_report_t* report);


/**
 * \brief Send report
 * \ingroup hid
 *
 * \details This function sends a report to a remote device
 *
 * \param session The HID session.
 * \param channel The type of the channel (CONTROL or INTERRUPT) used to send the report.
 * \param report_type The type of the report (INPUT, OUTPUR, or FEATURE).
 * \param report_id The id of the report.
 * \param send_report_id The flag that specifies weather the report id is included in the data packet.
 * \param data The pointer to the report data.
 * \param len The length of the report data.
 * \param tran_type The type of the transaction (see definition of the HID_TRANTYPE constants).
 * \param callback The callback function that is called when sending the report has been completed.
 *
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The callback function is not called in this case.
 */
bt_bool bt_hid_send_report(
	bt_hid_session_p session, bt_byte channel, 
	bt_byte report_type, bt_byte report_id, bt_bool send_report_id,
	bt_byte_p data, bt_int len, bt_byte tran_type,
	bt_hid_send_report_callback_fp callback);

#define bt_hid_get_remote_address(session)	&session->pch_ctl->hci_conn->bdaddr_remote

bt_hci_conn_state_t* bt_hid_get_hci_connection(const bt_hid_session_t* session);

#include "cdbt/hid/hid_private.h"

#ifdef __cplusplus
}
#endif

#endif // __HID_H
