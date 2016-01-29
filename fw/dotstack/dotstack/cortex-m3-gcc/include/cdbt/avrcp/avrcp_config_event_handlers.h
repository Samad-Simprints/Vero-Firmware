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

#ifndef __AVRCP_CONFIG_EVENT_HANDLERS_H
#define __AVRCP_CONFIG_EVENT_HANDLERS_H

#ifdef AVRCP_ENABLE_CONTROLLER
#define AVRCP_COMMAND_SENT_HANDLER    _bt_avrcp_handle_command_sent
#define AVRCP_RESPONSE_HANDLER        _bt_avrcp_handle_response
#else
#define AVRCP_COMMAND_SENT_HANDLER    NULL
#define AVRCP_RESPONSE_HANDLER        NULL
#endif

#ifdef AVRCP_ENABLE_TARGET
#define AVRCP_RESPONSE_SENT_HANDLER   _bt_avrcp_handle_response_sent
#define AVRCP_COMMAND_HANDLER         _bt_avrcp_handle_command
#else
#define AVRCP_RESPONSE_SENT_HANDLER   NULL
#define AVRCP_COMMAND_HANDLER         NULL
#endif

#endif // __AVRCP_CONFIG_EVENT_HANDLERS_H
