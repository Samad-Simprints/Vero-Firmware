/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTLOG_DEFINE_H_INCLUDED__
#define __BTLOG_DEFINE_H_INCLUDED__

#include "btlog.h"

#ifdef ENABLE_LOG

#if defined(BTLOG_TAG)
static const char __btlog_tag__[] = BTLOG_TAG;
#else
#define __btlog_tag__ 0
#endif

#define BTLOG(msg)                btlog_writeEntry(__btlog_tag__, msg)
#define BTLOG1(msg, arg)          btlog_formatEntry(__btlog_tag__, msg, arg)
#define BTLOG2(msg, arg, arg2)    btlog_formatEntry(__btlog_tag__, msg, arg, arg2)

#else  // ENABLE_LOG

#define BTLOG(msg)                ((void)0)
#define BTLOG1(msg, arg)          ((void)0)
#define BTLOG2(msg, arg, arg2)    ((void)0)

#endif


#endif // __BTLOG_DEFINE_H_INCLUDED__
