
#include "global.h"
#include "hal.h"
#include "cli.h"
#include "exception.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/*

Stubs of functions that are outside of the test set
Done to keep the linker happy.
These should be mocked if called in a test

*/

/*
hal.cpp
*/
extern "C" int iUsbInit(tInterface eWhich, vUsbCallback pvCallback, void *pvContext){ return 0;}
extern "C" void vUiVibrateControl(bool boOnOff){}
extern "C" void vUiInit(){}
extern "C" void vUiLedSet(tLeds eLed, tColorOptions eColor){}
extern "C" void vUiButtonCapture(vCallback pvCallback, void *pvData){}
extern "C" void vUn20Reset( bool boReset ){}
extern "C" void vPowerUsbOff(){}
extern "C" void vPowerUsbOn(){}
extern "C" void vPowerBtOff(){}
extern "C" void vPowerBtOn(){}
extern "C" void vPowerUn20Off(){}
extern "C" void vPowerUn20On(){}
extern "C" void vPowerSelfOff(){}
extern "C" void vPowerSelfOn(){}
extern "C" void vPowerSelfNotify(vCallback pvCallback){}
extern "C" int iBatteryVoltage(int iChannel){ return 4000;}
extern "C" int iHardwareVersion(void){ return 1;}
extern "C" void vSetHardwareConfig( int iMode ){}
extern "C" bool boHalBatteryIsCharging(){ return false;}
extern "C" bool boHalUSBChargePresent(){ return false;}
extern "C" bool boHalScanButtonPressed(){ return false;}
extern "C" bool boHalPowerButtonPressed(){ return false;}
extern "C" void vSetDebugPin(bool bo){}
extern "C" int iBtInit(vBtCallback pvCallback, void *pvData){ return 1;}
extern "C" void vBtCallbackFunction(tInterfaceEvent event, void *event_data){}
extern "C" int iBtSend(void *data, int count){ return 0;}
extern "C" int iBtReset( bool boReset ){ return 0;}
extern "C" void vUN20CallbackFunction(tInterfaceEvent event, void *event_data){}
extern "C" int iUsbSend(tInterface eWhich, void *pvData, int iCount){ return 0;}

/*
cli.c
*/
extern "C" bool boCLIregisterEntry(const tParserEntry *psNewEntry){ return true; }

/*
app_main.cpp
*/
extern "C" byte *pbGetBluetoothAddress(void){ return NULL; } /* ToDo: move out of app_main.cpp */

/*
exception.c
*/
extern "C" bool boLogCacheGet( tExceptionRecord **poException, word *pwLength ){ return false;}
extern "C" void vLogCacheClear(){}
extern "C" void vLogGeneral( byte bReason, void *pvData, word wLength ){}

/*
printf-stdarg.cpp
*/
int _printf(const char *format, ...){ return 0;}

/*
External
*/
extern "C" void *pvPortMalloc( size_t xSize ){ return NULL; }
extern "C" void vPortFree( void *pv ){  }
extern "C" unsigned long ulPortSetInterruptMask( void ){ return 0;}
extern "C" void vTaskDelay( portTickType xTicksToDelay ){}
extern "C" xQueueHandle xQueueGenericCreate( unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize, unsigned char ucQueueType ){ return NULL; }
extern "C" signed portBASE_TYPE xQueueGenericReceive( xQueueHandle xQueue, const void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking ){ return 0;}
extern "C" signed portBASE_TYPE xQueueGenericSend( xQueueHandle xQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition ){ return 0;}
extern "C" signed portBASE_TYPE xQueueGenericSendFromISR( xQueueHandle xQueue, const void * const pvItemToQueue, signed portBASE_TYPE *pxHigherPriorityTaskWoken, portBASE_TYPE xCopyPosition ){ return 0;}
extern "C" signed portBASE_TYPE xTaskGenericCreate( pdTASK_CODE pxTaskCode, const signed char * const pcName, unsigned short usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask, portSTACK_TYPE *puxStackBuffer, const xMemoryRegion * const xRegions ){ return 0;}
extern "C" portBASE_TYPE xTimerGenericCommand( xTimerHandle xTimer, portBASE_TYPE xCommandID, portTickType xOptionalValue, signed portBASE_TYPE *pxHigherPriorityTaskWoken, portTickType xBlockTime ){ return 0;}
extern "C" portTickType xTaskGetTickCount( void ){ return 0;}
extern "C" xTimerHandle xTimerCreate( const signed char * const pcTimerName, portTickType xTimerPeriodInTicks, unsigned portBASE_TYPE uxAutoReload, void *pvTimerID, tmrTIMER_CALLBACK pxCallbackFunction ){ return NULL;}
extern "C" int sppCancelAndDisconnect(){ return 0;}

