//******************************************************************************
//
// Project NIMBUS Application header file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       .h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:
//
//******************************************************************************

#if !defined(_HELPERS_HPP_)
#define _HELPERS_HPP_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

/*
  tPair is used to create key/value pairs
*/

template <typename Tfirst, typename Tsecond> class tPair
{
public:
  Tfirst                    first;
  Tsecond                   second;
};

/*
  ICustomCallback serves the purpose of being able to define pointers to the
  specialised instances of ICallback.
*/
class ICustomCallback
{
public:
                            ICustomCallback() {}
  virtual                  ~ICustomCallback() {}
};

/*
  ICallback defines the interface for a callback method which can be
  implemented by any class.  CallbackParamType defines the expected type
  of the parameter passed to the vHandler method.
*/
template <typename CallbackParamType> class ICallback : public ICustomCallback
{
public:
                            ICallback() : ICustomCallback() {}
  virtual                  ~ICallback() {}

  virtual void              vHandler( CallbackParamType xParam ) = 0;
};

/*
  tLock implements a resource locking class, for re-entrancy protection.
  Normally used in conjunction with the tAutoLock class (but may be used
  in isolation too.
*/
class tLock
{
public:
  tLock()
    : hLock( 0 )
  {
  }

  ~tLock()
  {
    if ( hLock != 0 )
    {
      vSemaphoreDelete( hLock );
    }
  }

  /*
    vInit should ideally be called prior to using the class, to avoid any
    re-entrancy issues during initialisation (hLock might be initialised
    twice by two different threads calling vInit simultaneously.
  */
  virtual void vInit()
  {
    if ( 0 == hLock )
    {
      hLock = xSemaphoreCreateRecursiveMutex();
    }
  }

  /*
    vLock locks the semaphore.  If already held by this thread, nothing
    happens (although vUnlock still needs to be called the same number of
    times).
  */
  void vLock()
  {
    vInit();
    xSemaphoreTakeRecursive( hLock, RTOS_FOREVER );
  }

  /*
    vUnLock unlocks the semaphore.  Must be called the same number of
    times as vLock.  tAutoLock is a good way to ensure this happens.
  */
  void vUnlock()
  {
    vInit();
    xSemaphoreGiveRecursive( hLock );
  }
private:
  xSemaphoreHandle hLock;
};

/*
  tAutoLock creates an automatic lock for a region of code.
  Usage:

    // oLock is defined globally or in the encapsulating class so only
    // one instance exists for all threads.  This constructor call causes
    // the lock to be acquired.
    {
      tAutoLock oAuto( oLock );

      <...>
      Code that needs protecting by lock

    }
    // at this point oAuto falls out of scope and the lock is released.

*/

class tAutoLock
{
public:
  tAutoLock( tLock &roLock )
  {
    poLock = &roLock;
    poLock->vLock();
  }

  ~tAutoLock()
  {
    poLock->vUnlock();
  }

private:
  tLock                      *poLock;
};

/*
  tListItem encapsulates a FreeRTOS xListItem.
*/
class tListItem
{
public:
  tListItem( void *poOwner )
    : pvData( poOwner )
  {
    vListInitialiseItem( &sItem );
    listSET_LIST_ITEM_OWNER( &sItem, this );
  }
  virtual                  ~tListItem()
  {
    if ( listLIST_ITEM_CONTAINER( &sItem ) != 0 )
    {
      uxListRemove( &sItem );
    }
  }

  inline xListItem         &sGetItem()                    { return sItem; }
  inline void              *pvGetData()                   { return pvData; }
  inline void               vSetData( void *pvValue )     { pvData = pvValue; }
private:
  // make auto constructor private to prevent automatic invokation by the compiler
  tListItem()               {}
  xListItem                 sItem;
  void                     *pvData;
};

/*
  tList encapsulates a FreeRTOS xList.
*/
class tList
{
public:
  tList()
    : boInitialised( false )
  {
  }

  virtual                  ~tList()
  {
  }

  void                      vInit()
  {
    vListInitialise( &sList );
    oLock.vInit();
    boInitialised = true;
  }

  bool                      boInsert( tListItem *poItem, void *pvDataValue );
  bool                      boRemove( tListItem *poItem );

  tListItem                *poGetFirst();
  tListItem                *poGetNext();

  tLock                    &oGetLock();
  xList                    &sGetList();
  bool                      boIsEmpty();
private:
  tLock                     oLock;
  xList                     sList;
  bool                      boInitialised;
};

/*
  tQueue encapsulates a FreeRTOS queue in a typesafe manner
*/
template <typename EntryClass> class tQueue
{
public:
                            tQueue() : hQueue( 0 ) {}
  virtual                  ~tQueue()
                            {
                              if ( hQueue != 0 )
                              {
                                vQueueDelete( hQueue );
                              }
                            }

  void                      vInit( const int iNoOfElements )
                            {
                              if ( hQueue != 0 )
                              {
                                vQueueDelete( hQueue );
                              }
                              hQueue = xQueueCreate( iNoOfElements, sizeof( EntryClass * ) );
                            }

  EntryClass               *poGet( dword dwWaitingTime = 0 )
                            {
                              EntryClass *poRes = 0;

                              if ( hQueue != 0 )
                              {
                                xQueueReceive( hQueue, &poRes, dwWaitingTime );
                              }

                              return poRes;
                            }

  bool                      boPut( EntryClass *poEntry )
                            {
                              bool boRes = false;
                              if ( hQueue != 0 )
                              {
                                boRes = ( xQueueSend( hQueue, &poEntry, 0 ) == pdTRUE );
                              }

                              return boRes;
                            }

  bool                      boPutToFront( EntryClass *poEntry )
                            {
                              bool boRes = false;
                              if ( hQueue != 0 )
                              {
                                boRes = ( xQueueSendToFront( hQueue, &poEntry, 0 ) == pdTRUE );
                              }

                              return boRes;
                            }

int                         iFreeBufferCount()
                            {
                              int iFreeCount = 0;

                              if ( hQueue != 0 )
                              {
                                iFreeCount = uxQueueMessagesWaiting( hQueue );
                              }

                              return iFreeCount;
                            }


private:
  xQueueHandle              hQueue;
};

/*
  tIPaddress can be used to store an IP address as a dword and for
  converting to and from a string.
*/
class tIPaddress
{
public:
                            tIPaddress( const dword dwIPaddress );
                            tIPaddress( const char * const pzIPaddress );

  // get as a dword
  dword                     dwGet();
  // get as a string
  char * const              pzGet();
private:
  // room for aaa.bbb.ccc.ddd and null terminator
  char                      azBuffer[ 4 + 4 + 4 + 4 ];
  union {
                            byte  abIP[4];
                            dword dwIP;
  } uIP;
};

/*
  tPulseStretch can be used to stretch a pulse through a callback
*/
class tPulseStretch
{
public:
                            tPulseStretch( const int iSetPulseLengthTicks,
                                           ICallback<bool> *poSetCallback );

  void                      vSet();
  void                      vClear();
private:
  xTimerHandle              hTimer;
  int                       iPulseLengthTicks;
  ICallback<bool>          *poCallback;
};


//******************************************************************************
// Function Prototypes
//******************************************************************************



#endif  // <HEADER_DEFINE>
