/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTLOG_H_INCLUDED__
#define __BTLOG_H_INCLUDED__

#ifdef ENABLE_LOG

#if defined(__ICC430__) || defined(STM32L15XXB_128K) || defined(EFM32G890F128)
	#define BTLOG_BUFFER_SIZE         (1024) // must be a power of 2
#elif  defined(EFM32TG840F32)
	#define BTLOG_BUFFER_SIZE         (256) // must be a power of 2
#else
	#define BTLOG_BUFFER_SIZE         (2 * 1024) // must be a power of 2
#endif

#define BTLOG_MAX_MESSAGE_LEN     (128)

#define BTLOG_OEM_BUFFER_SIZE     (128)


void btlog_init(void);
void btlog_start(void);
void btlog_writeEntry(const char* tag, const char* message);
void btlog_formatEntry(const char* tag, const char* format, ...);

char* btlog_getTransmitData(int* transmitLen);
void btlog_onDataTransmitted(int transmitLen);

void btlog_pal_init(void);
void btlog_pal_start(void);
void btlog_pal_transmit(void);
void btlog_pal_lockBuffer(void);
void btlog_pal_unlockBuffer(void);
void btlog_pal_lockTransmission(void);
void btlog_pal_unlockTransmission(void);

#else  // ENABLE_LOG

#define btlog_init()   ((void)0)
#define btlog_start()   ((void)0)

#endif


#endif // __BTLOG_H_INCLUDED__
