#include "config.h"
#include "FreeRTOS.h"
#include "task.h"

#include "btport.h"
#include "bttask.h"
#include "error.h"
#include "board.h"
// NST #include "lpc18xx_clkpwr.h"
#include "lpc18xx_cgu.h"
#include "lpc18xx_uart.h"
#include "lpc18xx_gpio.h"
// NsT #include "lpc18xx_pinsel.h"
#include "lpc18xx_scu.h"
#include "btapp.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_hcitr.h"
#include "cdbt/bt/bt_timer.h"
#include "cdbt/hci/hci.h"
#include "cdbt/hci/hcitr_uart.h"
#include "cdbt/extra/csr/csr.h"

#include "cdbt/extra/ti/ti.h"

#include <assert.h>

#if BT_CONTROLLER == BT_CONTROLLER_CSR8810 || BT_CONTROLLER == BT_CONTROLLER_CSR8811 || BT_CONTROLLER == BT_CONTROLLER_CSR8811A06 || BT_CONTROLLER == BT_CONTROLLER_CSR8811A08

// Receive FIFO buffer size
#define FIFO_SIZE                32

// Interval between auto-baud packets (ms)
#define AUTO_BAUD_INTERVAL       70

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

// Persistent Store (PS) values for BC7 initialization.
static const bt_uint BC7_PS_VALUES[] =
{
	// Set reference clock frequency to 26 MHz.
	SET_PS_VALUE_UINT16(PSKEY_ANA_FREQ, 26000),
	
	// Turn VM off as it has no use fo us.
	SET_PS_VALUE_UINT16(PSKEY_VM_DISABLE, 1),
	
	// Do not consider levels on any PIO pins when deciding whether deep sleep is possible.
	SET_PS_VALUE_UINT32(PSKEY_PIO_DEEP_SLEEP_EITHER_LEVEL, 0xFFFFFFFF),
	
	// Use extrenal clock for deep sleep.
	// SET_PS_VALUE_UINT16(PSKEY_DEEP_SLEEP_USE_EXTERNAL_CLOCK, 1),

	// Set deep sleep state to:
	//    1 - The controller will go to deep sleep whenever possible.
	//    3 - The controller will go to deep sleep whenever possible
	//        and also it will assume that the external slow clock is
	//        at least 20 ppm accurate. This will result in faster wakeup.
	// For now it is set to 1.
	//SET_PS_VALUE_UINT16(PSKEY_DEEP_SLEEP_STATE, 1),
	
	// Clear RTS (i.e. set it high) when the controller goes to deep sleep.
	//SET_PS_KEY_UINT16(PSKEY_DEEP_SLEEP_CLEAR_RTS, 1),
		
	// Disable waking up on active level on the CTS line.
	//SET_PS_KEY_UINT16(PSKEY_DEEP_SLEEP_WAKE_CTS, 0),
	
	// Configure controller to indicate on PIO2 when the exrnal fast clock is 
	// required. Note, this cannot be used to monitor when the controller goes
	// to deep sleep. Sometimes the clock is not required but the controller
	// is awake. More accurate signal to monitor deep sleep is the RTS pin
	// which was configured earlier.
	SET_PS_VALUE_UINT16(PSKEY_CLOCK_REQUEST_ENABLE, 2),

	// UART baud rate.
	SET_PS_VALUE_UINT32(PSKEY_UART_BITRATE, BT_UART_WORKING_BAUD_RATE),
	
	// Set host interface to H4.
	SET_PS_VALUE_UINT16(PSKEY_HOST_INTERFACE, 3),  // H4
	
	// Configure UART parameters for H4 interface.
	SET_PS_VALUE_UINT16(PSKEY_UART_CONFIG_H4, 0x08A8),

	// Disable initial HCI_Command_Status event.
	SET_PS_VALUE_UINT16(PSKEY_HCI_NOP_DISABLE, 1),

	// Reset timeout on break level on controller's UART Rx line.
	SET_PS_VALUE_UINT32(PSKEY_HOSTIO_UART_RESET_TIMEOUT, 5000),

	// Change BT address
//	SET_PS_VALUE_BDADDR(PSKEY_BDADDR, 0x1234, 0x56789abc), 

	0x0000 // End of list
};

static const btx_csr_get_script_fp _initScripts[] =
{
#if BT_CONTROLLER == BT_CONTROLLER_CSR8810
	&btx_csr_get_script__PB_90_REV6,
#elif BT_CONTROLLER == BT_CONTROLLER_CSR8811
	// CSR8811 A06
	&btx_csr_get_script__PB_101_CSR8811_CSP28_UART,

	// CSR8811 A08
	&btx_csr_get_script__PB_109_CSR8811_REV16
#elif BT_CONTROLLER == BT_CONTROLLER_CSR8811A06
	// CSR8811 A06
	&btx_csr_get_script__PB_101_CSR8811_CSP28_UART
#elif BT_CONTROLLER == BT_CONTROLLER_CSR8811A08
	// CSR8811 A08
	&btx_csr_get_script__PB_109_CSR8811_REV16
#endif
};

// Work buffers for btx_csr functions.
static union
{
	btx_csr_autobaud_buffer_t btx_csr_autobaud;
	btx_csr_exec_script_buffer_t btx_csr_exec_script;
	btx_csr_set_ps_vars_buffer_t btx_csr_set_ps_vars;
} mWorkBuffers;

static void configureUart(uint32_t baudRate);
static void configureOutputPin(uint8_t port, uint8_t pin,  uint8_t func);
static void configureInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t mode);
static void configureGPIOOutputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t gpio_port, uint8_t gpio_pin);
static void configureGPIOInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t gpio_port, uint8_t gpio_pin);

static void selHostInterfaceCallback(bt_bool success, btx_csr_autobaud_buffer_t* buffer);
static void execScriptCallback(bt_bool success, btx_csr_exec_script_buffer_t* buffer);
static void setPsVarsCallback(bt_bool success, btx_csr_set_ps_vars_buffer_t* buffer);
static void timerCallback(void);
static void warmResetCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt);
static void hciStopCallback(void* param);

void id_pin(int port, int id)
{
  int i;
  for (i = 0; i < id; i++)
  {
    GPIO_HIGH(port, id);
    GPIO_LOW(port, id);
  }
}

void bttask_pal_initBluetoothPort(void)
{
    mRxBuffer = NULL;
    mRxBufferBusy = 0;
    mRxFifoHead = mRxFifoTail = 0;
    mStalled = 0;
    mTxBuffer = NULL;

    CGU_ConfigPWR(CGU_PERIPHERAL_GPIO, ENABLE);
    CGU_ConfigPWR(CGU_PERIPHERAL_UART0, ENABLE);
    CGU_ConfigPWR(CGU_PERIPHERAL_UART1, ENABLE);
    CGU_EnableEntity(CGU_BASE_UART0, ENABLE);
    CGU_EnableEntity(CGU_BASE_UART1, ENABLE);

    // Configure BT Reset pin for output and set it low to activate reset.
#if 0
    GPIO_LOW(BT_RESET_PORT, BT_RESET_PIN);
    GPIO(BT_RESET_PORT)->FIODIR |= 1 << BT_RESET_PIN;
#else
    configureGPIOOutputPin(BT_RESET_PORT, BT_RESET_PIN,  BT_RESET_FUNC, BT_RESET_GPIO_PORT, BT_RESET_GPIO_PIN);
    GPIO_LOW(BT_RESET_GPIO_PORT, BT_RESET_GPIO_PIN);
#endif
    configureUart(BT_UART_INITIAL_BAUD_RATE);

#if 0
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
#endif	
#if 0	
    GPIO(LED1_PORT)->FIODIR |= 1 << LED1_PIN;
#else
    configureGPIOOutputPin(LED1_PORT, LED1_PIN,  LED1_FUNC, LED1_GPIO_PORT, LED1_GPIO_PIN);
    configureGPIOOutputPin(LED2_PORT, LED2_PIN,  LED2_FUNC, LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_LOW(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_HIGH(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_LOW(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_HIGH(LED2_GPIO_PORT, LED2_GPIO_PIN);

    LED1_ON();
    LED1_OFF();
    LED1_TOGGLE();
#if 0
    configureGPIOOutputPin(BT_UART_TX_PORT, BT_UART_TX_PIN, 0, 1, 6);
    configureGPIOOutputPin(BT_UART_RX_PORT, BT_UART_RX_PIN, 0, 1, 7);
    configureGPIOOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN, 0, 1, 2);
    configureGPIOOutputPin(BT_UART_CTS_PORT, BT_UART_CTS_PIN, 0, 1, 4);

    id_pin(1, 6);
    id_pin(1, 7);
    id_pin(1, 2);
    id_pin(1, 4);
#endif
#if 0
    GPIO_HIGH(1, 6);
    GPIO_HIGH(1, 7);
    GPIO_HIGH(1, 2);
    GPIO_HIGH(1, 4);
    GPIO_HIGH(BT_RESET_GPIO_PORT, BT_RESET_GPIO_PIN);

    GPIO_LOW(1, 6);
    GPIO_LOW(1, 7);
    GPIO_LOW(1, 2);
    GPIO_LOW(1, 4);
    GPIO_LOW(BT_RESET_GPIO_PORT, BT_RESET_GPIO_PIN);
#endif
    LED1_TOGGLE();
#endif

}

void bttask_pal_handleWakeupSignal(void)
{
}

void bttask_pal_startBluetoothPort(bttask_StartCallback callback)
{
  vTaskDelay(60);

  // Take BT controller out of reset
  GPIO_HIGH(BT_RESET_GPIO_PORT, BT_RESET_GPIO_PIN);

  // Enable only the line status interrupt.
  // Receive and transmit interrupts are enabled on as needed basis.
  // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RLS, ENABLE);
  UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RLS, ENABLE);

  // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RBR, ENABLE);

  // Enable UART transmitter.
  // NsT UART_TxCmd((LPC_UART_TypeDef*)BT_UART, ENABLE);
  UART_TxCmd((LPC_USARTn_Type*)BT_UART, ENABLE);

  // Remember callback to be called when port startup is complete
  mStartCallback = callback;

  btport_setLoadingInitScript(1);

  // Initialize and start transport layer that will be used
  // for controller initialization.
  bt_hcitr_uart_init();
  bt_hcitr_uart_start();

  // Initialize the HCI layer but do not start it.
  // This is needed because the subsequent call to btx_csr_autobaud
  // is using HCI internal buffers.
  bt_hci_init();

  // Start BlueCore controller initialization.
  btx_csr_init();
  btx_csr_bc7_sel_host_interface_h4(&mWorkBuffers.btx_csr_autobaud, AUTO_BAUD_INTERVAL, selHostInterfaceCallback, NULL);
}

void bttask_pal_restartBluetoothPort(bttask_StartCallback callback)
{
}

static void selHostInterfaceCallback(bt_bool success, btx_csr_autobaud_buffer_t* buffer)
{
  if (success)
  {
    // Start the HCI layer but without executing its standard initialization
    // sequence as for now we are going to use it to send patches and
    // set configuration parameters. After that we will perform a warm reset and
    // start everything again.
    bt_hci_start_no_init();

    // Execute script that loads patches.
    btx_csr_patch_controller(
                    _initScripts,
                    sizeof(_initScripts) / sizeof(btx_csr_get_script_fp),
                    &mWorkBuffers.btx_csr_exec_script,
                    execScriptCallback, NULL);
  }
  else
  {
    error_onFatalError();
  }
}

static void execScriptCallback(bt_bool success, btx_csr_exec_script_buffer_t* buffer)
{
  if (success)
  {
    // Set configuration parameters.
    btx_csr_set_ps_vars(BC7_PS_VALUES, &mWorkBuffers.btx_csr_set_ps_vars, setPsVarsCallback, NULL);
  }
  else
  {
    error_onFatalError();
  }
}

static void setPsVarsCallback(bt_bool success, btx_csr_set_ps_vars_buffer_t* buffer)
{
  if (success)
  {
    // Perform warm reset. The controller will reset but it will keep the
    // patches and configuration parameters we sent before. The btx_csr_warm_reset()
    // returns immediately after submitting the HCI command carrying the warm reset
    // command. The controller does not respond to this command as it starts
    // resetting immediately upon receiving the command. warmResetCallback is called
    // right after the command packet has been transmitted to the controller.
    btx_csr_warm_reset_ex(&warmResetCallback, NULL);
  }
  else
  {
    error_onFatalError();
  }
}

static void warmResetCallback(bt_int status, bt_hci_command_t* cmd, bt_hci_event_t* evt)
{
  // Wait until UART completes transmitting
  // NsT while (UART_CheckBusy((LPC_UART_TypeDef*)BT_UART) == SET) {}
  while (UART_CheckBusy((LPC_USARTn_Type*)BT_UART) == SET) {}

  // At this point the controller is reseting itself. While it is busy doing that
  // we reconfigure our UART to working speed.
  btport_configureForHighSpeed();

  // We are going to wait for a short period of time that should be enough
  // for the controller to receive the warm reset command and reset.
  // Since the L2CAP layer is not yet functioning we can reuse one of its timers.
  bt_oem_timer_set(BT_TIMER_L2CAP, 200, &timerCallback);
}

static void timerCallback(void)
{
  bt_hci_stop(&hciStopCallback, NULL);
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

  bt_hcitr_uart_reset();
  bt_hcitr_uart_start();
  btport_setLoadingInitScript(0);

  mStartCallback();
}

static void configureUart(uint32_t baudRate)
{
  // Configure UART clock
  // NsT CLKPWR_SetPCLKDiv(BT_UART_PCLK, BT_UART_PCLK_DIV);

  // Configure UART pins
  configureOutputPin(BT_UART_TX_PORT, BT_UART_TX_PIN,  BT_UART_TX_FUNC);
  configureInputPin(BT_UART_RX_PORT, BT_UART_RX_PIN,  BT_UART_RX_FUNC, MD_PUP);
  configureOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN,  BT_UART_RTS_FUNC);
  configureInputPin(BT_UART_CTS_PORT, BT_UART_CTS_PIN,  BT_UART_CTS_FUNC, MD_PUP);

  // Configure UART parameters
  {
    UART_CFG_Type uartCfg;
    UART_ConfigStructInit(&uartCfg);
    uartCfg.Baud_rate = baudRate;
    uartCfg.Parity = UART_PARITY_NONE;
    uartCfg.Databits = UART_DATABIT_8;
    uartCfg.Stopbits = UART_STOPBIT_1;
    // NsT UART_Init((LPC_UART_TypeDef*)BT_UART, &uartCfg);
    UART_Init((LPC_USARTn_Type*)BT_UART, &uartCfg);
  }

  // Configure UART FIFO
  {
    UART_FIFO_CFG_Type fifoCfg;
    UART_FIFOConfigStructInit(&fifoCfg);
    // NsT UART_FIFOConfig((LPC_UART_TypeDef*)BT_UART, &fifoCfg);
    UART_FIFOConfig((LPC_USARTn_Type*)BT_UART, &fifoCfg);
  }

  // Enable auto RTS/CTS
  UART_FullModemConfigMode(BT_UART, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
  UART_FullModemConfigMode(BT_UART, UART1_MODEM_MODE_AUTO_RTS, ENABLE);

  // UART_Send((LPC_USARTn_Type*)BT_UART, "Hello World", sizeof("Hello World"), BLOCKING);	

  NVIC_SetPriority(UART1_IRQn, 5);  // TODO: make it configurable
  NVIC_EnableIRQ(UART1_IRQn);
}


void btport_configureForHighSpeed(void )
{
  configureUart(BT_UART_WORKING_BAUD_RATE);

  // Enable only the line status interrupt.
  // Receive and transmit interrupts are enabled on as needed basis.
  // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RLS, ENABLE);
  UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RLS, ENABLE);

  // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
  UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RBR, ENABLE);

  // Enable UART transmitter.
  // NsT UART_TxCmd((LPC_UART_TypeDef*)BT_UART, ENABLE);
  UART_TxCmd((LPC_USARTn_Type*)BT_UART, ENABLE);
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

    // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, ENABLE);
    UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RBR, ENABLE);
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
  {
    mTxCallback();
  }
}

void btport_suspendRx(void)
{
  taskENTER_CRITICAL();
  {
    // (Re)Configure BT RTS pin as GPIO and deassert it
    GPIO_HIGH(BT_UART_RTS_GPIO_PORT, BT_UART_RTS_GPIO_PIN);
    configureGPIOOutputPin(BT_UART_RTS_PORT, BT_UART_RTS_PIN, BT_UART_RTS_GPIO_FUNC, BT_UART_RTS_GPIO_PORT, BT_UART_RTS_GPIO_PIN);
  }
  taskEXIT_CRITICAL();
}


void btport_resumeRx(void)
{
  taskENTER_CRITICAL();
  {
    // (Re)Configure BT RTS pin as AF
    GPIO_LOW(BT_UART_RTS_GPIO_PORT, BT_UART_RTS_GPIO_PIN);
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

    // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_THRE, ENABLE);
    UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_THRE, ENABLE);
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
        // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_THRE, DISABLE);
        UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_THRE, DISABLE);
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
              // NsT UART_IntConfig((LPC_UART_TypeDef*)BT_UART, UART_INTCFG_RBR, DISABLE);
              UART_IntConfig((LPC_USARTn_Type*)BT_UART, UART_INTCFG_RBR, DISABLE);
              break;
            }
          }
        }

        if (setSignal)
        {
          bttask_setSignalFromISR(BTTASK_SIG_RX, &higherPriorityTaskWoken);
        }
        break;
      }
    default:
      break;
  }

  if (mLoadingInitScript)
  {
    LED1_TOGGLE();		
  }

  portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

static void configureGPIOOutputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t gpio_port, uint8_t gpio_pin)
{
  scu_pinmux(port, pin, MD_PDN, func);
  GPIO_SetDir(gpio_port, (1UL << gpio_pin), 1);
}

static void configureOutputPin(uint8_t port, uint8_t pin,  uint8_t func)
{
  scu_pinmux(port, pin, MD_PDN, func);
#if 0
  PINSEL_CFG_Type pinsel;

  pinsel.Portnum = port;
  pinsel.Pinnum = pin;
  pinsel.Funcnum = func;
  pinsel.Pinmode = PINSEL_PINMODE_PULLUP;
  pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;

  PINSEL_ConfigPin(&pinsel);
#endif
}

static void configureGPIOInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t gpio_port, uint8_t gpio_pin)
{
  scu_pinmux(port, pin, MD_PDN, func);
  GPIO_SetDir(gpio_port, (1UL << gpio_pin), 0);
}

static void configureInputPin(uint8_t port, uint8_t pin,  uint8_t func, uint8_t mode)
{
  scu_pinmux(port, pin, mode|MD_EZI|MD_ZI, func);
#if 0
  PINSEL_CFG_Type pinsel;
  
  pinsel.Portnum = port;
  pinsel.Pinnum = pin;
  pinsel.Funcnum = func;
  pinsel.Pinmode = mode;
  pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;
  
  PINSEL_ConfigPin(&pinsel);
#endif
}


#endif
