/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_CHARACTERISTIC_H
#define __GATT_CHARACTERISTIC_H

#define GATT_HEARTRATE_FLAG_VALUE_FORMAT_UINT8             0x00
#define GATT_HEARTRATE_FLAG_VALUE_FORMAT_UINT16            0x01
#define GATT_HEARTRATE_FLAG_SENSOR_CONTACT_NOT_DETECTED    0x04
#define GATT_HEARTRATE_FLAG_SENSOR_CONTACT_DETECTED        0x06
#define GATT_HEARTRATE_FLAG_ENEGRY_EXPENDED_PRESENT        0x08
#define GATT_HEARTRATE_FLAG_RR_INTERVAL_PRESENT            0x10

#define GATT_HEARTRATE_SENSOR_LOCATION_OTHER       0x00
#define GATT_HEARTRATE_SENSOR_LOCATION_CHEST       0x01
#define GATT_HEARTRATE_SENSOR_LOCATION_WRIST       0x02
#define GATT_HEARTRATE_SENSOR_LOCATION_FINGER      0x03
#define GATT_HEARTRATE_SENSOR_LOCATION_HAND        0x04
#define GATT_HEARTRATE_SENSOR_LOCATION_EAR_LOBE    0x05
#define GATT_HEARTRATE_SENSOR_LOCATION_FOOT        0x06

#define GATT_HEARTRATE_CONTROL_RESET_ENEGRY_EXPENDED    0x01


#endif // __GATT_CHARACTERISTIC_H
