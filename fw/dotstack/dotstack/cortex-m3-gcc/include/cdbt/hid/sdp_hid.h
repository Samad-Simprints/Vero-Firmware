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

#ifndef __SDP_HID_H
#define __SDP_HID_H

#define BTHID_USAGEPAGE_CD			0xFF00
#define BTHID_USAGE_PEN				1
#define BTHID_USAGE_PEN_CFG			2

#define BTHID_DATA_REPORT_ID		0x10

#define BTHID_POS_REPORT_ID			0x10
#define BTHID_CFG_REPORT_ID			0x11
#define BTHID_CFG_WORD_REPORT_ID	0x12
#define BTHID_KEYBOARD_REPORT_ID	0x13

#define BTHID_DATA_REPORT_SIZE		8
#define BTHID_DATA_REPORT_COUNT		28

#define BTHID_POS_REPORT_SIZE		16
#define BTHID_POS_REPORT_COUNT		2
#define BTHID_CFG_REPORT_SIZE		16
#define BTHID_CFG_REPORT_COUNT		1
#define BTHID_CFG_WORD_REPORT_SIZE	16
#define BTHID_CFG_WORD_REPORT_COUNT	68
//#define BTHID_CFG_REPORT_COUNT	0x768

#define BTHID_DATA_REPORT_BYTE_COUNT	(BTHID_DATA_REPORT_COUNT * (BTHID_DATA_REPORT_SIZE >> 3))

#define BTHID_KEYBOARD_REPORT_BYTE_COUNT	8


#endif // __SDP_HID_H
