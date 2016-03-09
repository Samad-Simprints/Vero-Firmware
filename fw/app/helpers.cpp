//******************************************************************************
//
// Project INDEX Application source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       .c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include <string.h>

#include "global.h"
//#include "exception.h"

#include "helpers.hpp"
#include "gpio_pindefs.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "list.h"

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// tList
//******************************************************************************

tLock &tList::oGetLock()
{
  PBC_ASSERT( boInitialised );
  return oLock;
}

xList &tList::sGetList()
{
  PBC_ASSERT( boInitialised );
  return sList;
}

bool tList::boIsEmpty()
{
  PBC_ASSERT( boInitialised );
  return ( listLIST_IS_EMPTY( &sList ) == pdTRUE );
}

/* -----------------------------------------------------------------------
   tList::boInsert
   .......................................................................
   Description  : Adds an entry to the end of the list if not already in
                  the list.

   Params : poItem - list item to add
            pvDSataValue - value to assign to pvData field, typically used
                           to point to the owner of the list item

   Returns : true if item was not already in the list, i.e. was inserted.
   ----------------------------------------------------------------------- */

bool tList::boInsert( tListItem *poItem, void *pvDataValue )
{
  PBC_ASSERT( boInitialised );
  bool boRes = false;
  tAutoLock oAuto( oLock );

  // check an item has been provided
  PRECOND( poItem != 0 );
  xListItem *psItem = &poItem->sGetItem();

  // check if not already in the list
  boRes = ( listIS_CONTAINED_WITHIN( &sList, psItem ) == pdFALSE );
  if ( boRes )
  {
    // check it is not already in another list
    PRECOND( listLIST_ITEM_CONTAINER( psItem ) == 0 );
    if ( listLIST_ITEM_CONTAINER( psItem ) == 0 )
    {
      poItem->vSetData( pvDataValue );
      vListInsertEnd( &sList, psItem );
    }
  }

  return boRes;
}

/* -----------------------------------------------------------------------
   tList::boRemove
   .......................................................................
   Description  : Removes an entry from the list

   Params : poItem - list item to remove

   Returns : true if item was in the list, i.e. was removed.
   ----------------------------------------------------------------------- */

bool tList::boRemove( tListItem *poItem )
{
  PBC_ASSERT( boInitialised );
  bool boRes = false;
  tAutoLock oAuto( oLock );

  // only remove if in the list
  if ( listLIST_ITEM_CONTAINER( &poItem->sGetItem() ) == &sList )
  {
    uxListRemove( &poItem->sGetItem() );
    boRes = true;
  }

  return boRes;
}

/* -----------------------------------------------------------------------
   tList::poGetFirst
   .......................................................................
   Description  : Retrieves the first item in the list

   Params : None

   Returns : tListItem * - first item in list, or 0
   ----------------------------------------------------------------------- */

tListItem *tList::poGetFirst()
{
  PBC_ASSERT( boInitialised );

  // we need a lock, as we're resetting the list back to start
  tAutoLock oAuto( oLock );

  // reset the list back to beginning
  sList.pxIndex = (xListItem *)&(sList.xListEnd);

  return ( (tListItem *)listGET_OWNER_OF_HEAD_ENTRY( &sList ) );
}

/* -----------------------------------------------------------------------
   tList::poGetNext
   .......................................................................
   Description  : Retrieves the next item in the list

   Params : None

   Returns : tListItem * - first item in list, or 0
   ----------------------------------------------------------------------- */

tListItem *tList::poGetNext()
{
  PBC_ASSERT( boInitialised );
  void *pvRes = 0;

  // we need a lock, as we're moving the list index
  tAutoLock oAuto( oLock );

  listGET_OWNER_OF_NEXT_ENTRY( pvRes, &sList );

  return ( (tListItem *)pvRes );
}

//******************************************************************************
// tIPaddress
//******************************************************************************

/* -----------------------------------------------------------------------
   tIPaddress::tIPaddress
   .......................................................................
   Description  : Constructor, sets IP address to dword value

   Params : dwIPaddress - IP address

   Returns : Nothing
   ----------------------------------------------------------------------- */

tIPaddress::tIPaddress( const dword dwIPaddress )
{
  uIP.dwIP = dwIPaddress;
  sprintf( azBuffer,
          "%d.%d.%d.%d",
          uIP.abIP[ 0 ],
          uIP.abIP[ 1 ],
          uIP.abIP[ 2 ],
          uIP.abIP[ 3 ] );
}

/* -----------------------------------------------------------------------
   tIPaddress::tIPaddress
   .......................................................................
   Description  : Constructor, sets IP address to stringvalue

   Params : pzIPaddress - IP address

   Returns : Nothing
   ----------------------------------------------------------------------- */

tIPaddress::tIPaddress( const char * const pzIPaddress )
{
  char *pzScan = azBuffer;
  strlcpy( azBuffer, pzIPaddress, sizeof( azBuffer ) );

  for ( int i = 0; ( ( i < 4 ) && ( pzScan != 0 ) ); ++i )
  {
    int iTemp;
    sscanf( pzScan, "%d", &iTemp );
    uIP.abIP[ i ] = (byte)iTemp;
    pzScan = strchr( pzScan, '.' ) + 1;
  }
}

/* -----------------------------------------------------------------------
   tIPaddress::dwGet
   .......................................................................
   Description  : Returns IP address as dword

   Params : None

   Returns : dword - IP address
   ----------------------------------------------------------------------- */

dword tIPaddress::dwGet()
{
  return uIP.dwIP;
}

/* -----------------------------------------------------------------------
   tIPaddress::pzGet
   .......................................................................
   Description  : Returns IP address as string

   Params : None

   Returns : char * const - IP address
   ----------------------------------------------------------------------- */

char * const tIPaddress::pzGet()
{
  return azBuffer;
}

//******************************************************************************
// tPulseStretch
//******************************************************************************

/* -----------------------------------------------------------------------
   vPulseStretchTimerCallback
   .......................................................................
   Description  : Timer callback for the pulse stretcher, clears the
                  state of the associated pulse stretcher

   Params : hTimer - handle to timer

   Returns : Nothing
   ----------------------------------------------------------------------- */

void vPulseStretchTimerCallback( xTimerHandle hTimer )
{
  PRECOND( hTimer != 0 );

  // Which timer expired?
  tPulseStretch *poPulser = (tPulseStretch *)pvTimerGetTimerID( hTimer );
  if ( poPulser != 0 )
  {
    poPulser->vClear();
  }
}

/* -----------------------------------------------------------------------
   tPulseStretch::tPulseStretch
   .......................................................................
   Description  : Constructor

   Params : iPulseLengthTicks - pulse length in ticks
            poCallback - callback routine when pulse starts or stops

   Returns : Nothing
   ----------------------------------------------------------------------- */

tPulseStretch::tPulseStretch( const int iSetPulseLengthTicks,
                              ICallback<bool> *poSetCallback )
  : poCallback( poSetCallback ),
    iPulseLengthTicks( iSetPulseLengthTicks )
{
  hTimer = xTimerCreate( (const signed char *)"ps",
                         iPulseLengthTicks,
                         pdFALSE,
                         (void *)this,
                         vPulseStretchTimerCallback );
}

/* -----------------------------------------------------------------------
   tPulseStretch::vSet
   .......................................................................
   Description  : Set the pulse stretcher to its active state and start
                  the pulse timer

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tPulseStretch::vSet()
{
  if ( poCallback != 0 )
  {
    poCallback->vHandler( true );
  }
  xTimerChangePeriod( hTimer, iPulseLengthTicks, RTOS_FOREVER );
  xTimerStart( hTimer, 0 );
}

/* -----------------------------------------------------------------------
   tPulseStretch::vClear
   .......................................................................
   Description  : Set the pulse stretcher to its inactive state

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tPulseStretch::vClear()
{
  if ( poCallback != 0 )
  {
    poCallback->vHandler( false );
  }
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************
#if !defined(CM_HOSTED)
/* -----------------------------------------------------------------------
abort
.......................................................................
Description  : Called if std::allocator fails

Params : None

Returns : Nothing
----------------------------------------------------------------------- */

extern "C" void abort()
{
  taskDISABLE_INTERRUPTS();
  //vLogGeneral( ERROR_SOFTWARE_ABORT, 0, 0 );
}
#endif

#if 0
/* -----------------------------------------------------------------------
   __cxa_pure_virtual
   .......................................................................
   Description  : Helper giving support for C++

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void __cxa_pure_virtual()
{
  taskDISABLE_INTERRUPTS();
  //vLogGeneral( ERROR_SOFTWARE_CPP, 0, 0 );
}

/* -----------------------------------------------------------------------
   __cxa_guard_acquire
   .......................................................................
   Description  : Helper giving support for C++

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" int __cxa_guard_acquire( long long *pllLock )
{
  return 0;
}

/* -----------------------------------------------------------------------
   __cxa_guard_release
   .......................................................................
   Description  : Helper giving support for C++

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void __cxa_guard_release( long long *pllLock )
{
}
#endif

/* -----------------------------------------------------------------------
   vApplicationIdleHook
   .......................................................................
   Description  : Called by FreeRTOS when idle

   Call the user defined function from within the idle task.  This
   allows the application designer to add background functionality
   without the overhead of a separate task.
   NOTE: vApplicationIdleHook() MUST NOT, UNDER ANY CIRCUMSTANCES,
         CALL A FUNCTION THAT MIGHT BLOCK.

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void vApplicationIdleHook( void )
{
#if defined(CM_HOSTED)
  Sleep(INFINITE);
#else
  //
  // put the processor to sleep, as there is nothing to do right now
  //
#if 1//ndef DEBUG
  DEBUG_GPIO0->vSet(true);
  __asm__( "WFI" );
  DEBUG_GPIO0->vSet(false);
#endif
#endif
}

/* -----------------------------------------------------------------------
   vApplicationMallocFailedHook
   .......................................................................
   Description  : Called by FreeRTOS if malloc fails

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void vApplicationMallocFailedHook( void )
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */

  // log malloc failure and restart
  taskDISABLE_INTERRUPTS();
  char *pzTaskName = (char*)pcTaskGetTaskName( NULL );
  //vLogGeneral( ERROR_SOFTWARE_RTOS_MALLOC, pzTaskName, strlen(pzTaskName)+1 );
}

/* -----------------------------------------------------------------------
   vApplicationStackOverflowHook
   .......................................................................
   Description  : Called by FreeRTOS if a task overflows its stack

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
  taskDISABLE_INTERRUPTS();

  ( void ) pcTaskName;
  ( void ) pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */

  // log stack error and restart
  //vLogGeneral( ERROR_SOFTWARE_RTOS_STACK, pcTaskName, strlen((char*)pcTaskName)+1 );
}

#if defined(CM_HOSTED)

/* -----------------------------------------------------------------------
strlcpy
.......................................................................
Description  : ??

Params : None

Returns : size_t
----------------------------------------------------------------------- */

size_t strlcpy(char *dest, const char *src, size_t size)
{
  size_t ret = strlen(src);

  if ( size != 0 )
  {
    size_t len = (ret >= size) ? size - 1 : ret;
    memcpy(dest, src, len);
    dest[len] = '\0';
  }
  return ret;
}

// linkage glue
uint32_t SystemCoreClock = 100000000;     /*!< System Clock Frequency (Core Clock)  */
extern void SystemCoreClockUpdate(void)
{
}

#endif
