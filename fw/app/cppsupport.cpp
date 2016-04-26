//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2015. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       cppsupport.hpp
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#include "FreeRTOS.h"

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

/* -----------------------------------------------------------------------
   <FUNCTION NAME>
   .......................................................................
   Description  : <FUNCTION DESCRIPTION>

   Params : <PARAM1 NAME> - <PARAM1 DESCRIPTION>
            <PARAM2 NAME> - <PARAM2 DESCRIPTION>
            .....
            <PARAMn NAME> - <PARAMn DESCRIPTION>

   Returns : <DESCRIPTION OF RETURN PARAM>
   e.g.
   Returns : TRUE if succeeded. FALSE if failed.
   Returns : File Handle, -1 if failed to open
   Returns : Nothing
   Returns : int - Number of Hex data bytes written to pabOutputData
   ----------------------------------------------------------------------- */

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   malloc
   .......................................................................
   Description  : redirection of malloc through FreeRTOS's heap

   Params : iSize - size of allocation

   Returns : void * - allocated memory, or NULL
   ----------------------------------------------------------------------- */

extern "C" void *malloc( size_t iSize )
{
  void *pvRes = pvPortMalloc( iSize );
  return pvRes;
}

/* -----------------------------------------------------------------------
   calloc
   .......................................................................
   Description  : redirection of malloc through FreeRTOS's heap

   Params : iNumberOfObj - number of objects to allocate
            iSize - size of each object

   Returns : void * - allocated memory, or NULL
   ----------------------------------------------------------------------- */

extern "C" void *calloc( size_t iNumberOfObj, size_t iSize )
{
  return malloc( iNumberOfObj * iSize );
}

/* -----------------------------------------------------------------------
   free
   .......................................................................
   Description  : redirection of free through FreeRTOS's heap

   Params : pvPtr - allocated block

   Returns : Nothing
   ----------------------------------------------------------------------- */

extern "C" void free( void *pvPtr )
{
  vPortFree( pvPtr );
}

/* -----------------------------------------------------------------------
   operator new
   .......................................................................
   Description  : Our own new operator, for object instantiation

   Params : size - size of allocation

   Returns : void * - allocated memory, or NULL
   ----------------------------------------------------------------------- */

// our own new and delete...
void *operator new(size_t size)
{
  return pvPortMalloc( size );
}

/* -----------------------------------------------------------------------
   operator new
   .......................................................................
   Description  : Our own new operator, for array instantiation

   Params : size - size of allocation

   Returns : void * - allocated memory, or NULL
   ----------------------------------------------------------------------- */

void *operator new[](size_t size)
{
  return pvPortMalloc( size );
}

/* -----------------------------------------------------------------------
   operator delete
   .......................................................................
   Description  : Our own delete operator, for object deletion

   Params : ptr - pointer to object

   Returns : Nothing
   ----------------------------------------------------------------------- */

void operator delete(void *ptr)
{
  vPortFree( ptr );
}

/* -----------------------------------------------------------------------
   operator delete
   .......................................................................
   Description  : Our own delete operator, for array deletion

   Params : ptr - pointer to array

   Returns : Nothing
   ----------------------------------------------------------------------- */

void operator delete[](void *ptr)
{
  vPortFree( ptr );
}

/* -----------------------------------------------------------------------
   __cxa_pure_virtual
   .......................................................................
   Description  : Called if a pure virtual class is instantiated

   Params : None

   Returns : Doesn't return
   ----------------------------------------------------------------------- */

extern "C" void __cxa_pure_virtual() 
{ 
  taskDISABLE_INTERRUPTS();
  vLogGeneral( ERROR_SOFTWARE_CPP_PURE_VIRTUAL, 0, 0 );
}

/* -----------------------------------------------------------------------
   __cxa_guard_acquire
   .......................................................................
   Description  : ??

   Params : None

   Returns : int
   ----------------------------------------------------------------------- */

extern "C" int __cxa_guard_acquire( int *pllLock ) 
{
  UNUSED_VARIABLE( pllLock );
  return 0;
}

/* -----------------------------------------------------------------------
   __cxa_guard_release
   .......................................................................
   Description  : ??

   Params : None

   Returns : int
   ----------------------------------------------------------------------- */

extern "C" void __cxa_guard_release( int *pllLock ) 
{ 
  UNUSED_VARIABLE( pllLock );
}

