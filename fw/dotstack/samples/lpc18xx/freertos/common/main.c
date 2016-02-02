#include "FreeRTOS.h"
#include "task.h"

#include "bttask.h"
#include "usbcdc.h"
#ifdef BTAPP_USE_AUDIO
#include "audio.h"
#endif

#include "lpc18xx_gpdma.h"

void __assert( const char *__expression, const char *__filename, int __line )
{
  for( ;; );
}

int bt_main()
{
#if 0
  // Ensure that all interrupt priority bits are used as the pre-emption priority
  NVIC_SetPriorityGrouping(2);
#endif	
#ifdef ENABLE_LOG
#ifdef LOG_USE_USB
  usbcdc_init();
#else
  GPDMA_Init();
#endif
#endif

  bttask_init();

#ifdef BTAPP_USE_AUDIO
  audio_init();
#endif

#if 0	
  vTaskStartScheduler();
#endif
  return 0;
}


