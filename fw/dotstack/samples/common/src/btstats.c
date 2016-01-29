/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/
#include "btapp.h"
#include "btstats.h"

#ifdef BTAPP_ENABLE_STATS

void btstats_init(void)
{
	btstats_pal_init();
}

void btstats_start(void)
{
	btstats_pal_start();
}

#endif // BTAPP_ENABLE_STATS
