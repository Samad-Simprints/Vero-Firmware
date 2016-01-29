/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_H
#define __GATT_H

/**
 * \defgroup gatt GATT
 *
 * The GATT profile is designed to be used by an application or another profile, so
 * that a client can communicate with a server. The server contains a number of
 * attributes, and the GATT Profile defines how to use the Attribute Protocol to
 * discover, read, write and obtain indications of these attributes, as well as configuring
 * broadcast of attributes.

 * The Generic Attribute Profile (GATT) defines a service framework using the
 * Attribute Protocol. This framework defines procedures and formats of services
 * and their characteristics. The procedures defined include discovering, reading,
 * writing, notifying and indicating characteristics, as well as configuring the
 * broadcast of characteristics.
 *
 * \defgroup gatt_server GATT Server
 * \ingroup gatt
 * 
 *  This module describes functions and data structures used to implement a GATT server (peripheral).
*/

#include "cdbt/gatt/gatt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

bt_bool bt_gatt_init(void);

#include "cdbt/gatt/gatt_service.h"
#include "cdbt/gatt/gatt_characteristic.h"
#include "cdbt/gatt/gatt_private.h"

#ifdef __cplusplus
}
#endif

#endif // __GATT_H
