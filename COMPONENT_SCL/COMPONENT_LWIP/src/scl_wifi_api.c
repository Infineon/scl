/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "scl_wifi_api.h"
#include "scl_ipc.h"
#include "scl_types.h"
#include "string.h"
#include "scl_buffer_api.h"
/******************************************************
 *        Variables Definitions
 *****************************************************/

typedef struct {
    scl_mac_t *mac;
    uint32_t retval;
} scl_mac;

typedef struct {
    scl_scan_type_t scan_type;
    scl_bss_type_t bss_type;
    const scl_ssid_t *optional_ssid;
    const scl_mac_t *optional_mac;
    const uint16_t *optional_channel_list;
    const scl_scan_extended_params_t *optional_extended_params;
    scl_scan_result_t *result_ptr;
    void *user_data;
} scl_scan_parameters_for_np_t;

typedef struct {
    uint32_t retval;
    scl_wl_bss_info_t* bss_info;
} scl_bss_info_t;

typedef struct {
    uint32_t ioctl;
    uint32_t value;
} scl_ioctl_value_t;

typedef struct {
    const scl_ssid_t *ssid;
    scl_nsapi_security_t auth_type;
    const uint8_t *security_key;
    uint8_t key_length;
} scl_network_credentials_t;


typedef struct {
    scl_bool_t event_set;
    scl_event_num_t events[SCL_MAX_EVENT_SUBSCRIPTION];
    scl_event_handler_t handler;
    void *handler_user_data;
    uint8_t ifidx;
} event_list_elem_t;

event_list_elem_t scl_event_list[SCL_EVENT_HANDLER_LIST_SIZE];

static scl_scan_result_callback_t scan_callback;
static scl_scan_result_t *g_result_ptr;
static uint8_t *g_ie_ptr;
static void *g_user_data;
/******************************************************
 *               Function Definitions
 ******************************************************/

static scl_nsapi_security_t scl_to_nsapi_security(scl_security_t sec)
{
    switch (sec) {
        case SCL_SECURITY_OPEN:
            return SCL_NSAPI_SECURITY_NONE;
        case SCL_SECURITY_WEP_PSK:
        case SCL_SECURITY_WEP_SHARED:
            return SCL_NSAPI_SECURITY_WEP;
        case SCL_SECURITY_WPA_TKIP_PSK:
        case SCL_SECURITY_WPA_TKIP_ENT:
        case SCL_SECURITY_WPA_AES_ENT:
        case SCL_SECURITY_WPA_MIXED_ENT:
            return SCL_NSAPI_SECURITY_WPA;
        case SCL_SECURITY_WPA2_MIXED_PSK:
        case SCL_SECURITY_WPA2_WPA_PSK:
        case SCL_SECURITY_WPA2_WPA_TKIP_PSK:
            return SCL_NSAPI_SECURITY_WPA_WPA2;
        case SCL_SECURITY_WPA2_MIXED_ENT:
            return SCL_NSAPI_SECURITY_WPA2_ENT;
        case SCL_SECURITY_WPA2_AES_PSK:
        case SCL_SECURITY_WPA2_AES_ENT:
        case SCL_SECURITY_WPA2_FBT_PSK:
        case SCL_SECURITY_WPA2_FBT_ENT:
        case SCL_SECURITY_WPA2_TKIP_ENT:
            return SCL_NSAPI_SECURITY_WPA2;

        default:
            return SCL_NSAPI_SECURITY_UNKNOWN;
    }
}

scl_result_t scl_wifi_is_ready_to_transceive(void)
{
    scl_result_t result = SCL_SUCCESS;
    scl_result_t retval = SCL_SUCCESS;

    result = scl_send_data(SCL_TX_TRANSCEIVE_READY, (char *)&retval, TIMER_DEFAULT_VALUE);
    if (result == SCL_ERROR) {
        SCL_LOG(("Ready to tranceive error\r\n"));
        return SCL_ERROR;
    } else {
        return retval;
    }
}

bool scl_wifi_on(void)
{
    bool retval = false;
    scl_result_t result = SCL_SUCCESS;
    result = scl_send_data(SCL_TX_WIFI_ON, (char *)&retval, WIFI_ON_TIMEOUT);
    if (result == SCL_ERROR) {
        SCL_LOG(("wifi_on Error\r\n"));
        return false;
    } else {
        return retval;
    }
}

scl_result_t scl_wifi_set_up(void)
{
    scl_result_t retval = SCL_SUCCESS;
    scl_result_t result = SCL_SUCCESS;
    result = scl_send_data(SCL_TX_WIFI_SET_UP, (char *)&retval, TIMER_DEFAULT_VALUE);
    if (result == SCL_SUCCESS) {
        return retval;
    } else {
        SCL_LOG(("Wifi set up error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_get_mac_address(scl_mac_t *mac)
{
    scl_mac scl_mac_data;
    scl_result_t scl_retval = SCL_SUCCESS;
    scl_mac_data.mac = mac;
    scl_mac_data.retval = SCL_SUCCESS;

    if (mac == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_GET_MAC, (char *)&scl_mac_data, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return scl_mac_data.retval;
    } else {
        SCL_LOG(("Get MAC address error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_get_bssid(scl_mac_t *bssid)
{
    struct scl_bssid {
        scl_mac_t *bssid;
        uint32_t retval;
    } scl_bssid_t;
    scl_result_t scl_retval = SCL_SUCCESS;
    scl_bssid_t.bssid = bssid;
    scl_bssid_t.retval = SCL_SUCCESS;
    if (bssid == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_WIFI_GET_BSSID, (char *)&scl_bssid_t, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return scl_bssid_t.retval;
    } else {
        SCL_LOG(("get bssid error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_register_multicast_address(scl_mac_t *mac)
{
    scl_mac scl_mac_t;
    scl_mac_t.mac = mac;
    scl_mac_t.retval = SCL_SUCCESS;
    scl_result_t scl_retval = SCL_SUCCESS;
    if (mac == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_REGISTER_MULTICAST_ADDRESS, (char *)&scl_mac_t, TIMER_DEFAULT_VALUE);
    if (scl_retval != SCL_SUCCESS) {
        SCL_LOG(("Register Multicast Address IPC Error\r\n"));
        return SCL_ERROR;
    }
    return (scl_mac_t.retval);
}

scl_result_t scl_network_send_ethernet_data(scl_tx_buf_t scl_buffer)
{
    scl_result_t retval = SCL_SUCCESS;
    if (scl_buffer.buffer == NULL) {
        return SCL_BADARG;
    }
    retval = scl_send_data(SCL_TX_SEND_OUT, (char *)&scl_buffer, TIMER_DEFAULT_VALUE);
    return retval;
}

scl_result_t scl_wifi_get_rssi(int32_t *rssi)
{
    struct tx_param {
        uint32_t retval;
        int32_t *get_rssi;
    } tx_param_t;
    scl_result_t scl_retval = SCL_SUCCESS;

    if (rssi == NULL) {
        return SCL_BADARG;
    }
    tx_param_t.get_rssi = rssi;
    scl_retval = scl_send_data(SCL_TX_WIFI_GET_RSSI, (char *) &tx_param_t, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return tx_param_t.retval;
    } else {
        SCL_LOG(("get rssi error\r\n"));
        return SCL_ERROR;
    }
}

/*
 * NOTE: search references of function wlu_get in wl/exe/wlu.c to find what format the returned IOCTL data is.
 */
uint32_t scl_wifi_scan(scl_scan_type_t scan_type,
                       scl_bss_type_t bss_type,
                       const scl_ssid_t *optional_ssid,
                       const scl_mac_t *optional_mac,
                       const uint16_t *optional_channel_list,
                       const scl_scan_extended_params_t *optional_extended_params,
                       scl_scan_result_callback_t callback,
                       scl_scan_result_t *result_ptr,
                       void *user_data
                       )
{
    scl_scan_parameters_for_np_t scl_scan_parameters_for_np;
    scl_result_t retval = SCL_SUCCESS;
    /* fill the scan parameters to a structure and send it to NP */
    scl_scan_parameters_for_np.scan_type = scan_type;
    scl_scan_parameters_for_np.bss_type = bss_type;
    scl_scan_parameters_for_np.optional_ssid = optional_ssid;
    scl_scan_parameters_for_np.optional_mac = optional_mac;
    scl_scan_parameters_for_np.optional_channel_list = optional_channel_list;
    scl_scan_parameters_for_np.optional_extended_params = optional_extended_params;
    scl_scan_parameters_for_np.result_ptr = result_ptr;
    scl_scan_parameters_for_np.user_data = user_data;
    /* callback to be used when there is a scan result from CP */
    g_result_ptr = result_ptr;
    g_user_data = user_data;
    scan_callback = callback;
    /* send scan parameters to NP*/
    retval = scl_send_data(SCL_TX_SCAN, (char *)&scl_scan_parameters_for_np, TIMER_DEFAULT_VALUE);
    return retval;
}

void scl_wifi_scan_callback(scl_scan_status_t status) {
    if (status == SCL_SCAN_COMPLETED_SUCCESSFULLY) {
        scl_buffer_release(g_ie_ptr, SCL_NETWORK_RX);
    }
    else {
        g_ie_ptr = (g_result_ptr)->ie_ptr;
    }
    if (g_result_ptr != NULL && scan_callback != NULL)
        scan_callback(g_result_ptr,g_user_data,status);
    else {
        SCL_LOG(("scan callback not registered\n"));
    }
    
}
uint32_t scl_wifi_get_bss_info(scl_wl_bss_info_t *bi) {
    scl_result_t retval = SCL_SUCCESS;
    scl_bss_info_t scl_bss_info;
    scl_bss_info.bss_info = bi;
    retval = scl_send_data(SCL_TX_GET_BSS_INFO, (char *)&scl_bss_info, TIMER_DEFAULT_VALUE);
    if (retval == SCL_SUCCESS) {
        return scl_bss_info.retval;
    }
    else {
        SCL_LOG(("SCL_TX_GET_BSS_INFO error\n"));
        return SCL_ERROR;
    }
}

uint32_t scl_wifi_set_ioctl_value(uint32_t ioctl, uint32_t value) {
    scl_result_t retval = SCL_SUCCESS;
    scl_ioctl_value_t scl_ioctl_value;
    scl_ioctl_value.ioctl = ioctl;
    scl_ioctl_value.value = value;
    retval = scl_send_data(SCL_TX_SET_IOCTL_VALUE, (char *)&scl_ioctl_value, TIMER_DEFAULT_VALUE);
    return retval;
}

uint32_t scl_wifi_join(const scl_ssid_t *ssid, scl_security_t auth_type,
                              const uint8_t *security_key, uint8_t key_length) {
    scl_result_t retval = SCL_SUCCESS;
    scl_network_credentials_t network_credentials_for_np;
    network_credentials_for_np.ssid = ssid;
    network_credentials_for_np.auth_type = scl_to_nsapi_security(auth_type);
    network_credentials_for_np.security_key = security_key;
    network_credentials_for_np.key_length = key_length;
    retval = scl_send_data(SCL_TX_WIFI_JOIN, (char *)&network_credentials_for_np, TIMER_DEFAULT_VALUE);

    return retval;
    
}

scl_result_t scl_wifi_leave(void) {
    scl_result_t retval = SCL_SUCCESS;
    char dummy_variable;
    retval = scl_send_data(SCL_TX_DISCONNECT, &dummy_variable, TIMER_DEFAULT_VALUE);

    return retval;
    
}

static uint8_t scl_find_number_of_events(const scl_event_num_t *event_nums) {
    uint8_t count = 0;

    while (*event_nums != SCL_WLC_E_NONE)
    {
        count++;
        event_nums++;

        if (count >= SCL_MAX_EVENT_SUBSCRIPTION)
            return 0;
    }
    return count + 1;
}

scl_result_t scl_management_set_event_handler(const scl_event_num_t *event_nums,
                                                     scl_event_handler_t handler_func,
                                                     void *handler_user_data, uint16_t *event_index) {
    uint16_t entry = (uint16_t)0xFF;
    uint16_t i;
    uint8_t num_of_events;
    num_of_events = scl_find_number_of_events(event_nums);
    
    if (num_of_events <= 1)
    {
        SCL_LOG( ("Exceeded the maximum event subscription/no event subscribed\n") );
        return SCL_UNFINISHED;
    }

    /* Find an existing matching entry OR the next empty entry */
    for (i = 0; i < (uint16_t)SCL_EVENT_HANDLER_LIST_SIZE; i++)
    {
        /* Find a matching event list OR the first empty event entry */
        if (!(memcmp(scl_event_list[i].events, event_nums,
                     num_of_events * (sizeof(scl_event_num_t) ) ) ) )
        {
            /* Check if all the data already matches */
            if ( (scl_event_list[i].handler           == handler_func) &&
                 (scl_event_list[i].handler_user_data == handler_user_data) )
            {
                /* send back the entry where the handler is added */
                *event_index = i;
                return SCL_SUCCESS;
            }
        }
        else if ( (entry == (uint16_t)0xFF) && (scl_event_list[i].event_set == SCL_FALSE) )
        {
            entry = i;
        }
    }

    /* Check if handler function was provided */
    if (handler_func != NULL)
    {
        /* Check if an empty entry was not found */
        if (entry == (uint16_t)0xFF)
        {
            SCL_LOG( ("Out of space in event handlers table - try increasing SCL_EVENT_HANDLER_LIST_SIZE\n") );
            return SCL_OUT_OF_EVENT_HANDLER_SPACE;
        }

        /* Add the new handler in at the free space */
        memcpy (scl_event_list[entry].events, event_nums, num_of_events * (sizeof(scl_event_num_t) ) );
        scl_event_list[entry].handler           = handler_func;
        scl_event_list[entry].handler_user_data = handler_user_data;
        scl_event_list[entry].event_set         = SCL_TRUE;
        *event_index = entry;
    }
    else
    {
        SCL_LOG(("Event handler callback function is NULL/not provided to register\n") );
        return SCL_BADARG;
    }
    return SCL_SUCCESS;
}
void scl_process_events_from_np(const scl_event_header_t *event_header,
                                     const uint8_t *event_data, void *handler_user_data) {
    uint16_t i;
    uint16_t j;

    for (i = 0; i < (uint16_t)SCL_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (scl_event_list[i].event_set)
        {
            for (j = 0; scl_event_list[i].events[j] != SCL_WLC_E_NONE; ++j)
            {
                if ( (scl_event_list[i].events[j] == event_header->event_type) )
                {
                    /* Correct event type has been found - call the handler function and exit loop */
                    scl_event_list[i].handler_user_data =
                        scl_event_list[i].handler(event_header,event_data,handler_user_data);
                    break;
                }
            }
        }
    }
}
