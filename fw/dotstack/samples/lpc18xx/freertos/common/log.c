/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "btlog.h"
#include "error.h"
#include "usbcdc.h"

/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "btlog.h"
#include "error.h"
#include "board.h"
#include "usbcdc.h"

// NsT #include "lpc18xx_clkpwr.h"
#include "lpc18xx_uart.h"
#include "lpc18xx_gpio.h"
// NsT #include "lpc18xx_pinsel.h"
#include "lpc18xx_gpdma.h"
#include "lpc18xx_scu.h"

#include <string.h>

// Contents of this file are only compiled if logging is enabled.
#ifdef ENABLE_LOG

// Mutex for accessing log from multiple tasks.
static xSemaphoreHandle mLogMutex;

#ifdef LOG_USE_USB

void btlog_pal_init(void)
{
	vSemaphoreCreateBinary(mLogMutex);
}


void btlog_pal_start(void)
{
}


void btlog_pal_lockBuffer(void)
{
	xSemaphoreTake(mLogMutex, portMAX_DELAY);
}


void btlog_pal_unlockBuffer(void)
{
	xSemaphoreGive(mLogMutex);
}


void btlog_pal_lockTransmission(void)
{
	taskENTER_CRITICAL();
}


void btlog_pal_unlockTransmission(void)
{
	taskEXIT_CRITICAL();
}

static void btlog_pal_transmit_callback(int len)
{
	if (len > 0)
	{
    	// Update btlog on partial tranfer progress
		btlog_onDataTransmitted(len);
	}
	else
	{
	  	char *buf;
		int len;

		// Zero length transfer indicates that USB has finished with previos
		// data buffer and is asking for more. Ask log if more data to be
		// transferred is available, and if it is, continue the
		// operation without bothering btlog with this completion.
		// Otherwise, tell btlog we are done, so it will wake us up the
		// next time there's data it wants to send.
		buf = btlog_getTransmitData(&len);
		if (NULL != buf)
	  		usbcdc_transmit(buf, len, btlog_pal_transmit_callback);
		else
			btlog_onDataTransmitted(0);
	}
}
								

void btlog_pal_transmit(void)
{
    btlog_pal_transmit_callback(0);
}

#else

// Maximum transfer length allowed by the DMA controller is 4095.
// However, we limit transfer length here because in cases when the whole log
// buffer is filled it will be freed faster using shorter transfers.
#define MAX_DMA_LEN        256

// Current DMA transfer length
static int              mDmaLen;

static void initUart(void);
static void startDmaTransfer(const char* data, int len);

void btlog_pal_init(void)
{
	vSemaphoreCreateBinary(mLogMutex);

	initUart();
}


void btlog_pal_start(void)
{
}


void btlog_pal_lockBuffer(void)
{
	xSemaphoreTake(mLogMutex, portMAX_DELAY);
}


void btlog_pal_unlockBuffer(void)
{
	xSemaphoreGive(mLogMutex);
}


void btlog_pal_lockTransmission(void)
{
	taskENTER_CRITICAL();
}


void btlog_pal_unlockTransmission(void)
{
	taskEXIT_CRITICAL();
}

void btlog_pal_transmit(void)
{
	const char* data;
	int len;

	data = btlog_getTransmitData(&len);
	if (len == 0)
	{
		btlog_onDataTransmitted(0);
	}
	else
	{
		startDmaTransfer(data, len);
	}
}

static void initUart(void)
{
	// Configure UART clock
//	CLKPWR_SetPCLKDiv(LOG_UART_PCLK, LOG_UART_PCLK_DIV);

	// Configure UART TX pin
	{
        	scu_pinmux(LOG_UART_TX_PORT, LOG_UART_TX_PIN, MD_PDN, LOG_UART_TX_FUNC);
#if 0
		PINSEL_CFG_Type pinsel;

		pinsel.Portnum = LOG_UART_TX_PORT;
		pinsel.Pinnum = LOG_UART_TX_PIN;
		pinsel.Funcnum = LOG_UART_TX_FUNC;
		pinsel.Pinmode = PINSEL_PINMODE_PULLUP;
		pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

		PINSEL_ConfigPin(&pinsel);
#endif
	}

	// Configure UART parameters
	{
		UART_CFG_Type uartCfg;
		UART_ConfigStructInit(&uartCfg);
		uartCfg.Baud_rate = LOG_UART_BAUD_RATE;
		uartCfg.Parity = UART_PARITY_NONE;
		uartCfg.Databits = UART_DATABIT_8;
		uartCfg.Stopbits = UART_STOPBIT_1;
		//NsT UART_Init((LPC_UART_TypeDef*)LOG_UART, &uartCfg);
                UART_Init((LPC_USARTn_Type*)LOG_UART, &uartCfg);
	}

	// Configure UART FIFO
	{
		UART_FIFO_CFG_Type fifoCfg;
		UART_FIFOConfigStructInit(&fifoCfg);
		//NsT fifoCfg.FIFO_DMAMode = ENABLE;
		//NsT UART_FIFOConfig((LPC_UART_TypeDef*)LOG_UART, &fifoCfg);
		UART_FIFOConfig((LPC_USARTn_Type*)LOG_UART, &fifoCfg);
	}

	// Enable UART transmitter.
	// NsT UART_TxCmd((LPC_UART_TypeDef*)LOG_UART, ENABLE);
	UART_TxCmd((LPC_USARTn_Type*)LOG_UART, ENABLE);

	// Enable DMA interrupt
	//NsT NVIC_SetPriority(DMA_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	//NsT NVIC_EnableIRQ(DMA_IRQn);

        UART_Send((LPC_USARTn_Type*)LOG_UART, "\r\nThis is data\r\n", 16, BLOCKING);
}

// This function must be called with interrupts disabled.
static void startDmaTransfer(const char* data, int len)
{
	// Limit transfer length.
	if (len > MAX_DMA_LEN)
		len = MAX_DMA_LEN;

	mDmaLen = len;

	// Configure DMA
	{
		GPDMA_Channel_CFG_Type init;

		memset(&init, 0, sizeof(init));
		init.ChannelNum = LOG_UART_DMA_CH_NUM;
		init.SrcMemAddr = (uint32_t)data;
		init.DstConn = LOG_UART_DST_CONN;
		//Nst init.TransferType = GPDMA_TRANSFERTYPE_M2P;
                init.TransferType = GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA;
		init.TransferSize = mDmaLen;
		GPDMA_Setup(&init);

		GPDMA_ChannelCmd(LOG_UART_DMA_CH_NUM, ENABLE);
	}
}

void GPDMA_IRQHandler(void)
{
	if (GPDMA_IntGetStatus(GPDMA_STAT_INTTC, LOG_UART_DMA_CH_NUM) == SET)
	{
		GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, LOG_UART_DMA_CH_NUM);
		GPDMA_ChannelCmd(LOG_UART_DMA_CH_NUM, DISABLE);

		btlog_onDataTransmitted(mDmaLen);

		btlog_pal_transmit();
	}
	else if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, LOG_UART_DMA_CH_NUM) == SET)
	{
		error_onFatalError();
	}

	portEND_SWITCHING_ISR(pdFALSE);
}

#endif

#endif // ENABLE_LOG
