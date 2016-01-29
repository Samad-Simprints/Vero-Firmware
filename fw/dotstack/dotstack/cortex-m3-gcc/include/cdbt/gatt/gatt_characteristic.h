/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_CHARACTERISTIC_H
#define __GATT_CHARACTERISTIC_H

#define GATT_CHAR_UUID_ALERT_CATEGORY_ID                                   0x2A43
#define GATT_CHAR_UUID_ALERT_CATEGORY_ID_BITMASK                           0x2A42
#define GATT_CHAR_UUID_ALERT_LEVEL                                         0x2A06
#define GATT_CHAR_UUID_ALERT_NOTIFICATION_CONTROL_POINT                    0x2A44
#define GATT_CHAR_UUID_ALERT_STATUS                                        0x2A3F
#define GATT_CHAR_UUID_APPEARANCE                                          0x2A01
#define GATT_CHAR_UUID_BATTERY_LEVEL                                       0x2A19
#define GATT_CHAR_UUID_BLOOD_PRESSURE_FEATURE                              0x2A49
#define GATT_CHAR_UUID_BLOOD_PRESSURE_MEASUREMENT                          0x2A35
#define GATT_CHAR_UUID_BODY_SENSOR_LOCATION                                0x2A38
#define GATT_CHAR_UUID_BOOT_KEYBOARD_INPUT_REPORT                          0x2A22
#define GATT_CHAR_UUID_BOOT_KEYBOARD_OUTPUT_REPORT                         0x2A32
#define GATT_CHAR_UUID_BOOT_MOUSE_INPUT_REPORT                             0x2A33
#define GATT_CHAR_UUID_CURRENT_TIME                                        0x2A2B
#define GATT_CHAR_UUID_DATE_TIME                                           0x2A08
#define GATT_CHAR_UUID_DAY_DATE_TIME                                       0x2A0A
#define GATT_CHAR_UUID_DAY_OF_WEEK                                         0x2A09
#define GATT_CHAR_UUID_DEVICE_NAME                                         0x2A00
#define GATT_CHAR_UUID_DST_OFFSET                                          0x2A0D
#define GATT_CHAR_UUID_EXACT_TIME_256                                      0x2A0C
#define GATT_CHAR_UUID_FIRMWARE_REVISION_STRING                            0x2A26
#define GATT_CHAR_UUID_HARDWARE_REVISION_STRING                            0x2A27
#define GATT_CHAR_UUID_HEART_RATE_CONTROL_POINT                            0x2A39
#define GATT_CHAR_UUID_HEART_RATE_MEASUREMENT                              0x2A37
#define GATT_CHAR_UUID_HID_CONTROL_POINT                                   0x2A4C
#define GATT_CHAR_UUID_HID_INFORMATION                                     0x2A4A
#define GATT_CHAR_UUID_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST 0x2A2A
#define GATT_CHAR_UUID_INTERMEDIATE_CUFF_PRESSURE                          0x2A36
#define GATT_CHAR_UUID_INTERMEDIATE_TEMPERATURE                            0x2A1E
#define GATT_CHAR_UUID_LOCAL_TIME_INFORMATION                              0x2A0F
#define GATT_CHAR_UUID_MANUFACTURER_NAME_STRING                            0x2A29
#define GATT_CHAR_UUID_MEASUREMENT_INTERVAL                                0x2A21
#define GATT_CHAR_UUID_MODEL_NUMBER_STRING                                 0x2A24
#define GATT_CHAR_UUID_NEW_ALERT                                           0x2A46
#define GATT_CHAR_UUID_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS          0x2A04
#define GATT_CHAR_UUID_PERIPHERAL_PRIVACY_FLAG                             0x2A02
#define GATT_CHAR_UUID_PNP_ID                                              0x2A50
#define GATT_CHAR_UUID_PROTOCOL_MODE                                       0x2A4E
#define GATT_CHAR_UUID_RECONNECTION_ADDRESS                                0x2A03
#define GATT_CHAR_UUID_REFERENCE_TIME_INFORMATION                          0x2A14
#define GATT_CHAR_UUID_REPORT                                              0x2A4D
#define GATT_CHAR_UUID_REPORT_MAP                                          0x2A4B
#define GATT_CHAR_UUID_RINGER_CONTROL_POINT                                0x2A40
#define GATT_CHAR_UUID_RINGER_SETTING                                      0x2A41
#define GATT_CHAR_UUID_SCAN_INTERVAL_WINDOW                                0x2A4F
#define GATT_CHAR_UUID_SCAN_REFRESH                                        0x2A31
#define GATT_CHAR_UUID_SERIAL_NUMBER_STRING                                0x2A25
#define GATT_CHAR_UUID_SERVICE_CHANGED                                     0x2A05
#define GATT_CHAR_UUID_SOFTWARE_REVISION_STRING                            0x2A28
#define GATT_CHAR_UUID_SUPPORTED_NEW_ALERT_CATEGORY                        0x2A47
#define GATT_CHAR_UUID_SUPPORTED_UNREAD_ALERT_CATEGORY                     0x2A48
#define GATT_CHAR_UUID_SYSTEM_ID                                           0x2A23
#define GATT_CHAR_UUID_TEMPERATURE_MEASUREMENT                             0x2A1C
#define GATT_CHAR_UUID_TEMPERATURE_TYPE                                    0x2A1D
#define GATT_CHAR_UUID_TIME_ACCURACY                                       0x2A12
#define GATT_CHAR_UUID_TIME_SOURCE                                         0x2A13
#define GATT_CHAR_UUID_TIME_UPDATE_CONTROL_POINT                           0x2A16
#define GATT_CHAR_UUID_TIME_UPDATE_STATE                                   0x2A17
#define GATT_CHAR_UUID_TIME_WITH_DST                                       0x2A11
#define GATT_CHAR_UUID_TIME_ZONE                                           0x2A0E
#define GATT_CHAR_UUID_TX_POWER_LEVEL                                      0x2A07
#define GATT_CHAR_UUID_UNREAD_ALERT_STATUS                                 0x2A45

#define GATT_CHAR_DESC_EXTENDED_PROPERTIES                                 0x2900
#define GATT_CHAR_DESC_USER_DESCRIPTION                                    0x2901
#define GATT_CHAR_DESC_CLIENT_CONFIGURATION                                0x2902
#define GATT_CHAR_DESC_SERVER_CONFIGURATION                                0x2903
#define GATT_CHAR_DESC_PRESENTATION_FORMAT                                 0x2904
#define GATT_CHAR_DESC_AGREAGATE_PRESENTATION_FORMAT                       0x2905

#endif // __GATT_CHARACTERISTIC_H
