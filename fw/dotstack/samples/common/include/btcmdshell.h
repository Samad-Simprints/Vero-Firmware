/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTCMDSHELL_H_INCLUDED__
#define __BTCMDSHELL_H_INCLUDED__

typedef void (*btcmdshell_CommandHandler)(const char* cmd, const char* params[], bt_byte paramsCount);

void btcmdshell_init(btcmdshell_CommandHandler handler);

void btcmdshell_execCommand(const char* cmd, const char* params);

void btcmdshell_write(const char* tag, const char* format, ...);

void btcmdshell_dumpMemory(const char* p, unsigned int len);

#endif // __BTCMDSHELL_H_INCLUDED__
