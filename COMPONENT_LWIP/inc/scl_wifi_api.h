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

/** @file
 *  Prototypes of functions for controlling the Wi-Fi system
 *
 *  This file provides prototypes for end-user functions, which allow
 *  actions such as scanning for Wi-Fi networks, joining Wi-Fi
 *  networks, getting the MAC address, and so on.
 *
 */

#include <stdbool.h>
#include "scl_common.h"
#include "scl_types.h"
#ifndef INCLUDED_SCL_WIFI_API_H
#define INCLUDED_SCL_WIFI_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * SCL transmit buffer structure
 */
typedef struct scl_tx_buf {
    scl_buffer_t buffer; /**< pointer to the buffer */
    uint32_t size;       /**< size of the buffer */
} scl_tx_buf_t;

/**
 *  Structure to store fields after ethernet header in event message
 */
struct scl_event_msg
{
    uint16_t version;               /**< Version */
    uint16_t flags;                 /**< see flags below */
    uint32_t event_type;            /**< Event type indicating a response from firmware for IOCTLs/IOVARs sent */
    uint32_t status;                /**< Status code corresponding to any event type */
    uint32_t reason;                /**< Reason code associated with the event occurred */
    uint32_t auth_type;             /**< WLC_E_AUTH: 802.11 AUTH request */
    uint32_t datalen;               /**< Length of data in event message */
    scl_mac_t addr;                 /**< Station address (if applicable) */
    char ifname[16];                /**< name of the incoming packet interface */
    uint8_t ifidx;                  /**< destination OS i/f index */
    uint8_t bsscfgidx;              /**< source bsscfg index */
};

/** @cond */
typedef struct scl_event_msg scl_event_header_t;
/** @endcond */
/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup wifi SCL Wi-Fi API
 *  APIs for controlling the Wi-Fi system
 *  @{
 */

/** Turn on the Wi-Fi device
 *
 *  @note This API should be called before using any SCL Wi-Fi API.
 *
 *  @return True if initialization is successful, False otherwise.
 */
extern bool scl_wifi_on(void);

/** Brings up the Wi-Fi core
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_set_up(void);

/** Retrieves the current Media Access Control (MAC) address
 *  (or Ethernet hardware address) of the 802.11 device
 *
 *  @param   mac           Pointer to a variable to which the current MAC address will be written.
 *
 *  @return  SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_get_mac_address(scl_mac_t *mac);

/** Gets the BSSID of the interface
 *
 *  @note This API should be used after the device is connected to a network.
 *
 *  @param  bssid         Returns the BSSID address (mac address), if associated.
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_get_bssid(scl_mac_t *bssid);

/** Registers interest in a multicast address
 *
 *  Once a multicast address has been registered, all packets detected on the
 *  medium destined for that address are forwarded to the host.
 *  Otherwise, the packets are ignored.
 *
 *  @param  mac              Ethernet MAC address.
 *
 *  @return SCL_SUCCESS      If the address was registered successfully or Error code.
 */
extern scl_result_t scl_wifi_register_multicast_address(scl_mac_t *mac);

/** Determines if an interface is ready to transmit/receive ethernet packets.
 *
 *  @note This function must be called after the connection is established; otherwise, it returns Error code.
 *
 *  @return    SCL_SUCCESS            If the interface is ready to transmit/receive ethernet packets.
 *             SCL_NOTFOUND           If no AP with a matching SSID was found.
 *             SCL_NOT_AUTHENTICATED  If matching AP was found, but it does not let you authenticate.
 *                                    @note This can occur if the device is in the blocklist of the AP.
 *             SCL_NOT_KEYED          If the device has authenticated and associated but has not completed the key exchange.
 *                                    @note This can occur if the passphrase is incorrect.
 *             Error code             If the interface is not ready to transmit/receive ethernet packets.
 */
extern scl_result_t scl_wifi_is_ready_to_transceive(void);

/** Sends an ethernet frame to SCL (called by the Network Stack)
 *
 *  This function takes ethernet data from the network stack and transmits over the wireless network.
 *  This function returns immediately after the packet has been queued for transmission,
 *  NOT after it has been transmitted. Packet buffers passed to the SCL
 *  are released inside the SCL once they have been transmitted.
 *
 *  @param buffer        Handle of the packet buffer to be sent.
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_network_send_ethernet_data(scl_tx_buf_t buffer);

/** Retrieves the latest RSSI value
 *
 *  @note This API must be called after the device is connected to a network.
 *
 *  @param   rssi          Location where the RSSI value will be stored.
 *
 *  @return  SCL_SUCCESS   If the RSSI was successfully retrieved or Error code.
 */
extern scl_result_t scl_wifi_get_rssi(int32_t *rssi);

/** Retrieves the RX data packet
 *
 *  @param   buffer        Pointer to RX buffer.
 */
extern void scl_network_process_ethernet_data(scl_buffer_t buffer);

/** Notifies network stack about the change in network connection state
 *
 *  @param   state_up      Connection state.
 */
extern void scl_emac_wifi_link_state_changed(bool state_up);

/** Scan result callback function pointer type
 *
 * @param result_ptr   A pointer to the pointer that indicates where to put the next scan result
 * @param user_data    User provided data
 * @param status       Status of scan process
 */
typedef void (*scl_scan_result_callback_t)(scl_scan_result_t *result_ptr, void *user_data, scl_scan_status_t status);

/** Initiates a scan to search for 802.11 networks.
 *
 *  The scan progressively accumulates results over time, and may take between 1 and 10 seconds to complete.
 *  The results of the scan will be individually provided to the callback function.
 *  Note: The callback function will be executed in the context of the SCL thread and so must not perform any
 *  actions that may cause a bus transaction.
 *
 *  @param   scan_type                 Specifies whether the scan should be Active, Passive or scan Prohibited channels
 *  @param   bss_type                  Specifies whether the scan should search for Infrastructure networks (those using
 *                                     an Access Point), Ad-hoc networks, or both types.
 *  @param   optional_ssid             If this is non-Null, then the scan will only search for networks using the specified SSID.
 *  @param   optional_mac              If this is non-Null, then the scan will only search for networks where
 *                                     the BSSID (MAC address of the Access Point) matches the specified MAC address.
 *  @param   optional_channel_list     If this is non-Null, then the scan will only search for networks on the
 *                                     specified channels - array of channel numbers to search, terminated with a zero
 *  @param   optional_extended_params  If this is non-Null, then the scan will obey the specifications about
 *                                     dwell times and number of probes.
 *  @param   callback                  The callback function which will receive and process the result data.
 *  @param   result_ptr                Pointer to a pointer to a result storage structure.
 *  @param   user_data                 user specific data that will be passed directly to the callback function
 *
 *  @note - When scanning specific channels, devices with a strong signal strength on nearby channels may be detected
 *        - Callback must not use blocking functions, nor use SCL functions, since it is called from the context of the
 *          SCL thread.
 *        - The callback, result_ptr and user_data variables will be referenced after the function returns.
 *          Those variables must remain valid until the scan is complete.
 *
 *  @return SCL_SUCCESS or Error code
 */
extern uint32_t scl_wifi_scan(scl_scan_type_t scan_type,
                              scl_bss_type_t bss_type,
                              const scl_ssid_t *optional_ssid,
                              const scl_mac_t *optional_mac,
                              const uint16_t *optional_channel_list,
                              const scl_scan_extended_params_t *optional_extended_params,
                              scl_scan_result_callback_t callback,
                              scl_scan_result_t *result_ptr,
                              void *user_data);

/** Retrives the bss info
 *
 *  @param  bi                   A pointer to the structure scl_wl_bss_info_t
 *
 *  @return SCL_SUCCESS or Error code
 */
extern uint32_t scl_wifi_get_bss_info(scl_wl_bss_info_t *bi);

/** Sends an IOCTL command - CDC_SET IOCTL value
 *
 *  @param  ioctl          CDC_SET - To set the I/O control
 *  @param  value          Data value to be sent
 *
 *  @return SCL_SUCCESS or Error code
 */
extern uint32_t scl_wifi_set_ioctl_value(uint32_t ioctl, uint32_t value);

/** Joins a Wi-Fi network
 *
 *  Scans for, associates and authenticates with a Wi-Fi network.
 *  On successful return, the system is ready to send data packets.
 *  If a network stack is being used, bring up of the network interface should be done after successful return from this function
 *	@note 	 wait until NP connects to the network after calling this API
 *  @param   ssid          A null terminated string containing the SSID name of the network to join
 *  @param   auth_type     Authentication type
 *  @param   security_key  A byte array containing either the cleartext security key for WPA/WPA2/WPA3 secured networks
 *  @param   key_length    The length of the security_key in bytes.
 *
 *  @return  SCL_SUCCESS   If NP received the credentials successfully,
 *           Error code    if an error occurred
 */
extern uint32_t scl_wifi_join(const scl_ssid_t *ssid, scl_security_t auth_type, const uint8_t *security_key, uint8_t key_length);

/** Leaves a Wi-Fi network
 *
 *  @return  SCL_SUCCESS   when the system is joined and ready to send data packets
 *           Error code    if an error occurred
 */
extern scl_result_t scl_wifi_leave(void);

/**
 * Event handler prototype definition
 *
 * @param[out] scl_event_header_t : scl event header
 * @param[out] uint8_t*           : event data
 * @param[out] handler_user_data  : semaphore data
 */
typedef void *(*scl_event_handler_t)(const scl_event_header_t *event_header,
                                     const uint8_t *event_data, void *handler_user_data);

/**
 * Registers a handler to receive event callbacks.
 * Subscribe locally and notify Wi-Fi about subscription.
 *
 * This function registers a callback handler to be notified when
 * a particular event is received.
 *
 * @note : Currently there is a limit to the number of simultaneously
 *         registered events
 *
 * @param event_nums          An array of event types that is to trigger the handler.
 *                            The array must be terminated with a SCL_WLC_E_NONE event.
 * @param handler_func        A function pointer to the new handler callback
 * @param handler_user_data   A pointer value which will be passed to the event handler function
 *                            at the time an event is triggered (NULL is allowed)
 * @param[out] *event_index   entry where the event handler is registered in the list
 *
 * @return SCL result code
 */
extern scl_result_t scl_management_set_event_handler(const scl_event_num_t *event_nums,
                                                     scl_event_handler_t handler_func,
                                                     void *handler_user_data, uint16_t *event_index);
/**
 * Invokes the registered scan callback when there is scan result available
 *
 * @note : This function is not meant to be used by application
 *
 * @param status          status of the scan
 */
extern void scl_wifi_scan_callback(scl_scan_status_t status);
/** @} wifi*/
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_WIFI_API_H */
