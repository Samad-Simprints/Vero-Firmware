/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_COMMON_H
#define __GATT_COMMON_H

#define GATT_ATTR_TYPE_SERVICE_TYPE_PRIMARY             0x2800
#define GATT_ATTR_TYPE_SERVICE_TYPE_SECONDARY           0x2801
#define GATT_ATTR_TYPE_INCLUDE                          0x2802
#define GATT_ATTR_TYPE_CHARACTERISTIC                   0x2803
#define GATT_ATTR_TYPE_EXTENDED_PROPERTIES              0x2900
#define GATT_ATTR_TYPE_USER_DESCRIPTION                 0x2901
#define GATT_ATTR_TYPE_CLIENT_CONFIGURATION             0x2902
#define GATT_ATTR_TYPE_SERVER_CONFIGURATION             0x2903
#define GATT_ATTR_TYPE_PRESENTATION_FORMAT              0x2904
#define GATT_ATTR_TYPE_AGREAGATE_PRESENTATION_FORMAT    0x2905

#define GATT_CHAR_CLIENT_CONFIG_NOTIFY      1
#define GATT_CHAR_CLIENT_CONFIG_INDICATE    2

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // __GATT_COMMON_H
