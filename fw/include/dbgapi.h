//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2009, 2010. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       dbgapi.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Project-wide debug macros
//
//******************************************************************************

#if !defined(_DBGAPI_H_)
#define _DBGAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Includes
//******************************************************************************


//******************************************************************************
// Definitions
//******************************************************************************

// Maximum number of zones the user can define
#define DBG_MAX_ZONES                     ( 30 )

// A debug zone which will always output
#define ZONE_DBG_ALWAYS                   ( 31 )

#define DEBUGZONE( _zone_ )               ( 0x00000001ul << ( ( _zone_)) )

#define DEBUG_MODULE_DEFINE( _MODULE_ )   sDebugSettings dbg##_MODULE_

#define DEBUG_MODULE_REF( _MODULE_ )      ( (sDebugSettings*)&dbg##_MODULE_ )

#define DEBUG_MODULE_INIT( _MODULE_ )     ( dbg##_MODULE_ .dwEnabledZones = ( dbg##_MODULE_ .dwDefaultZones | DEBUGZONE( ZONE_DBG_ALWAYS ) ) )

#define DEBUG_MODULE_USE(_MODULE_)        \
  extern sDebugSettings dbg##_MODULE_; \
  static const sDebugSettings *modDbg = &dbg##_MODULE_;

// structure that describes and controls the operation of a modules debug zones
typedef struct sDebugSettingsTag
{
  const char *pszModuleName;
  const char *pszZoneDescriptions[ DBG_MAX_ZONES ];
  const dword dwDefaultZones;
  dword dwEnabledZones;
} sDebugSettings;

enum
{
  ZONE_DBG_ERROR,
  ZONE_DBG_HI,
  ZONE_DBG_MED,
  ZONE_DBG_LO,
  ZONE_DBG_ALL = ( 0xffffffff >> (32 - DBG_MAX_ZONES ) )
};


// DBG_BUILD_LEVEL defines the highest zone number that will be output
#if !defined ( DBG_BUILD_LEVEL )
#define DBG_BUILD_LEVEL             ( DBG_MAX_ZONES + 1 )
#endif

extern int dbg_dump( const char *pzFmt, const char *pzBuffer, const int iLength );

// Sometimes we want release builds that contain debug so DBG_SHIP controls the removal of messages
#if !defined( DBG_SHIP )
#define DEBUGMSG(zone, printf_exp) \
      (void)( ( ( (zone) <= DBG_BUILD_LEVEL ) && \
                ( modDbg->dwEnabledZones & DEBUGZONE( zone ) ) ) ? (printf printf_exp) : 0)

#define DEBUGDATA(zone, dbg_dump_exp) \
      (void)( ( ( (zone) <= DBG_BUILD_LEVEL ) && \
                ( modDbg->dwEnabledZones & DEBUGZONE( zone ) ) ) ? (dbg_dump dbg_dump_exp) : 0)

#else
#define DEBUGMSG(zone, printf_exp)    ((void)0)
#define DEBUGDATA(zone, dbg_dump_exp) ((void)0)
#endif

// until we have moved to the new scheme we provide a set of defaults that work as before (see main.cpp)
#if 0
DEBUG_MODULE_DEFINE( CM_DEFAULTS ) = {
    "CM_DEFAULT", {
    "Error","Hi","Medium","Low" },
    DEBUGZONE(ZONE_DBG_ERROR) | DEBUGZONE(ZONE_DBG_HI) | DEBUGZONE(ZONE_DBG_MED) | DEBUGZONE(ZONE_DBG_LO)
};
#endif

#if !defined( DBG_LOCAL_DEFS )
DEBUG_MODULE_USE( INDEX_DEFAULTS )
#endif  // DBG_LOCAL_DEFS

// legacy debug out functions

//  DEBUG_ERROR() An error message.
//  DEBUG_HI()    A condition requiring special handling.
//  DEBUG_MED()   A general information message.
//  DEBUG_LO()    A message useful for debugging programs.
#ifdef DEBUG_OUTPUT
#define DEBUG_OUT( str )              DEBUGMSG( ZONE_DBG_ALWAYS, str )
#else
#define DEBUG_OUT( str )
#endif

#ifdef SET_DEBUG_HI
  #define DEBUG_ERROR(str)            DEBUGMSG( ZONE_DBG_ERROR, str )
  #define DEBUG_HI(str)               DEBUGMSG( ZONE_DBG_HI, str )
  #define DEBUG_MED(str)
  #define DEBUG_LO(str)
#else
#ifdef SET_DEBUG_MED
  #define DEBUG_ERROR(str)            DEBUGMSG( ZONE_DBG_ERROR, str )
  #define DEBUG_HI(str)               DEBUGMSG( ZONE_DBG_HI, str )
  #define DEBUG_MED(str)              DEBUGMSG( ZONE_DBG_MED, str )
  #define DEBUG_LO(str)
#else
#ifdef SET_DEBUG_ALL
  #define DEBUG_ERROR(str)            DEBUGMSG( ZONE_DBG_ERROR, str )
  #define DEBUG_HI(str)               DEBUGMSG( ZONE_DBG_HI, str )
  #define DEBUG_MED(str)              DEBUGMSG( ZONE_DBG_MED, str )
  #define DEBUG_LO(str)               DEBUGMSG( ZONE_DBG_LO, str )
#else
  #define DEBUG_ERROR(str)            DEBUGMSG( ZONE_DBG_ERROR, str )
  #define DEBUG_HI(str)
  #define DEBUG_MED(str)
  #define DEBUG_LO(str)
#endif
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
