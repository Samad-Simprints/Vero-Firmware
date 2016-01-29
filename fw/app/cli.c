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
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

#define SET_DEBUG_HI

//******************************************************************************
// Includes
//******************************************************************************

#include <string.h>
#include <ctype.h>

#include "global.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cli.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

static void   vParserTask( void * pvParameters );
static bool   boParser(const char *pzPrompt, const tParserEntry *psCurrentLevel);
static bool   boParseString(const tParserEntry *psTablePtr, char **papacTokens, int iTokenCount);
static const  tParserEntry *psFindCommand(const char *pzCmdName, const tParserEntry *psLevelPtr);
static int    iGetString(const char *pzPrompt, char *pcReadBuffer, int iMaxNumBytes);
static bool   boHelpCmd(const tParserEntry *psEntry, char **papacArgv, int iArgc);
static void   vHelp(const tParserEntry *psEntry);
static void   vPrintHelp(const char *pzStr);
static void   vAddToHistory(char *pzInputLine);
static void   vPrintHistory( void );

//******************************************************************************
// Constants
//******************************************************************************

// *** MACRO DEFINITIONS
#define MAX_COMMANDS          (50)
#define MAX_TOKENS            (40)
#define MAX_HISTORY           (10)
#define ROOT_PROMPT           ("root")
#define HELP_CMD              ('?')
#define EXIT_CMD              ('.')
#define ROOT_CMD              ('/')

//******************************************************************************
// Definitions
//******************************************************************************

extern int kbhit( void );

static bool boDebugLevelCmd( char **papzArgs, int iInstance, int iNumArgs );

//******************************************************************************
// Local Storage
//******************************************************************************

// *** PRIVATE DATA DEFINITIONS
// Array where Root Commands are stored
static tParserEntry asRoot[MAX_COMMANDS];
// This is the Root where all Top level commands are added.
static tParserEntry sRoot = { NULL, "Root", "Root Level Commands Available", NULL, 0, 0, NULL, asRoot, 0};
// Holds the last executed commands for history
static char acHistoryList[MAX_HISTORY][MAX_COMMAND_LINE + 1];
// Index into the Array of Command History.  Points to the Next Index to Write to.
static int  iHistoryIndex = 0;
// Any array of all White Space possibilies
static char *pzWhiteSpace=" \n\r\t";

// Hold the current level for debuglevel command
static tParserEntry const *psCurrentLevelForDebug = NULL;

// DebugLevel command
tParserEntry sDebugLevelCmd[] =
{
  CLICMD("debuglevel", "Set Debug Level", -DBG_MAX_ZONES, "<debugzone> [<debugzone> ]", boDebugLevelCmd, 0)
};

// DebugLevel top-level command
tParserEntry sDebugTLC =
{
  0, "alm", "ALM commands", "", 1, 0, 0, sDebugLevelCmd, ELEMENTSOF(sDebugLevelCmd)
};

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
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// boDebugLevelCmd
//
// Description ; Implements support for debug zones
//
// Params : papzArgs - arguments passed to command
//          iInstance - instance number (unused)
//          iNumArgs - number of arguments actually parsed to command
//
// Returns : true if no error
//
//******************************************************************************
static bool boDebugLevelCmd( char **papzArgs, int iInstance, int iNumArgs )
{
  bool boRes = false;

  if ( psCurrentLevelForDebug != NULL )
  {
    sDebugSettings *psSettings = (sDebugSettings *)psCurrentLevelForDebug->psDebugSettings;
    if ( psSettings != NULL )
    {
      dword dwMask = psSettings->dwEnabledZones;
	  int iArg;
      for ( iArg = 1; iArg < iNumArgs; ++iArg )
      {
        int iArgLen = strlen( papzArgs[ iArg ] );
		int iZone;
        for ( iZone = 0; iZone < DBG_MAX_ZONES; ++iZone )
        {
          char const *pzZone = (char const *)psSettings->pszZoneDescriptions[ iZone ];
          if ( pzZone != 0 )
          {
            if ( ( strncasecmp( pzZone, papzArgs[ iArg ], iArgLen ) == 0 ) ||
                 ( strncasecmp( "all", papzArgs[ iArg ], iArgLen ) == 0 ) )
            {
              // found a match - update mask
              dwMask |= DEBUGZONE( iZone );
            }
            else
            if ( strncasecmp( "none", papzArgs[ iArg ], iArgLen ) == 0 )
            {
              // none seen, turn off all but mandatory zone
              dwMask = DEBUGZONE( ZONE_DBG_ALWAYS );
            }
            else
            if ( strncasecmp( "default", papzArgs[ iArg ], iArgLen ) == 0 )
            {
              // default seen, reset mask
              dwMask = psSettings->dwDefaultZones;
            }
          }
          else
          {
            break;
          }
        }
      }
      // write mask back
      psSettings->dwEnabledZones = dwMask;

      // display debug zones
      CLI_PRINT(( "Debug zones enabled for module %s:", psSettings->pszModuleName ));
      if ( dwMask != 0 )
      {
		int iZone;
        for ( iZone = 0; iZone < DBG_MAX_ZONES; ++iZone )
        {
          char const *pzZone = (char const *)psSettings->pszZoneDescriptions[ iZone ];
          if ( pzZone != 0 )
          {
            if ( ( dwMask & DEBUGZONE( iZone ) ) != 0 )
            {
              CLI_PRINT(( " %s", pzZone ));
            }
          }
          else
          {
            break;
          }
        }
      }
      else
      {
        CLI_PRINT(( " None" ));
      }
      CLI_PRINT(( "\n" ));
      boRes = true;
    }
  }
  else
  {
    CLI_PRINT(( "debuglevel command not applicable at this level\n" ));
  }

  // reset
  psCurrentLevelForDebug = NULL;

  return boRes;
}

//******************************************************************************
// boParser
//
// Description ; Parses a command string passed in, against the relative level
// in the command hierarchy.
//
// Params : pzPrompt - Prompt String, shows the level in the hierarchy.
//          psCurrentLevel - A pointer to the current active level.
//
// Returns : TRUE if an EXIT_CMD was received, otherwise FALSE.
//
//******************************************************************************
static bool boParser(const char *pzPrompt, const tParserEntry *psCurrentLevel)
{
  static char   zInputLine[MAX_COMMAND_LINE + 1];
  static char   zInputLineCopy[MAX_COMMAND_LINE + 1];
  static char  *apzTokens[MAX_TOKENS];
  static int    iTokenCount;
  static bool   boRootExitCommand = FALSE;
  int           iLen, iIndex;

  // Have we received a return to Root Command at some point?
  if (boRootExitCommand == TRUE)
  {
    // Is the current level the root?
    if (psCurrentLevel != &sRoot)
    {
      // No, Not at root yet, exit this level
      return TRUE;
    }
    else
    {
      // Yes, At Root, nothing to do
      boRootExitCommand = FALSE;
    }
  }

  CLI_PRINT(((char *)pzPrompt));
  CLI_PRINT(("> "));

  iLen = iGetString(pzPrompt, zInputLine, sizeof(zInputLine));
  DEBUG_LO(("boParser()-%s\n", zInputLine));

  PRECOND( (size_t)iLen < sizeof( zInputLine ) );
  PRECOND( zInputLine[iLen] == '\0' );
  PRECOND( (int)strlen(zInputLine) == iLen );

  // Convert Entire Input string to lower case.
  for(iIndex = 0; iIndex < iLen; iIndex++)
  {
    zInputLine[iIndex] = (char)tolower(zInputLine[iIndex]);
  }

  // Length 0, CR etc, Exit ignoring the string
  if ((zInputLine[0] == '\n') || (zInputLine[0] == '\r') || (iLen == 0))
  {
    return FALSE;
  }

  // Received exit current level command?
  if (zInputLine[0] == EXIT_CMD)
  {
    return TRUE;
  }

  // Received return to root command?
  if (zInputLine[0] == ROOT_CMD)
  {
    if (psCurrentLevel != &sRoot)
    {
      boRootExitCommand = TRUE;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }

  // Command History Checks ; Check for '!!' or '![0-9]' for last command repeat ...
  if (zInputLine[0] == '!')
  {
    // A single ! prints out the command history
    if (zInputLine[1] == 0)
    {
      vPrintHistory();
      return FALSE;
    }
    else if (zInputLine[1] == '!')
    {
      // Repeat last command, copy last saved command to history.
      strcpy(zInputLine, acHistoryList[iHistoryIndex - 1]);
    }
    else if ((zInputLine[1] >= '0') && (zInputLine[1] <= '9'))
    {
      // !x = Repeat Command [0-9]
      int iTemp = (int)(zInputLine[1] - '0');
      strcpy(zInputLine, acHistoryList[( ( iHistoryIndex - iTemp ) + MAX_HISTORY) % MAX_HISTORY]);
    }
    else
    {
      CLI_PRINT(("History either !, !! or ![0-9]\n"));
      return FALSE;
    }
  }

  // Strip the command and parameter(s) into individual strings for parsing.
  // strok() modifies the input string, so to preserve command history the input line is copied.
  strcpy(zInputLineCopy, zInputLine);
  iTokenCount = 0;
  apzTokens[iTokenCount] = strtok(zInputLineCopy, pzWhiteSpace);
  while(0 != apzTokens[iTokenCount])
  {
    DEBUG_LO(("boParser() Token[%d]-%s\n", iTokenCount, apzTokens[iTokenCount]));
    if (iTokenCount++ == MAX_TOKENS)
    {
      iTokenCount = 0;
      break;
    }
    apzTokens[iTokenCount] = strtok(NULL, pzWhiteSpace);
  }
  // If we have atleast a command to process.
  if (iTokenCount > 0)
  {
    if (FALSE == boParseString(psCurrentLevel, apzTokens, iTokenCount))
    {
      CLI_PRINT(("Unrecognised command, try ? for help\n"));
    }
    else
    {
      // Command Successful, Add to History list
      vAddToHistory(&zInputLine[0]);
    }
  }
  return FALSE;
}

//******************************************************************************
// vAddToHistory
//
// Description ; Adds the Passed Command to the History, if not the same as the
// last command, where it is ignored.
//
// Params : pzInputLine - String to add to the History.
//
// Returns : Nothing.
//
//******************************************************************************

static void vAddToHistory(char *pzInputLine)
{
  // Compare last entry with new entry
  if ( ( iHistoryIndex > 0 ) && ( 0 == strncmp(acHistoryList[iHistoryIndex - 1], pzInputLine, MAX_COMMAND_LINE) ) )
  {
    // Command same as last, don't add to history
  }
  else
  {
    // Not already in the list, add to top of list
    strcpy(acHistoryList[iHistoryIndex], pzInputLine);
    // Point to next index
    iHistoryIndex++;
    // Wrap around History Index if required
    iHistoryIndex = iHistoryIndex % MAX_HISTORY;
  }
}


//******************************************************************************
// vPrintHistory
//
// Description ; Prints the Current Command History to the Parser output.
//
// Params : None
//
// Returns : Nothing.
//
//******************************************************************************
static void vPrintHistory( void )
{
  int iLoop;

  // Print out the history list
  CLI_PRINT(("** Command History\n"));

  for(iLoop = 0; (iLoop < MAX_HISTORY); iLoop++)
  {
    // Put a Line Feed half way through
    if (iLoop == (MAX_HISTORY / 2))
    {
      CLI_PRINT(("\n"));
    }
    if (acHistoryList[( ( iHistoryIndex - iLoop ) + MAX_HISTORY) % MAX_HISTORY][0] != 0x00)
    {
      CLI_PRINT(("[%d]=%s  ", iLoop, acHistoryList[( ( iHistoryIndex - iLoop ) + MAX_HISTORY) % MAX_HISTORY]));
    }
  }
  CLI_PRINT(("\n"));
}

//******************************************************************************
// boParseString
//
// Description ; Parses an array of strings [Command then parameters] against
// the current level.  If it is a command this is executed.  If it is a level
// change, then it recursively calls boParser() with the next level structure.
// Will print help if no command can be executed.
//
// Params : psLevelPtr - Pointer to the current level
//          papzTokens - Array of pointer to string contains command and parameters
//          iTokenCount - Number of elements in the array.
//
// Returns : Returns FALSE if no command could be found, otherwise TRUE.
//
//******************************************************************************
static bool boParseString(const tParserEntry *psLevelPtr, char **papzTokens, int iTokenCount)
{
  bool boResult = FALSE;
  const tParserEntry  *psSelectedCommand = NULL;

  DEBUG_LO(("boParseString() papzTokens[0]=%s iTokenCount=%d\n", papzTokens[0], iTokenCount));

  PRECOND( psLevelPtr );

  // If it's a help request process at the current level
  if (*papzTokens[0] == HELP_CMD)
  {
    boResult = boHelpCmd(psLevelPtr, papzTokens, iTokenCount);
  }
  else
  {
    // Search for a command/level match in the current Level
    psSelectedCommand = psFindCommand(papzTokens[0], psLevelPtr);

    if (psSelectedCommand != NULL)
    {
      // We have found a Match.
      // Is Match a command then get the remainder of the line into tokens
      // or
      // A sub level, then parse the rest of the line against that sub menu.
      if (psSelectedCommand->iNumberInNextLevel > 0)
      {
        // Is Command a level change, or do we want to execute command in a sublevel
        if (iTokenCount == 1)
        {
          // No parameters, change to level by recursively calling the parser
          CLI_PRINT(( "Type '/' to return to Root, '.' to exit current level or '?' for local help\n" ));
          while (FALSE == boParser(psSelectedCommand->pzCmdName, psSelectedCommand))
          {
            // Do Nothing, in recursive call until it exits
          }
          boResult = TRUE;
        }
        else
        {
          // Recursively parse the string within the sublevel, taking 1 off the token list
          boResult = boParseString(psSelectedCommand, papzTokens + 1, iTokenCount - 1);
        }
      }
      else if (psSelectedCommand->rCmdCallback != NULL)
      {
        // Command performs callback if required number of arguments is correct.
        if ((psSelectedCommand->iNumArgs < 0) && (iTokenCount > -psSelectedCommand->iNumArgs))
        {
          // Not enough Optional arguments
          CLI_PRINT(("Too many Optional arguments, Max %d allowed\n", -psSelectedCommand->iNumArgs - 1));
          boResult = FALSE;
        }
        else if ((psSelectedCommand->iNumArgs > 0) && (iTokenCount != psSelectedCommand->iNumArgs))
        {
          // Not enough Mandatory Arguments
          CLI_PRINT(("Incorrect number of Mandatory arguments, %d are required\n", psSelectedCommand->iNumArgs - 1));
          boResult = FALSE;
        }
        else
        {
          // Execute callback, and print help if it returns FALSE
          boResult = (*psSelectedCommand->rCmdCallback)(papzTokens, psSelectedCommand->iInstance, iTokenCount);
        }
      }
      // If Callback not executed correctly (i.e. we have had insufficient arguments) then print help
      if (boResult == FALSE)
      {
        vHelp(psSelectedCommand);
        boResult = TRUE;
      }
    }
  }
  return boResult;
}

//******************************************************************************
// psFindCommand
//
// Description ; Searches for a given command at the passed level.
//
// Params : pzCmdName - User Cmd we are looking for.
//          psLevelPtr - Pointer to the current level for Find.
//
// Returns : Returns a pointer to the level entry that has been found to match.
//           Returns NULL if command did not match.
//
//******************************************************************************
static const tParserEntry *psFindCommand(const char *pzCmdName, const tParserEntry *psLevelPtr)
{
  int iEntry;
  int iLen;
  const tParserEntry *psPartialEntry = NULL;
  int iIndex;

  if ((NULL == pzCmdName) || (NULL == psLevelPtr))
  {
    return NULL;
  }

  iLen = strlen(pzCmdName);

  // Loop For every entry in the Current Level
  for (iEntry = 0; (iEntry < psLevelPtr->iNumberInNextLevel); iEntry++)
  {
    // Get the Registered command from the current Entry
    const tParserEntry *psNextLevelCommand = &psLevelPtr->psNextLevel[iEntry];

    // Compare Each Char in the Current Entry against the Command to be found.
    // Exit loop if mismatch found before the full length is compared.
    for (iIndex = 0; iIndex < iLen; iIndex++)
    {
      if (tolower(psNextLevelCommand->pzCmdName[iIndex]) != pzCmdName[iIndex])
      {
        break;
      }
    }

    // Have we matched all the chars in the Command to Find?
    if (iIndex == iLen)
    {
      // Was the last match performed the terminator and hence the full command?
      if (psNextLevelCommand->pzCmdName[iIndex] == '\0')
      {
        return psNextLevelCommand;
      }
      // If we have more than one partial match then return with no Match
      if (psPartialEntry != NULL)
      {
        return NULL;
      }
      // Save the current Partial Match, we will return this if we do not get another
      psPartialEntry = psNextLevelCommand;
    }

  }

  if ( ( NULL == psPartialEntry ) && ( psLevelPtr != &sDebugTLC ) && ( psLevelPtr != &sRoot ) )
  {
    // save the current top level block
    psCurrentLevelForDebug = psLevelPtr;
    // see if it is a debuglevel command
    psPartialEntry = psFindCommand( pzCmdName, &sDebugTLC );
  }

  // Either returns NULL or a single partial match.
  return psPartialEntry;
}

//******************************************************************************
// vParserTask
//
// Description ; Test Parser loop
//
// Params :  None
//
// Returns : Continuous parsing loop, will not return
//
//******************************************************************************
static portTASK_FUNCTION( vParserTask, pvParameters )
{
  UNUSED_VARIABLE( pvParameters );

  // Clean out the History
  for(iHistoryIndex = 0; iHistoryIndex < MAX_HISTORY; iHistoryIndex++)
  {
    acHistoryList[iHistoryIndex][0] = 0;
  }
  iHistoryIndex = 0;

  for (;;)
  {
    boParser(ROOT_PROMPT, &sRoot);
  }
}

//******************************************************************************
// iGetString
//
// Description ; Gets a string from the keyboard, and returns with the entered
// string when a CR/LF has been pressed.  Supports backspace.
//
// Params : pcReadBuffer - Pointer to buffer to store entered characters.
//          iMaxNumBytes -
//
// Returns : Returns the Number
//
//******************************************************************************
static int iGetString(const char *pzPrompt, char *pcReadBuffer, int iMaxNumBytes)
{
  int iCount = 0;
  char cChar;
  bool boExit;
  int iHistLine = iHistoryIndex;
  char *pzHistLine;

  UNUSED_VARIABLE( pzPrompt );
  PRECOND(iMaxNumBytes > 1);
  boExit = FALSE;

  do
  {
    cChar = getchar();

    switch(cChar)
    {
      case 255:
        vTaskDelay( MS_TO_TICKS( 100 ) );
        break;
      case '\0':
      case '\n':
      case '\r':
        // Carriage return, Line Feed or Null indicates the end of the line
        CLI_PRINT(("\n"));
        boExit = TRUE;
        break;

      case '\b':
      case 127:     // PuTTY returns this for a backspace
        // Backspace pressed, remove last character type
        if (iCount > 0)
        {
          iCount--;
          CLI_PRINT(("\b \b"));
        }
        break;

      case 27:
        // erase the previous command from the command line
        CLI_PRINT(( "\r%s %*s ",
                    (char *)pzPrompt,
                    strlen( pcReadBuffer ),
                    "" ));
        CLI_PRINT(( "\r%s> ", (char *)pzPrompt ));
        if ( kbhit() )
        {
          // handle VT100 escape codes
          if ( getchar() == 91 )
          {
            cChar = getchar();

            switch(cChar)
            {
              case 65:
                if ( acHistoryList[ ( MAX_HISTORY + iHistLine - 1 ) % MAX_HISTORY ][ 0 ] != '\0' )
                {
                  iHistLine--;
                }
                break;
              case 66:
                if ( acHistoryList[ ( iHistLine + 1 ) % MAX_HISTORY ][ 0 ] != '\0' )
                {
                  iHistLine++;
                }
                break;
            }
            pzHistLine = acHistoryList[ iHistLine ];
            CLI_PRINT(( "%s", pzHistLine ));
            strlcpy( pcReadBuffer, pzHistLine, iMaxNumBytes );
            iCount = strlen( pcReadBuffer );
          }
        }
        else
        {
          // clear the input buffer
          pcReadBuffer[ 0 ] = 0;
          iCount = 0;
        }
        break;

      default:
        // Test for a full buffer
        if ( iCount < (iMaxNumBytes - 1) )
        {
          // Add the character
          pcReadBuffer[iCount++] = cChar;
        }
        CLI_PRINT(("%c", cChar));
        break;
    }
  } while (FALSE == boExit);

  if (iCount < iMaxNumBytes)
  {
    // Line not too long. Terminate string with \0
    pcReadBuffer[iCount] = '\0';
  }
  else
  {
    // Line was too long. Return nothing
    iCount = 0;
    pcReadBuffer[iCount] = '\0';
  }

  DEBUG_LO(("\nString Received %s\n", pcReadBuffer));

  return iCount;
}

//******************************************************************************
// boHelpCmd
//
// Description ; Prints the help for a command and for its sub commands.
// If the Command is not found, list all available help.
//
// Params :  psEntry - Cmd structure for the Help string to print.
//           papacArgs - Array of Char Pointers, holding the cmd and arg list.
//           iNumArgs - Number of arguments
//
// Returns : TRUE, required for the callback
//
//******************************************************************************
static bool boHelpCmd(const tParserEntry *psEntry, char **papacArgs, int iNumArgs)
{
  const tParserEntry *psCommandPtr = NULL;

  // Try to find Help on a specific command if we have a cmd argument
  if (iNumArgs > 1)
  {
    psCommandPtr = psFindCommand(papacArgs[1], psEntry);
  }
  if (psCommandPtr != NULL)
  {
    // Command Found give command specific help
    vHelp(psCommandPtr);
  }
  else
  {
    vHelp(psEntry);
  }
  return TRUE;
}

//******************************************************************************
// vHelp
//
// Description ; Prints the help for the given command and for its sub commands
// Handles Indentation of Help String.
//
// Params :  psEntry - Cmd structure for the Help string to print.
//
// Returns : Nothing
//
//******************************************************************************
static void vHelp(const tParserEntry *psEntry)
{
  PRECOND(psEntry != NULL);

  DEBUG_LO(("vHelp()\n"));

  if (psEntry->pzHelpText == 0)
  {
    CLI_PRINT(("No Help Available\n"));
    return;
  }

  CLI_PRINT(("%-20s", psEntry->pzCmdName));

  if (psEntry->pzCmdArgs != 0)
  {
    CLI_PRINT(("[%s] ", psEntry->pzCmdArgs));
  }

  vPrintHelp(psEntry->pzHelpText);

  // Print out any entries in the next level, without Cmd Args
  if (psEntry->iNumberInNextLevel > 0)
  {
    int i;
    const tParserEntry *psCommand;
    for(i = 0; i < (psEntry->iNumberInNextLevel); i++)
    {
      psCommand = &psEntry->psNextLevel[i];
      CLI_PRINT(("    %-16s%", psCommand->pzCmdName));
      vPrintHelp(psCommand->pzHelpText);
    }
  }
}

//******************************************************************************
// vPrintHelp
//
// Description ; Prints the help string over multiple lines with indentation.
// Handles Indentation of Help String.
//
// Params :  pzHelpString - Help string to print.
//           Can contain multiple lines separated by a New Line char.
//
// Returns : Nothing
//
//******************************************************************************
static void vPrintHelp(const char *pzHelpString)
{
  char  zTempHelpBuf[MAX_HELP_BUFFER + 1];
  int   iLen;
  const char *pzStringStart;
  char *pzNewLine;

  // Break buffer into lines of max length and print them individually with indentation
  pzStringStart = pzHelpString;
  if (0 == *pzStringStart)
  {
    // Cope with commands which do not have help
    CLI_PRINT(("\n"));
  }

  // Loop until a Null Terminator is found
  while (0 != *pzStringStart)
  {
    if (pzStringStart != pzHelpString)
    {
      // Not the first line of help, insert indent
      CLI_PRINT(("%-20s", " "));
    }
    // Search for a newline char
    pzNewLine = strchr(pzStringStart, '\n');
    // Check if New Line found.
    if (0 != pzNewLine)
    {
    // If a New Line is found, Print out help string until the new line
      iLen = (int)(pzNewLine - pzStringStart) + 1;
      // Check if Length exceeds printable line length
      while (iLen > MAX_HELP_BUFFER)
      {
        // Too long for buffer.  Do a bit at a time
        strlcpy(zTempHelpBuf, pzStringStart, MAX_HELP_BUFFER);
        zTempHelpBuf[MAX_HELP_BUFFER] = 0;
        CLI_PRINT((zTempHelpBuf));
        pzStringStart = pzStringStart + MAX_HELP_BUFFER;
        iLen = iLen - MAX_HELP_BUFFER;
      }
      // Write the remaining string up to and including the \n
      strlcpy(zTempHelpBuf, pzStringStart, iLen);
      zTempHelpBuf[iLen] = 0;
      CLI_PRINT((zTempHelpBuf));
      // Advance the pointer to beyond the New Line
      pzStringStart = pzNewLine + 1;
    }
    else
    {
      // Help does not end with a New Line, print help string and exit
      CLI_PRINT(("%s\n", pzStringStart));
      break;
    }
  }
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// vCLIinit
//
// Description ; Initialises the CLI
//
// Params :  None
//
// Returns : Nothing
//
//******************************************************************************
void vCLIinit( void )
{
  // Create and Start a Task
  xTaskCreate( vParserTask,
               (const char *)"cli",
               CLI_TASK_STACK_SIZE,
               NULL,
               CLI_TASK_PRIORITY,
               NULL );
}

//******************************************************************************
// boCLIregisterEntry
//
// Description ; Register a Command Entry
//
// Params : psNewEntry - A pointer to a structure defining a new level or command.
//
// Returns : True if entry registered, false if already present or full.
//
//******************************************************************************
bool boCLIregisterEntry(const tParserEntry *psNewEntry)
{
  tParserEntry *psFreeCmdEntry;

  PRECOND(psNewEntry);

  if (sRoot.iNumberInNextLevel >= MAX_COMMANDS)
  {
    DEBUG_LO(("boRegisterEntry() Out of space in command table.\n"));
    return FALSE;
  }

  if (NULL != psFindCommand(psNewEntry->pzCmdName, &sRoot))
  {
    DEBUG_LO(("boRegisterEntry() Entry '%s' is Registered Twice.\n", psNewEntry->pzCmdName));
    return FALSE;
  }

  // Get next free Entry
  psFreeCmdEntry = (tParserEntry *) &sRoot.psNextLevel[sRoot.iNumberInNextLevel++];
  // Copy Entry to table.  Note sub entries are not copied, hence must be constant.
  *psFreeCmdEntry = *psNewEntry;

  return TRUE;
}

