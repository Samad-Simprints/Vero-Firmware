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

#ifndef __OBEX_H
#define __OBEX_H

#ifdef __cplusplus
extern "C" {
#endif

#define OBEX_VERSION		0x10

#define OBEX_FINAL_BIT				0x80

#define OBEX_OPCODE_CONNECT			0x80
#define OBEX_OPCODE_DISCONNECT		0x81
#define OBEX_OPCODE_PUT				0x02
#define OBEX_OPCODE_GET				0x03
#define OBEX_OPCODE_SET_PATH		0x85
#define OBEX_OPCODE_ABORT			0xFF

#define OBEX_RESPONSE_CONTINUE				(0x10 | OBEX_FINAL_BIT) // (0x90) 100 Continue
#define OBEX_RESPONSE_SUCCESS				(0x20 | OBEX_FINAL_BIT) // (0xA0) 200 OK, Success
#define OBEX_RESPONSE_CREATED				(0x21 | OBEX_FINAL_BIT) // (0xA1) 201 Created
#define OBEX_RESPONSE_ACCEPTED				(0x22 | OBEX_FINAL_BIT) // (0xA2) 202 Accepted
#define OBEX_RESPONSE_NON_AUTH_INFO			(0x23 | OBEX_FINAL_BIT) // (0xA3) 203 Non-Authoritative Information
#define OBEX_RESPONSE_NO_CONTENT			(0x24 | OBEX_FINAL_BIT) // (0xA4) 204 No Content
#define OBEX_RESPONSE_RESET_CONTENT			(0x25 | OBEX_FINAL_BIT) // (0xA5) 205 Reset Content
#define OBEX_RESPONSE_PARTIAL_CONTENT		(0x26 | OBEX_FINAL_BIT) // (0xA6) 206 Partial Content
#define OBEX_RESPONSE_MULTIPLE_CHOICES		(0x30 | OBEX_FINAL_BIT) // (0xB0) 300 Multiple Choices
#define OBEX_RESPONSE_MOVED_PERMANENTLY		(0x31 | OBEX_FINAL_BIT) // (0xB1) 301 Moved Permanently
#define OBEX_RESPONSE_MOVED_TEMPORARILY		(0x32 | OBEX_FINAL_BIT) // (0xB2) 302 Moved temporarily
#define OBEX_RESPONSE_SEE_OTHER				(0x33 | OBEX_FINAL_BIT) // (0xB3) 303 See Other
#define OBEX_RESPONSE_NOT_MODIFIED			(0x34 | OBEX_FINAL_BIT) // (0xB4) 304 Not modified
#define OBEX_RESPONSE_USE_PROXY				(0x35 | OBEX_FINAL_BIT) // (0xB5) 305 Use Proxy
#define OBEX_RESPONSE_BAD_REQUEST			(0x40 | OBEX_FINAL_BIT) // (0xC0) 400 Bad Request - server couldn�t understand request
#define OBEX_RESPONSE_UNAUTHORIZED			(0x41 | OBEX_FINAL_BIT) // (0xC1) 401 Unauthorized
#define OBEX_RESPONSE_PAYMENT_REQUIRED		(0x42 | OBEX_FINAL_BIT) // (0xC2) 402 Payment required
#define OBEX_RESPONSE_FORBIDDEN				(0x43 | OBEX_FINAL_BIT) // (0xC3) 403 Forbidden - operation is understood but refused
#define OBEX_RESPONSE_NOT_FOUND				(0x44 | OBEX_FINAL_BIT) // (0xC4) 404 Not Found
#define OBEX_RESPONSE_METHOD_NOT_ALLOWED	(0x45 | OBEX_FINAL_BIT) // (0xC5) 405 Method not allowed
#define OBEX_RESPONSE_NOT_ACCEPTABLE		(0x46 | OBEX_FINAL_BIT) // (0xC6) 406 Not Acceptable
#define OBEX_RESPONSE_PROXY_AUTH_REQUIRED	(0x47 | OBEX_FINAL_BIT) // (0xC7) 407 Proxy Authentication required
#define OBEX_RESPONSE_TIMEOUT				(0x48 | OBEX_FINAL_BIT) // (0xC8) 408 Request Time Out
#define OBEX_RESPONSE_CONFLICT				(0x49 | OBEX_FINAL_BIT) // (0xC9) 409 Conflict
#define OBEX_RESPONSE_GONE					(0x4A | OBEX_FINAL_BIT) // (0xCA) 410 Gone
#define OBEX_RESPONSE_LENGTH_REQUIRED		(0x4B | OBEX_FINAL_BIT) // (0xCB) 411 Length Required
#define OBEX_RESPONSE_PRECONDITION_FAILED	(0x4C | OBEX_FINAL_BIT) // (0xCC) 412 Precondition failed
#define OBEX_RESPONSE_REQ_ENTITY_TOO_LARGE	(0x4D | OBEX_FINAL_BIT) // (0xCD) 413 Requested entity too large
#define OBEX_RESPONSE_REQ_URL_TOO_LARGE		(0x4E | OBEX_FINAL_BIT) // (0xCE) 414 Request URL too large
#define OBEX_RESPONSE_UNSUPPORTED_MEDIA		(0x4F | OBEX_FINAL_BIT) // (0xCF) 415 Unsupported media type
#define OBEX_RESPONSE_INTERNAL_SERVER_ERR	(0x50 | OBEX_FINAL_BIT) // (0xD0) 500 Internal Server Error
#define OBEX_RESPONSE_NOT_IMPLEMENTED		(0x51 | OBEX_FINAL_BIT) // (0xD1) 501 Not Implemented
#define OBEX_RESPONSE_BAD_GATEWAY			(0x52 | OBEX_FINAL_BIT) // (0xD2) 502 Bad Gateway
#define OBEX_RESPONSE_SERVICE_UNAVAILABLE	(0x53 | OBEX_FINAL_BIT) // (0xD3) 503 Service Unavailable
#define OBEX_RESPONSE_GATEWAY_TIMEOUT		(0x54 | OBEX_FINAL_BIT) // (0xD4) 504 Gateway Timeout
#define OBEX_RESPONSE_HTTP_VER_NOT_SUPP		(0x55 | OBEX_FINAL_BIT) // (0xD5) 505 HTTP version not supported
#define OBEX_RESPONSE_DATABASE_FULL			(0x60 | OBEX_FINAL_BIT) // (0xE0) - - - Database Full
#define OBEX_RESPONSE_DATABASE_LOCKED		(0x61 | OBEX_FINAL_BIT) // (0xE1) - - - Database Locked


#define OBEX_HEADER_TYPE_UNICODE	0x00
#define OBEX_HEADER_TYPE_BYTE_SEQ	0x40
#define OBEX_HEADER_TYPE_1_BYTE		0x80
#define OBEX_HEADER_TYPE_4_BYTE		0xC0


#define OBEX_HI_COUNT				0xC0 // Number of objects (used by Connect)
#define OBEX_HI_NAME				0x01 // name of the object (often a file name)
#define OBEX_HI_TYPE				0x42 // type of object - e.g. text, html, binary, manufacturer specific
#define OBEX_HI_LENGTH				0xC3 // the length of the object in bytes
#define OBEX_HI_TIME_ISO8601		0x44 // date/time stamp - ISO 8601 version - preferred
#define OBEX_HI_TIME_4BYTE			0xC4 // date/time stamp - 4 byte version (for compatibility only)
#define OBEX_HI_DESCRIPTION			0x05 // text description of the object
#define OBEX_HI_TARGET				0x46 // name of service that operation is targeted to
#define OBEX_HI_HTTP				0x47 // an HTTP 1.x header
#define OBEX_HI_BODY				0x48 // a chunk of the object body
#define OBEX_HI_END_OF_BODY			0x49 // the final chunk of the object body
#define OBEX_HI_WHO					0x4A // identifies the OBEX application, used to tell if talking to a peer
#define OBEX_HI_CONNECTION_ID		0xCB // an identifier used for OBEX connection multiplexing
#define OBEX_HI_APP_PARAMETERES		0x4C // extended application request & response information
#define OBEX_HI_AUTH_CHALLENGE		0x4D // authentication digest-challenge
#define OBEX_HI_AUTH_RESPONSE		0x4E // authentication digest-response
#define OBEX_HI_OBJECT_CLASS		0x4F // Object Class

#define OBEX_MIN_PACKET_LENGTH		0xFF

#define OBEX_PACKET_HEADER_LEN		3

#define OBEX_SETPATH_BACKUP_LEVEL	1
#define OBEX_SETPATH_DONOTCREATE	2

#define OBEX_MAX_RESPONSE_HEADERS	3

typedef enum _bt_obex_session_state_enum
{
	OBEX_SESSION_STATE_FREE = -1,
	OBEX_SESSION_STATE_DISCONNECTED = 0,
	OBEX_SESSION_STATE_RFCOMM_CONNECTED,
	OBEX_SESSION_STATE_CONNECTED
} bt_obex_session_state_enum;

typedef enum _bt_obex_session_phase_enum
{
	OBEX_SESSION_PHASE_IDLE,
	OBEX_SESSION_PHASE_WAITING_RESPONSE
} bt_obex_session_phase_enum;

typedef struct _obex_auth_challenge_s
{
	const bt_byte* nonce;
	bt_byte nonce_len;
	bt_byte options;
	const bt_byte* realm;
	bt_byte realm_len;

} obex_auth_challenge_t;

typedef enum _obex_session_event_enum
{
	OBEX_SESSION_NOTHING,
	OBEX_SESSION_RFCOMM_CONNECTION_STATE_CHANGED,
	OBEX_SESSION_RFCOMM_CONNECTION_FAILED,
	OBEX_SESSION_CONNECTION_STATE_CHANGED,
	OBEX_SESSION_PUT_COMPLETED,
	OBEX_SESSION_GET_COMPLETED,
	OBEX_SESSION_ABORT_COMPLETED,
	OBEX_SESSION_SETPATH_COMPLETED,
	OBEX_SESSION_CONNECT_ATTEMPT,

	OBEX_SESSION_CONNECT_REQUESTED,
	OBEX_SESSION_PUT_REQUESTED,
	OBEX_SESSION_GET_REQUESTED,
	OBEX_SESSION_SETPATH_REQUESTED,
	OBEX_SESSION_ABORT_REQUESTED,

	OBEX_SESSION_AUTHENTICATION_REQUESTED
} obex_session_event_enum;

struct _bt_obex_session;

typedef void (*bt_obex_session_callback_pf)(struct _bt_obex_session* session, obex_session_event_enum event, void* what_param, void* what_param2, void* callback_param);

typedef struct _bt_obex_header
{
	bt_byte hi;
	bt_uint hv_length;
	union
	{
		bt_byte* ustr; // pointer to compressed (copied with _compact_buffer) header data
		bt_byte* bseq; // pointer to compressed (copied with _compact_buffer) header data
		bt_byte b;
		bt_ulong ul;
	} hv;
} bt_obex_header;

typedef struct _bt_obex_session
{
	bt_obex_session_state_enum state;
	bt_obex_session_phase_enum phase;
	bt_bdaddr_t peer_bdaddr;
	bt_rfcomm_dlc_t* rfcomm_dlc;
	bt_uint remote_max_packet_len;

	bt_obex_session_callback_pf session_callback;
	void* session_callback_param;

	bt_byte cur_opcode;
	bt_uint cur_header_count;
	bt_obex_header* cur_headers;

	bt_byte rx_packet_type;
	bt_uint rx_packet_len;
	bt_byte* rx_packet_data;
	bt_uint rx_packet_remaining_len;
	bt_byte rx_leftover[OBEX_PACKET_HEADER_LEN - 1];
	bt_uint rx_leftover_len;
	bt_int rx_packet_read_pos;
	bt_byte* rx_tmp_buffer;

	bt_byte* tx_packet;

} bt_obex_session;

bt_bool bt_obex_init(void);

bt_obex_session* bt_obex_allocate_session(void);

bt_bool bt_obex_connect(
	bt_obex_session* session, bt_bdaddr_t* pbdaddr_remote, bt_byte server_channel,
	bt_uint header_count, bt_obex_header* headers,
	bt_obex_session_callback_pf callback, void* param);

bt_bool bt_obex_listen(bt_obex_session* session, bt_byte server_channel, bt_obex_session_callback_pf callback, void* param);

bt_bool bt_obex_disconnect(bt_obex_session* session, bt_uint header_count, bt_obex_header* headers);

bt_bool bt_obex_send_request(bt_obex_session* session, bt_byte opcode, bt_byte flags, bt_uint header_count, bt_obex_header* headers);

bt_bool bt_obex_send_response(bt_obex_session* session, bt_byte opcode, bt_byte response_code, bt_byte flags, bt_uint header_count, bt_obex_header* headers);

bt_bool bt_obex_abort(bt_obex_session* session, bt_uint header_count, bt_obex_header* headers);

bt_bool bt_obex_setpath(bt_obex_session* session, bt_byte* path, bt_uint len, bt_byte flags);

bt_bool bt_obex_get_first_header(bt_obex_session* session, bt_obex_header* hdr);

bt_bool bt_obex_get_next_header(bt_obex_session* session, bt_obex_header* hdr);

bt_uint bt_obex_get_next_connection_id(void);

bt_int bt_obex_get_frame_length(bt_obex_session* session);

#include "cdbt/obex/obex_private.h"

#ifdef __cplusplus
}
#endif

#endif // __OBEX_H
