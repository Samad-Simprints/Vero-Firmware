#ifndef __FREERTOS_CONFIG_IAR_H_INCLUDED__
#define __FREERTOS_CONFIG_IAR_H_INCLUDED__

//------------------------------------------------------------------------------
// The following definitions are specific and fine tuned for the DotStack SDK
// examples. If new tasks or other FreeRTOS objects are added to the application
// the total heap size should be adjusted accordingly. Also, keep in mind that
// stack sizes are expressed in 32-bit word units.
//------------------------------------------------------------------------------

#ifndef max
	#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#endif

// Priority of the task in which all DotStack code is executed.
#define configBTTASK_PRIORITY           (tskIDLE_PRIORITY + 2)

// Audio task configuration
#if defined(BTAPP_USE_AUDIO)
    #define configAUDIO_TASK_PRIORITY           (tskIDLE_PRIORITY + 2)
    #if defined(BTAPP_WITH_AAC)
    	#define configAUDIO_TASK_STACK_SIZE     ( 1350 )
	#else
    	#define configAUDIO_TASK_STACK_SIZE     ( 200 )
	#endif

    #ifdef BTAPP_USE_AUDIO_ANALIZER
        #define configANALIZER_TASK_PRIORITY    (tskIDLE_PRIORITY)
        #define configANALIZER_TASK_STACK_SIZE  ( 200 )
        #define configHEAP_EXTRA_ANALIZER       ( 256 )
    #else
        #define configANALIZER_TASK_STACK_SIZE  ( 0 )
        #define configHEAP_EXTRA_ANALIZER       ( 0 )
    #endif

#else
    #define configAUDIO_TASK_STACK_SIZE         ( 0 )
	#define configANALIZER_TASK_STACK_SIZE      ( 0 )
	#define configHEAP_EXTRA_ANALIZER           ( 0 )
#endif

#if defined(BTAPP_USE_AVRCP)
	#define configBTTASK_AVRCP_EXTRA_STACK      ( 128 )
#else
	#define configBTTASK_AVRCP_EXTRA_STACK      ( 0 )
#endif

#ifdef BTAPP_USE_BLE
	#define configBTTASK_EXTRA_STACK    ( 80 )
#else
	#define configBTTASK_EXTRA_STACK    ( 0 )
#endif

#define configBTTASK_STACK_SIZE         ( 200 + max(configBTTASK_EXTRA_STACK, configBTTASK_EXTRA_STACK) )

#define configBASE_HEAP_SIZE			( 480 )

#ifdef ENABLE_LOG
	// Extra heap needed when loogin enabled (uses one mutex and USB task)
	#define configHEAP_EXTRA_LOG  62 + (configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE))
#else
	#define configHEAP_EXTRA_LOG  0
#endif

// Total heap size should include space for stacks of all tasks, and other
// objects allocated uisng FreeRTOS memoru APIs.
#define configTOTAL_HEAP_SIZE           ( (size_t) (configBASE_HEAP_SIZE + configHEAP_EXTRA_LOG + (configBTTASK_STACK_SIZE + configMINIMAL_STACK_SIZE + configAUDIO_TASK_STACK_SIZE) * sizeof(portSTACK_TYPE)) )

#endif // __FREERTOS_CONFIG_IAR_H_INCLUDED__
