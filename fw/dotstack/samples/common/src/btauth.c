/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"

//
// Bluetooth PIN code
// -------------------------------------------------------------------
//
void bt_oem_get_pin_code(bt_bdaddr_t* bdaddr_remote)
{
	bt_hci_send_pin_code(bdaddr_remote, "0000");
}

//
// Bluetooth SSP handler
// -------------------------------------------------------------------
//
void bt_oem_ssp_callback(SPP_EVENT spp_event, void* event_param, void* init_param)
{
	switch (spp_event)
	{
		case SSP_EVENT_USER_PASSKEY_NOTIFICATION:
			{
//				bt_ssp_user_passkey_notification* upk = (bt_ssp_user_passkey_notification*)event_param;
//				const char* s = _ulong2str_dec(upk->passkey);

				// depending on the input capabilities of the remote device
				// the user will now have to either enter or confirm the number
				// displayed on this device.

			}
			break;

		case SSP_EVENT_SIMPLE_PAIRING_COMPLETE:
			{
				bt_ssp_simple_pairing_complete* spc = (bt_ssp_simple_pairing_complete*)event_param;

				if (spc->status != HCI_ERR_SUCCESS)
				{
					// authentication failed.
				}
				else
				{
					// all is good. connection set up will proceed.
				}
			}
			break;

		case SSP_EVENT_USER_PASSKEY_REQUEST:
			{
				//bt_ssp_user_passkey_request* user_passkey = (bt_ssp_user_passkey_request*)event_param;

				//mPairingDeviceAddress = user_passkey->bdaddr_remote;

				// implement logic to request a user to enter a number displayed on the other device,
				// read user's input, convert it to bt_ulong and send to the controller.
				//
				// if user confirms input the execute the following code:
				//
				//     bt_ssp_user_passkey_request upkr;
				//
				//     upkr.bdaddr_remote = <value received with the SSP_EVENT_USER_PASSKEY_REQUEST event>;
				//     upkr.passkey = <number entered by the user>;
				//
				//     bt_ssp_send_user_passkey(
				//         HCI_ERR_SUCCESS, &upkr, NULL);
				//
				//  if user cancels input then execute the following code:
				//
				//     bt_ssp_user_passkey_request upkr;
				//
				//     upkr.bdaddr_remote = <value received with the SSP_EVENT_USER_PASSKEY_REQUEST event>;
				//
				//     bt_ssp_send_user_passkey(
				//         HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED, &upkr, NULL);

			}
			break;

		case SSP_EVENT_USER_CONFIRMATION_REQUEST:
			{
				bt_ssp_user_confirmation_request* user_confirmation = (bt_ssp_user_confirmation_request*)event_param;
//				const char* s = _ulong2str_dec(user_confirmation->numeric_value);

				// implement logic to read the user's answer and send it to the controller.
				// if user confirms that the passkey displayed on this device is the same
				// as the one on the remote device then exeute the following code
				//    bt_ssp_user_confirmation_request user_confirmation;
				//
				//    user_confirmation.bdaddr_remote = <value received with the SSP_EVENT_USER_CONFIRMATION_REQUEST event>;
				//    bt_ssp_send_user_confirmation(
				//        HCI_ERR_SUCCESS, &user_confirmation, NULL);
				//
				// otherwise execute the following code:
				//    bt_ssp_user_confirmation_request user_confirmation;
				//
				//    user_confirmation.bdaddr_remote = <value received with the SSP_EVENT_USER_CONFIRMATION_REQUEST event>;
				//    bt_ssp_send_user_confirmation(
				//        HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED, &user_confirmation, NULL);
				//
				// see bttask_onButtonStateChanged for an example

//				mPairingDeviceAddress = user_confirmation->bdaddr_remote;
				bt_ssp_send_user_confirmation(HCI_ERR_SUCCESS, user_confirmation, NULL);

			}
			break;

		case SSP_EVENT_OOB_DATA_REQUEST:
			{
				//bt_ssp_oob_data* oob_data = (bt_ssp_oob_data*)event_param;
				//
				// lookup OOB data for the device identified by the bdaddr member of the
				// bt_ssp_oob_data structure. if OOB data is present then fill the hash and randomizer
				// members and call bt_ssp_set_oob_data with success.
				//    bt_ssp_set_oob_data(HCI_SSP_MGR, HCI_ERR_SUCCESS, oob_data, NULL);
				//
				// if OOB data is not present call bt_ssp_set_oob_data with error code HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED
				//    bt_ssp_set_oob_data(HCI_SSP_MGR, HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED, oob_data, NULL);

			}
			break;

		case SSP_EVENT_IO_CAPABILITY_REQUEST:
			{
				bt_ssp_io_capability* io_caps = (bt_ssp_io_capability*)event_param;

				// the demo board has only a display and an ability to send either yes or no answer.
				// the SSP_EVENT_USER_PASSKEY_REQUEST will never be sent to us as there is no way
				// to enter digits.
				//io_caps->io_capability = SSP_IO_CAPABILITY_DISPLAY_YESNO;

				io_caps->io_capability = SSP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT;

				// we do not have OOB data for any remote devices.
				// the SSP_EVENT_OOB_DATA_REQUEST will never be sent to us.
				io_caps->oob_data_present = SSP_OOB_DATA_NOT_PRESENT;

				io_caps->authentication_requirements = SSP_MITM_NOT_REQUIRED_GENERAL_BONDING;

				bt_ssp_set_io_capabilities(HCI_ERR_SUCCESS, io_caps, NULL);
			}
			break;

		default:
			break;
	}
}
