//******************************************************************************
//
// Project INDEX Application source file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       cli.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Definitions of core structures and methods for CLI framework
//
//******************************************************************************

#if !defined(_CLI_H_)
#define _CLI_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************

#define MAX_COMMAND_LINE              ( 80 )
#define MAX_HELP_BUFFER               ( 108 )

#define CLI_PRINT( str )              printf str

//******************************************************************************
// Definitions
//******************************************************************************

typedef bool (*boCmdFunction)(char **papzArgs, int iInstance, int iNumArgs);

// Structure Definition of the Basic Parser entry.  Allows for a layered hierarchies to be created.
// Can be used to define a Sub Level, or a Command in a level.
typedef struct tParserEntry
{
  // Link to debuginfo (only for top level structures)
  sDebugSettings             *psDebugSettings;
  // Registered Command or Sub Level Name, i.e. memtest1 or memtests, All commands SHOULD be in lower case
  const char                 *pzCmdName;
  // Descriptive text of what the test does or what is contained in the Sub Level.
  const char                 *pzHelpText;
  // Descriptive Text of the Parameter Arguments (only required for Commands).
  const char                 *pzCmdArgs;
  // Number of Arguments INCLUDING command.  A -ve number means arguments are optional.
  int                         iNumArgs;
  // The instance allows the same callback to be used with this ident.
  int                         iInstance;
  // Function pointer to the callback function (only required for Commands).
  boCmdFunction               rCmdCallback;
  // Pointer to a structure of this type (used when defining a sub level).
  const struct tParserEntry  *psNextLevel;
  // Number of entries in the sublevel (used when defining a sub level).
  int                         iNumberInNextLevel;
} tParserEntry;

#define CLICMD(name, help, argc, argh, func, inst)  \
{ \
  0, name, help, argh, argc, inst, func, 0, 0  \
}

#define CLICMD_TOPLEVEL(name, help, argc, argh, func, inst, sublevel)  \
{ \
  0, name, help, argh, argc, inst, func, sublevel, ELEMENTSOF(sublevel)  \
}


//******************************************************************************
// Function Prototypes
//******************************************************************************

extern void vCLIinit( void );
extern bool boCLIregisterEntry( const tParserEntry *psNewEntry );
extern void vSetErrorCallbackFn( void (*pfnErrorCallbackArg)() );

#ifdef __cplusplus
}
#endif

#endif  // <HEADER_DEFINE>
