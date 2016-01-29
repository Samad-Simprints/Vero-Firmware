#include "config.h"
#include "FreeRTOS.h"
#include "task.h"

#include "btport.h"
#include "bttask.h"
#include "error.h"
#include "board.h"
//# NsT include "lpc18xx_clkpwr.h"
#include "lpc18xx_uart.h"
#include "lpc18xx_gpio.h"
// NsT #include "lpc18xx_pinsel.h"
#include "btapp.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_hcitr.h"
#include "cdbt/hci/hci.h"
#include "cdbt/hci/hcitr_tih4.h"

#include "cdbt/extra/ti/ti.h"

#include <assert.h>

#if BT_CONTROLLER == BT_CONTROLLER_CC2560 || BT_CONTROLLER == BT_CONTROLLER_CC2564 || BT_CONTROLLER == BT_CONTROLLER_CC2564B

// TI init script selection
#if BT_CONTROLLER == BT_CONTROLLER_CC2560
	#define INIT_SCRIPT               btx_ti_get_script__CC2560_ServicePack()
#elif BT_CONTROLLER == BT_CONTROLLER_CC2564
	#define INIT_SCRIPT               btx_ti_get_script__CC2564_ServicePack()

	// Additional script is required for BLE
	#ifdef BTAPP_USE_BLE
		#define INIT_SCRIPT_BLE       btx_ti_get_script__CC2564_BLE_Init()
	#endif
#elif BT_CONTROLLER == BT_CONTROLLER_CC2564B
	#define INIT_SCRIPT               btx_ti_get_script__CC2564B_ServicePack()

	// Additional script is required for BLE
	#ifdef BTAPP_USE_BLE
		#define INIT_SCRIPT_BLE       btx_ti_get_script__CC2564B_BLE_Init()
	#endif
#else
	#error BT_CONTROLLER is undefined or not supported
#endif

// Receive FIFO buffer size
#define FIFO_SIZE                32

#define BTLOG_TAG "BTPORT"
#include "btlog_define.h"

static bt_oem_recv_callback_fp  mRxCallback;
static bt_byte*                 mRxBuffer;
static bt_uint                  mRxLen;
static bt_uint                  mRxIndex;
static bt_byte                  mRxBufferBusy;
static bt_byte                  mRxFifo[FIFO_SIZE];
static bt_uint                  mRxFifoHead;
static bt_uint                  mRxFifoTail;
static bt_byte                  mStalled;

static bt_oem_send_callback_fp  mTxCallback;
static const bt_byte*           mTxBuffer;
static bt_uint                  mTxLen;
static bt_uint                  mTxIndex;

static bt_byte                  mLoadingInitScript;

static bttask_StartCallback     mStartCallback;

static btx_ti_exec_script_buffer_t mExecScriptBuffer;

static void configureUart(uint32_t baudRate);
static void configureOutputPin(uint8_t port, uint8_t pin,  uint8_t func);
static void configureInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t mode);

static void hcillPowerCallback(bt_hcitr_tih4_power_event_e event);
static void setBaudRateCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void execScriptCallback(bt_bool success, btx_ti_exec_script_buffer_t* data, void* callback_param);
static void execBleScriptCallback(bt_bool success, btx_ti_exec_script_buffer_t* data, void* callback_param);
static void hciResetCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void enableFastClockCrystalCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void enableLowPowerScanCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void enableDeepSleepCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void hciStopCallback(void* param);

void bttask_pal_initBluetoothPort(void)
{
	mRxBuffer = NULL;
	mRxBufferBusy = 0;
	mRxFifoHead = mRxFifoTail = 0;
	mStalled = 0;
	mTxBuffer = NULL;
	
    // Configure BT Reset pin for output and set it low to activate reset.
    GPIO_LOW(BT_RESET_PORT, BT_RESET_PIN);
    GPIO(BT_RESET_PORT)->FIODIR |= 1 << BT_RESET_PIN;

    configureUart(BT_UART_INITIAL_BAUD_RATE);

    // Set up 32.768Khz output on P0.6 - MAT2.0
    LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM2;  // enable Timer2 power
    LPC_SC->PCLKSEL1 &= ~0x00003000;    // Bit 12,13 are for TIM2
    LPC_SC->PCLKSEL1 |= 0x00001000;
    LPC_PINCON->PINSEL0 &= ~0x00003000;
    LPC_PINCON->PINSEL0 |= 0x00003000;  // MAT2.0 is P0.6
    LPC_TIM2->CTCR = 0x0000;            // timer mode
    LPC_TIM2->MR0  = 1525;              // div 100Mhz by 3052 for 32.768Khz
    LPC_TIM2->MCR  = 0x0002;            // reset on match MAT2.0
    LPC_TIM2->EMR  = 0x0030;            // toggle MAT2.0 on match
    LPC_TIM2->PR   = 0x0000;            // no prescale, tick every pclk
    LPC_TIM2->TCR  = 0x0001;            // enable the timer
	
	
    GPIO(1)->FIODIR |= 1 << 29;
}

void bttask_pal_handleWakeupSignal(void)
{
}

void bttask_pal_startBluetoothPort(bttask_StartCallback callback)
{
	vTaskDelay(60);

	// Take BT controller out of reset
	GPIO_HIGH(BT_RESET_PORT, BT_RESET_PIN);
	
	// Enable only the line status interrupt.
	// Receive and transmit interrupts are enabled on as needed basis.
	UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RLS, ENABLE);

	UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
	
	// Enable UART transmitter.
	UART_TxCmd((LPC_UART_TypeDef*)BT_UART, ENABLE);

	// Remember callback to be called when port startup is complete
	mStartCallback = callback;

	btport_setLoadingInitScript(1);

	bt_hcitr_tih4_init(&hcillPowerCallback);
	bt_hcitr_tih4_start();
	
	bt_hci_init();
	bt_hci_start_no_init();

	// Configure BT controller for working baud rate
	btx_ti_set_uart_baud_rate(BT_UART_WORKING_BAUD_RATE, setBaudRateCallback);
}

void bttask_pal_restartBluetoothPort(bttask_StartCallback callback)
{
}

static void setBaudRateCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
	if (status == 0)
	{
		// Now the BT controller is configured for the working baud rate.
		// Configure our UART accordingly.
		btport_configureForHighSpeed();
		
		// Execute the init script.
		btx_ti_exec_script(
			INIT_SCRIPT,
			&mExecScriptBuffer,
			&execScriptCallback,
			NULL);
	}
	else
	{
		error_onFatalError();
	}
}

static void execScriptCallback(bt_bool success, btx_ti_exec_script_buffer_t* data, void* callback_param)
{
	BTLOG1("Script completed: %d", success);
	
	if (success)
	{
#ifdef BTAPP_USE_BLE
		btx_ti_exec_script(
			INIT_SCRIPT_BLE,
			&mExecScriptBuffer,
			&execBleScriptCallback,
			NULL);
#else
		execBleScriptCallback(BT_TRUE, NULL, NULL);
#endif
	}
	else
	{
		error_onFatalError();
	}
}

static void execBleScriptCallback(bt_bool success, btx_ti_exec_script_buffer_t* data, void* callback_param)
{
	BTLOG1("Script completed: %d", success);
	
	if (success)
	{
		bt_hci_command_t* cmd = bt_hci_alloc_command(HCI_RESET, &hciResetCallback);
		bt_hci_send_cmd(cmd);
	}
	else
	{
		error_onFatalError();
	}
}

static void hciResetCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
	if (status == 0)
	{
		btx_ti_enable_fast_clock_crystal(&enableFastClockCrystalCallback);
	}
	else
	{
		error_onFatalError();
	}
}

static void enableFastClockCrystalCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
	if (status == 0)
	{
		btx_ti_enable_low_power_scan(BT_TRUE, &enableLowPowerScanCallback);
	}
	else
	{
		error_onFatalError();
	}
}

static void enableLowPowerScanCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
	if (status == 0)
	{
//#ifdef DISABLE_DEEP_SLEEP
		enableDeepSleepCallback(0, NULL, NULL);
//#else
//		btx_ti_enable_deep_sleep(BT_TRUE, &enableDeepSleepCallback);
//#endif
	}
	else
	{
		error_onFatalError();
	}
}

static void enableDeepSleepCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
	if (status == 0)
	{
		bt_hci_stop(&hciStopCallback, NULL);
	}
	else
	{
		error_onFatalError();
	}
}


static void hciStopCallback(void* param)
{
	taskENTER_CRITICAL();
	{
		mRxBuffer = NULL;
		mTxBuffer = NULL;
		mRxCallback = NULL;
		mTxCallback = NULL;
	}
	taskEXIT_CRITICAL();
	
	bt_hcitr_tih4_reset();
	btport_setLoadingInitScript(0);
	
	mStartCallback();
}

static void configureUart(uint32_t baudRate)
{
	// Configure UART clock
	CLKPWR_SetPCLKDiv(BT_UART_PCLK, BT_UART_PCLK_DIV);
	
	// Configure UART pins
	configureOutputPin(BT_UART_TX_PORT, BT_UART_TX_PIN,  BT_UART_TX_FUNC);
	configureInputPin(BT_UART_RX_PORT, BT_UART_RX_PIN,  BT_UART_RX_FUNC, PINSEL_PINMODE_PULLUP);
	configureOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN,  BT_UART_RTS_FUNC);
	configureInputPin(BT_UART_CTS_PORT, BT_UART_CTS_PIN,  BT_UART_CTS_FUNC, PINSEL_PINMODE_PULLUP);
	
	// Configure UART parameters
	{
		UART_CFG_Type uartCfg;
		UART_ConfigStructInit(&uartCfg);
		uartCfg.Baud_rate = baudRate;
		uartCfg.Parity = UART_PARITY_NONE;
		uartCfg.Databits = UART_DATABIT_8;
		uartCfg.Stopbits = UART_STOPBIT_1;
		UART_Init((LPC_UART_TypeDef*)BT_UART, &uartCfg);
	}
	
	// Configure UART FIFO
	{
		UART_FIFO_CFG_Type fifoCfg;
		UART_FIFOConfigStructInit(&fifoCfg);
		UART_FIFOConfig((LPC_UART_TypeDef*)BT_UART, &fifoCfg);
	}
	
	// Enable auto RTS/CTS
	UART_FullModemConfigMode(BT_UART, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
	UART_FullModemConfigMode(BT_UART, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
	
	NVIC_SetPriority(UART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);  // TODO: make it configurable
	NVIC_EnableIRQ(UART1_IRQn);
}


void btport_configureForHighSpeed(void )
{
	configureUart(BT_UART_WORKING_BAUD_RATE);

	// Enable only the line status interrupt.
	// Receive and transmit interrupts are enabled on as needed basis.
	UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RLS, ENABLE);

	UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
	
	// Enable UART transmitter.
	UART_TxCmd((LPC_UART_TypeDef*)BT_UART, ENABLE);
}


void bttask_pal_handleRxSignal(void)
{
	int avail, tail, head;
	int complete = 0;

	// Get available bytes in the receive fifo
	taskENTER_CRITICAL();
	{
		tail = mRxFifoTail;
		head = mRxFifoHead;
		avail = tail - head;

		// Mark the receive buffer as busy so that ISR will not
		// try to write data into it while we are working with it
		// on the task level.
		mRxBufferBusy = 1;
	}
	taskEXIT_CRITICAL();

	if (avail != 0)
	{
		int copyLen = mRxLen - mRxIndex;
		if (copyLen > avail)
			copyLen = avail;
		memcpy(mRxBuffer + mRxIndex, mRxFifo + head, copyLen);
		head += copyLen;
		mRxIndex += copyLen;
	}

	if (mRxIndex == mRxLen)
	{
		complete = 1;
		mRxBuffer = NULL;
	}

	taskENTER_CRITICAL();
	{
		mRxFifoHead = head;
		tail = mRxFifoTail;
		if (head == tail)
		{
			mRxFifoHead = mRxFifoTail = 0;
		}

		// Release the receive buffer so that ISR can write directly into it.
		mRxBufferBusy = 0;
	}
	taskEXIT_CRITICAL();

	if (mStalled)
	{
		taskENTER_CRITICAL();
		{
			mStalled = BT_FALSE;
		}
		taskEXIT_CRITICAL();

		UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
	}

	if (complete)
	{
		mRxCallback(mRxLen);
	}
}


void bttask_pal_handleTxSignal(void)
{
	int complete = 0;

	taskENTER_CRITICAL();
	{
		if (mTxBuffer != NULL && mTxIndex == mTxLen)
		{
			mTxBuffer = NULL;
			complete = 1;
		}
	}
	taskEXIT_CRITICAL();

	if (complete)
		mTxCallback();
}

void btport_suspendRx(void)
{
	taskENTER_CRITICAL();
	{
		// Configure BT RTS pin as GPIO and deassert it
		GPIO_HIGH(BT_UART_RTS_PORT, BT_UART_RTS_PIN);
		configureOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN, PINSEL_FUNC_0);
	}
	taskEXIT_CRITICAL();
}


void btport_resumeRx(void)
{
	taskENTER_CRITICAL();
	{
		// Configure BT RTS pin as AF
		GPIO_LOW(BT_UART_RTS_PORT, BT_UART_RTS_PIN);
		configureOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN, BT_UART_RTS_FUNC);
	}
	taskEXIT_CRITICAL();
}


void btport_setLoadingInitScript(int state)
{
	mLoadingInitScript = state;
}


void bt_oem_send(const bt_byte* buffer, bt_uint len, bt_oem_send_callback_fp callback)
{
	assert(mTxBuffer == NULL);
	assert(len != 0 && buffer != 0 && callback != 0);

	taskENTER_CRITICAL();
	{
		mTxBuffer = buffer;
		mTxLen = len;
		mTxIndex = 0;
		mTxCallback = callback;

		BT_UART->THR = *mTxBuffer++;
		mTxIndex++;
		UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_THRE, ENABLE);
	}
	taskEXIT_CRITICAL();
}


void bt_oem_recv(bt_byte* buffer, bt_uint len, bt_oem_recv_callback_fp callback)
{
	assert(mRxBuffer == 0);
	assert(len != 0 && buffer != 0 && callback != 0);

	taskENTER_CRITICAL();
	{
		mRxBuffer = buffer;
		mRxIndex = 0;
		mRxLen = len;
		mRxCallback = callback;
	}
	taskEXIT_CRITICAL();
	
	bttask_setSignal(BTTASK_SIG_RX);
}


void UART1_IRQHandler(void)
{
	signed portBASE_TYPE higherPriorityTaskWoken = pdFALSE;
	uint32_t intId = BT_UART->IIR & UART_IIR_INTID_MASK;  // interrupt source
	
	switch (intId)
	{
		case UART_IIR_INTID_RLS:
			assert(0);
			break;
	
		case UART_IIR_INTID_THRE:
			if (mTxIndex == mTxLen)
			{
				UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_THRE, DISABLE);
				bttask_setSignalFromISR(BTTASK_SIG_TX, &higherPriorityTaskWoken);
			}
			else
			{
				uint32_t fifoCount = UART_TX_FIFO_SIZE;
				while (fifoCount && (mTxIndex < mTxLen))// && (BT_UART->LSR & UART_LSR_THRE))
				{
					BT_UART->THR = *mTxBuffer++;
					mTxIndex++;
					fifoCount--;
				}
			}
			
			break;
			
		case UART_IIR_INTID_RDA:
		{
			bt_byte setSignal = BT_FALSE;
			int free;
			int head, tail;
			
			while (BT_UART->LSR & UART_LSR_RDR)
			{
				head = mRxFifoHead;
				tail = mRxFifoTail;
				if (head == tail && !mRxBufferBusy && mRxBuffer != NULL && mRxIndex < mRxLen)
				{
					mRxBuffer[mRxIndex++] = BT_UART->RBR;
					if (mRxIndex == mRxLen)
					{
						setSignal = BT_TRUE;
					}
				}
				else
				{
					free = FIFO_SIZE - tail;
					if (free != 0)
					{
						mRxFifo[tail++] = BT_UART->RBR;
						mRxFifoTail = tail;
						setSignal = BT_TRUE;
					}
					else
					{
						mStalled = BT_TRUE;
						setSignal = BT_TRUE;
						UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, DISABLE);
						break;
					}
				}
			}

			if (setSignal)
				bttask_setSignalFromISR(BTTASK_SIG_RX, &higherPriorityTaskWoken);
				
			break;
		}
			
		default:
			break;
	}
	
	if (mLoadingInitScript)
		LED1_TOGGLE();		
	
	portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

static void configureOutputPin(uint8_t port, uint8_t pin,  uint8_t func)
{
	PINSEL_CFG_Type pinsel;
	
	pinsel.Portnum = port;
	pinsel.Pinnum = pin;
	pinsel.Funcnum = func;
	pinsel.Pinmode = PINSEL_PINMODE_PULLUP;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	PINSEL_ConfigPin(&pinsel);
}


static void configureInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t mode)
{
	PINSEL_CFG_Type pinsel;
	
	pinsel.Portnum = port;
	pinsel.Pinnum = pin;
	pinsel.Funcnum = func;
	pinsel.Pinmode = mode;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	PINSEL_ConfigPin(&pinsel);
}

static void hcillPowerCallback(bt_hcitr_tih4_power_event_e event)
{
	BTLOG1("HCILL power event: %d", event);
}

#endif
