/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include <assert.h>
#include <string.h>

#include "bttimer.h"
#include "bttask.h"
#include "btapp.h"
#include "task.h"

#define BTLOG_TAG "BTAPP"
#include "btlog_define.h"

#define MAX_COMMANDS      10

btapp_CommandHandler mCommandHandler = NULL;
btapp_Command        mCommands[MAX_COMMANDS];
btapp_Command*       mCurrentCommand = NULL;
bt_uint              mCommandQueueHead = 0;
bt_uint              mCommandQueueTail = 0;
bt_uint              mCommandQueueLen = 0;

static void appCmdSignalHandler(void);

void btapp_setCommandHandler(btapp_CommandHandler handler)
{
	mCommandHandler = handler;
	
	bttask_setAppCmdSignalHandler(&appCmdSignalHandler);

}

bt_bool btapp_execCommand(bt_uint cmd, const char* params[], bt_byte paramsCount, btapp_CommandCompletionHandler completionHandler)
{
	bt_uint commandQueueLen;

	taskENTER_CRITICAL();
	{
		commandQueueLen = mCommandQueueLen;
	}
	taskEXIT_CRITICAL();
	
	if (mCommandHandler && commandQueueLen < MAX_COMMANDS)
	{
		btapp_Command* command = &mCommands[mCommandQueueTail];
		
		command->cmd = cmd;
		command->params = params;
		command->paramsCount = paramsCount;
		command->completionHandler = completionHandler;
		
		if (++mCommandQueueTail == MAX_COMMANDS)
			mCommandQueueTail = 0;
		taskENTER_CRITICAL();
		{
			mCommandQueueLen++;
		}
		taskEXIT_CRITICAL();

		bttask_setSignal(BTTASK_SIG_CMD);
		
		return BT_TRUE;
	}
	else
	{
		return BT_FALSE;
	}
}

void btapp_completeCommand(bt_uint status)
{
	if (mCurrentCommand)
	{
		if (mCurrentCommand->completionHandler)
			mCurrentCommand->completionHandler(status, mCurrentCommand->cmd, mCurrentCommand->params, mCurrentCommand->paramsCount);
		
		mCurrentCommand = NULL;
		
		bttask_setSignal(BTTASK_SIG_CMD);
	}
}

static void execNextCommand(void)
{
	bt_uint commandQueueLen;

	taskENTER_CRITICAL();
	{
		commandQueueLen = mCommandQueueLen;
	}
	taskEXIT_CRITICAL();
	
	if (!mCurrentCommand && mCommandHandler && commandQueueLen)
	{
		mCurrentCommand = &mCommands[mCommandQueueHead];

		if (++mCommandQueueHead == MAX_COMMANDS)
			mCommandQueueHead = 0;
		taskENTER_CRITICAL();
		{
			mCommandQueueLen--;
		}
		taskEXIT_CRITICAL();
		
		mCommandHandler(mCurrentCommand->cmd, mCurrentCommand->params, mCurrentCommand->paramsCount);
	}
}

static void appCmdSignalHandler(void)
{
	execNextCommand();
}
