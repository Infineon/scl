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

/** @file scl_types.h
 *  Defines common data types used in SCL
 *
 */

#include <stdint.h>
#include "cy_result.h"
#include "scl_common.h"
#ifndef INCLUDED_SCL_TYPES_H_
#define INCLUDED_SCL_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************
*                      Macros
******************************************************/
#define SSID_NAME_SIZE           (33)  /**< SSID Length */

#define WEP_ENABLED            0x0001  /**< Flag to enable WEP Security        */
#define TKIP_ENABLED           0x0002  /**< Flag to enable TKIP Encryption     */
#define AES_ENABLED            0x0004  /**< Flag to enable AES Encryption      */
#define SHARED_ENABLED     0x00008000  /**< Flag to enable Shared key Security */
#define WPA_SECURITY       0x00200000  /**< Flag to enable WPA Security        */
#define WPA2_SECURITY      0x00400000  /**< Flag to enable WPA2 Security       */
#define WPA3_SECURITY      0x01000000  /**< Flag to enable WPA3 PSK Security   */

#define ENTERPRISE_ENABLED 0x02000000  /**< Flag to enable Enterprise Security */
#define WPS_ENABLED        0x10000000  /**< Flag to enable WPS Security        */
#define IBSS_ENABLED       0x20000000  /**< Flag to enable IBSS mode           */
#define FBT_ENABLED        0x40000000  /**< Flag to enable FBT                 */

#define NO_POWERSAVE_MODE           (0) /**< No Power save mode */
#define PM1_POWERSAVE_MODE          (1) /**< Power save mode on specified interface without regard for throughput reduction */
#define PM2_POWERSAVE_MODE          (2) /**< Power save mode on specified interface with High throughput */

#define SCL_MAX_EVENT_SUBSCRIPTION      (33) /**< Maximum number of events registered at a time */

#define SCL_EVENT_NOT_REGISTERED        (0xFF) /**< Event not registered */

#define SCL_EVENT_HANDLER_LIST_SIZE     (5)      /**< Maximum number of simultaneously registered event handlers */

/**
 * Suppresses unused parameter warning
 */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/**
 * Suppresses unused variable warning
 */
#define UNUSED_VARIABLE(x) ( (void)(x) )

/**
 * Suppresses unused variable warning that occurs due to an assert being disabled in release mode
 */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*               Structures and Enumerations
******************************************************/

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum {
    SCL_SECURITY_OPEN             = 0,                                                                 /**< Open security                                         */
    SCL_SECURITY_WEP_PSK          = WEP_ENABLED,                                                       /**< WEP PSK Security with open authentication             */
    SCL_SECURITY_WEP_SHARED       = (WEP_ENABLED | SHARED_ENABLED),                                    /**< WEP PSK Security with shared authentication           */
    SCL_SECURITY_WPA_TKIP_PSK     = (WPA_SECURITY | TKIP_ENABLED),                                     /**< WPA PSK Security with TKIP                            */
    SCL_SECURITY_WPA_AES_PSK      = (WPA_SECURITY | AES_ENABLED),                                      /**< WPA PSK Security with AES                             */
    SCL_SECURITY_WPA_MIXED_PSK    = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                       /**< WPA PSK Security with AES & TKIP                      */
    SCL_SECURITY_WPA2_AES_PSK     = (WPA2_SECURITY | AES_ENABLED),                                     /**< WPA2 PSK Security with AES                            */
    SCL_SECURITY_WPA2_TKIP_PSK    = (WPA2_SECURITY | TKIP_ENABLED),                                    /**< WPA2 PSK Security with TKIP                           */
    SCL_SECURITY_WPA2_MIXED_PSK   = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                      /**< WPA2 PSK Security with AES & TKIP                     */
    SCL_SECURITY_WPA2_FBT_PSK     = (WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),                       /**< WPA2 FBT PSK Security with AES & TKIP */
    SCL_SECURITY_WPA3_SAE         = (WPA3_SECURITY | AES_ENABLED),                                     /**< WPA3 Security with AES */
    SCL_SECURITY_WPA2_WPA_AES_PSK  = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED),                     /**< WPA2 WPA PSK Security with AES                        */
    SCL_SECURITY_WPA2_WPA_MIXED_PSK = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),      /**< WPA2 WPA PSK Security with AES & TKIP                 */

    SCL_SECURITY_WPA3_WPA2_PSK    = (WPA3_SECURITY | WPA2_SECURITY | AES_ENABLED),                     /**< WPA3 WPA2 PSK Security with AES */
    SCL_SECURITY_WPA2_WPA_PSK     = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED),                      /**< WPA2 WPA PSK Security with AES */
    SCL_SECURITY_WPA2_WPA_TKIP_PSK = (WPA2_SECURITY | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                      /**< WPA2 WPA PSK Security with AES */

    SCL_SECURITY_WPA_TKIP_ENT     = (ENTERPRISE_ENABLED | WPA_SECURITY | TKIP_ENABLED),                /**< WPA Enterprise Security with TKIP                     */
    SCL_SECURITY_WPA_AES_ENT      = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED),                 /**< WPA Enterprise Security with AES                      */
    SCL_SECURITY_WPA_MIXED_ENT    = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),  /**< WPA Enterprise Security with AES & TKIP               */
    SCL_SECURITY_WPA2_TKIP_ENT    = (ENTERPRISE_ENABLED | WPA2_SECURITY | TKIP_ENABLED),               /**< WPA2 Enterprise Security with TKIP                    */
    SCL_SECURITY_WPA2_AES_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED),                /**< WPA2 Enterprise Security with AES                     */
    SCL_SECURITY_WPA2_MIXED_ENT   = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED), /**< WPA2 Enterprise Security with AES & TKIP              */
    SCL_SECURITY_WPA2_FBT_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),  /**< WPA2 Enterprise Security with AES & FBT               */

    SCL_SECURITY_IBSS_OPEN        = (IBSS_ENABLED),                                                    /**< Open security on IBSS ad-hoc network                  */
    SCL_SECURITY_WPS_OPEN         = (WPS_ENABLED),                                                     /**< WPS with open security                                */
    SCL_SECURITY_WPS_SECURE       = (WPS_ENABLED | AES_ENABLED),                                       /**< WPS with AES security                                 */

    SCL_SECURITY_UNKNOWN          = -1,                                                                /**< Returned by scan function if security is unknown. Do not pass this to the join function! */

    SCL_SECURITY_FORCE_32_BIT     = 0x7fffffff                                                         /**< Exists only to force scl_security_t type to 32 bits */
} scl_security_t;



/**
 * Enumeration of 802.11 radio bands
 */
typedef enum
{
    SCL_802_11_BAND_5GHZ   = 0, /**< Denotes 5GHz radio band   */
    SCL_802_11_BAND_2_4GHZ = 1  /**< Denotes 2.4GHz radio band */
} scl_802_11_band_t;

/** Structure for storing 802.11 powersave listen interval values */
typedef struct
{
    uint8_t beacon; /**< Listen interval in beacon periods */
    uint8_t dtim;   /**< Listen interval in DTIM periods   */
    uint16_t assoc; /**< Listen interval as sent to APs    */
} scl_listen_interval_t;

/**
 * Enumeration of methods of scanning
 */
typedef enum
{
    SCL_SCAN_TYPE_ACTIVE              = 0x00, /**< Actively scan a network by sending 802.11 probe(s)                              */
    SCL_SCAN_TYPE_PASSIVE             = 0x01, /**< Passively scan a network by listening for beacons from APs                      */
    SCL_SCAN_TYPE_PNO                 = 0x02, /**< Use preferred network offload to detect an AP                                   */
    SCL_SCAN_TYPE_PROHIBITED_CHANNELS = 0x04, /**< Permit (passively) scanning a channel that isn't valid for the current country  */
    SCL_SCAN_TYPE_NO_BSSID_FILTER     = 0x08  /**< Return a scan record for each beacon or probe response RX'ed                    */
} scl_scan_type_t;

/**
 * Enumeration of network types
 */
typedef enum
{
    SCL_BSS_TYPE_INFRASTRUCTURE = 0, /**< Denotes infrastructure network                  */
    SCL_BSS_TYPE_ADHOC          = 1, /**< Denotes an 802.11 ad-hoc IBSS network           */
    SCL_BSS_TYPE_ANY            = 2, /**< Denotes either infrastructure or ad-hoc network */
    SCL_BSS_TYPE_MESH           = 3, /**< Denotes 802.11 mesh network                     */

    SCL_BSS_TYPE_UNKNOWN = -1 /**< May be returned by scan function if BSS type is unknown. Do not pass this to the Join function */
} scl_bss_type_t;

/**
 * Structure for storing a Service Set Identifier (i.e. Name of Access Point)
 */
typedef struct
{
    uint8_t length;                /**< SSID length */
    uint8_t value[SSID_NAME_SIZE]; /**< SSID name (AP name)  */
} scl_ssid_t;

/**
 * Structure for storing scan status
 */
typedef enum
{
    SCL_SCAN_INCOMPLETE = 0,                /**< Denotes that scan is not finished */
    SCL_SCAN_COMPLETED_SUCCESSFULLY,    /**< Successful completion of scan */
    SCL_SCAN_ABORTED,                   /**< Scan is aborted */
} scl_scan_status_t;

/**
 * Structure for storing extended scan parameters
 */
typedef struct
{
    int32_t number_of_probes_per_channel;                     /**< Number of probes to send on each channel                                               */
    int32_t scan_active_dwell_time_per_channel_ms;            /**< Period of time to wait on each channel when active scanning                            */
    int32_t scan_passive_dwell_time_per_channel_ms;           /**< Period of time to wait on each channel when passive scanning                           */
    int32_t scan_home_channel_dwell_time_between_channels_ms; /**< Period of time to wait on the home channel when scanning. Only relevant if associated. */
} scl_scan_extended_params_t;

/**
 * Structure for storing scan results
 */
#pragma pack(1)
typedef struct scl_scan_result
{
    scl_ssid_t SSID;                            /**< Service Set Identification (i.e. Name of Access Point)                    */
    scl_mac_t BSSID;                            /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength;                    /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    uint32_t max_data_rate;                     /**< Maximum data rate in kilobits/s                                           */
    scl_bss_type_t bss_type;                    /**< Network type                                                              */
    scl_security_t security;                    /**< Security type                                                             */
    uint8_t channel;                            /**< Radio channel that the AP beacon was received on                          */
    scl_802_11_band_t band;                     /**< Radio band                                                                */
    uint8_t ccode[2];                           /**< Two letter ISO country code from AP                                       */
    uint8_t flags;                              /**< flags                                                                     */
    struct scl_scan_result *next;               /**< Pointer to the next scan result                                           */
    uint8_t *ie_ptr;                            /**< Pointer to received Beacon/Probe Response IE(Information Element)         */
    uint32_t ie_len;                            /**< Length of IE(Information Element)                                         */
} scl_scan_result_t;
#pragma pack()

/**
 * Structure to store scan result parameters for each AP
 */
typedef struct scl_simple_scan_result
{
    scl_ssid_t SSID;         /**< Service Set Identification (i.e. Name of Access Point)                    */
    scl_mac_t BSSID;         /**< Basic Service Set Identification (i.e. MAC address of Access Point)       */
    int16_t signal_strength; /**< Receive Signal Strength Indication in dBm. <-90=Very poor, >-30=Excellent */
    scl_security_t security; /**< Security type                                                             */
    uint8_t channel;         /**< Radio channel that the AP beacon was received on                          */
} scl_sync_scan_result_t;


typedef uint16_t scl_wl_chanspec_t;  /**< Channel specified in uint16_t */
#define MCSSET_LEN    16 /**< Maximum allowed mcs rate */

/** BSS(Basic Service Set) information structure
 *
 * Applications MUST CHECK ie_offset field and length field to access IEs(Information Elements) and
 * next bss_info structure in a vector (in whd_sync_scan_result_t)
 */
typedef struct scl_wl_bss_info_struct
{
    uint32_t version;              /**< version field */
    uint32_t length;               /**< byte length of data in this record, starting at version and including IEs */
    scl_mac_t BSSID;               /**< Unique 6-byte MAC address */
    uint16_t beacon_period;        /**< Interval between two consecutive beacon frames. Units are Kusec */
    uint16_t capability;           /**< Capability information */
    uint8_t SSID_len;              /**< SSID length */
    uint8_t SSID[32];              /**< Array to store SSID */
    struct
    {
        uint32_t count;            /**< Count of rates in this set */
        uint8_t rates[16];         /**< rates in 500kbps units, higher bit set if basic */
    } rateset;                     /**< supported rates */
    scl_wl_chanspec_t chanspec;        /**< Channel specification for basic service set */
    uint16_t atim_window;          /**< Announcement traffic indication message window size. Units are Kusec */
    uint8_t dtim_period;           /**< Delivery traffic indication message period */
    int16_t RSSI;                  /**< receive signal strength (in dBm) */
    int8_t phy_noise;              /**< noise (in dBm) */

    uint8_t n_cap;                 /**< BSS is 802.11N Capable */
    uint32_t nbss_cap;             /**< 802.11N BSS Capabilities (based on HT_CAP_*) */
    uint8_t ctl_ch;                /**< 802.11N BSS control channel number */
    uint32_t reserved32[1];        /**< Reserved for expansion of BSS properties */
    uint8_t flags;                 /**< flags */
    uint8_t reserved[3];           /**< Reserved for expansion of BSS properties */
    uint8_t basic_mcs[MCSSET_LEN]; /**< 802.11N BSS required MCS set */

    uint16_t ie_offset;            /**< offset at which IEs start, from beginning */
    uint32_t ie_length;            /**< byte length of Information Elements */
    int16_t SNR;                   /**< Average SNR(signal to noise ratio) during frame reception */
    /* Add new fields here */
    /* variable length Information Elements */
} scl_wl_bss_info_t;

/**
 * Enumerated list of event types
 */
typedef enum
{
    SCL_WLC_E_NONE = 0x7FFFFFFE,
    SCL_WLC_E_SET_SSID = 0 /** indicates status of set SSID */,
    SCL_WLC_E_JOIN = 1,     /** differentiates join IBSS from found (SCL_WLC_E_START) IBSS */
    SCL_WLC_E_START = 2,     /** STA founded an IBSS or AP started a BSS */
    SCL_WLC_E_AUTH = 3,     /** 802.11 AUTH request */
    SCL_WLC_E_AUTH_IND = 4,     /** 802.11 AUTH indication */
    SCL_WLC_E_DEAUTH = 5,     /** 802.11 DEAUTH request */
    SCL_WLC_E_DEAUTH_IND = 6,     /** 802.11 DEAUTH indication */
    SCL_WLC_E_ASSOC = 7,     /** 802.11 ASSOC request */
    SCL_WLC_E_ASSOC_IND = 8,     /** 802.11 ASSOC indication */
    SCL_WLC_E_REASSOC = 9,     /** 802.11 REASSOC request */
    SCL_WLC_E_REASSOC_IND = 10,     /** 802.11 REASSOC indication */
    SCL_WLC_E_DISASSOC = 11,     /** 802.11 DISASSOC request */
    SCL_WLC_E_DISASSOC_IND = 12,     /** 802.11 DISASSOC indication */
    SCL_WLC_E_QUIET_START = 13,     /** 802.11h Quiet period started */
    SCL_WLC_E_QUIET_END = 14,     /** 802.11h Quiet period ended */
    SCL_WLC_E_BEACON_RX = 15,     /** BEACONS received/lost indication */
    SCL_WLC_E_LINK = 16,     /** generic link indication */
    SCL_WLC_E_MIC_ERROR = 17,     /** TKIP MIC error occurred */
    SCL_WLC_E_NDIS_LINK = 18,     /** NDIS style link indication */
    SCL_WLC_E_ROAM = 19,     /** roam attempt occurred: indicate status & reason */
    SCL_WLC_E_TXFAIL = 20,     /** change in dot11FailedCount (txfail) */
    SCL_WLC_E_PMKID_CACHE = 21,     /** WPA2 pmkid cache indication */
    SCL_WLC_E_RETROGRADE_TSF = 22,     /** current AP's TSF value went backward */
    SCL_WLC_E_PRUNE = 23,     /** AP was pruned from join list for reason */
    SCL_WLC_E_AUTOAUTH = 24,     /** report AutoAuth table entry match for join attempt */
    SCL_WLC_E_EAPOL_MSG = 25,     /** Event encapsulating an EAPOL message */
    SCL_WLC_E_SCAN_COMPLETE = 26,     /** Scan results are ready or scan was aborted */
    SCL_WLC_E_ADDTS_IND = 27,     /** indicate to host addts fail/success */
    SCL_WLC_E_DELTS_IND = 28,     /** indicate to host delts fail/success */
    SCL_WLC_E_BCNSENT_IND = 29,     /** indicate to host of beacon transmit */
    SCL_WLC_E_BCNRX_MSG = 30,     /** Send the received beacon up to the host */
    SCL_WLC_E_BCNLOST_MSG = 31,     /** indicate to host loss of beacon */
    SCL_WLC_E_ROAM_PREP = 32,     /** before attempting to roam */
    SCL_WLC_E_PFN_NET_FOUND = 33,     /** PFN network found event */
    SCL_WLC_E_PFN_NET_LOST = 34,     /** PFN network lost event */
    SCL_WLC_E_RESET_COMPLETE = 35, SCL_WLC_E_JOIN_START = 36, SCL_WLC_E_ROAM_START = 37, SCL_WLC_E_ASSOC_START = 38,
    SCL_WLC_E_IBSS_ASSOC = 39, SCL_WLC_E_RADIO = 40, SCL_WLC_E_PSM_WATCHDOG = 41,                                                                                                      /** PSM microcode watchdog fired */
    SCL_WLC_E_CCX_ASSOC_START = 42,     /** CCX association start */
    SCL_WLC_E_CCX_ASSOC_ABORT = 43,     /** CCX association abort */
    SCL_WLC_E_PROBREQ_MSG = 44,     /** probe request received */
    SCL_WLC_E_SCAN_CONFIRM_IND = 45, SCL_WLC_E_PSK_SUP = 46,     /** WPA Handshake */
    SCL_WLC_E_COUNTRY_CODE_CHANGED = 47, SCL_WLC_E_EXCEEDED_MEDIUM_TIME = 48,     /** WMMAC excedded medium time */
    SCL_WLC_E_ICV_ERROR = 49,     /** WEP ICV error occurred */
    SCL_WLC_E_UNICAST_DECODE_ERROR = 50,     /** Unsupported unicast encrypted frame */
    SCL_WLC_E_MULTICAST_DECODE_ERROR = 51,     /** Unsupported multicast encrypted frame */
    SCL_WLC_E_TRACE = 52, SCL_WLC_E_BTA_HCI_EVENT = 53,     /** BT-AMP HCI event */
    SCL_WLC_E_IF = 54,     /** I/F change (for wlan host notification) */
    SCL_WLC_E_P2P_DISC_LISTEN_COMPLETE = 55,     /** P2P Discovery listen state expires */
    SCL_WLC_E_RSSI = 56,     /** indicate RSSI change based on configured levels */
    SCL_WLC_E_PFN_BEST_BATCHING = 57,     /** PFN best network batching event */
    SCL_WLC_E_EXTLOG_MSG = 58, SCL_WLC_E_ACTION_FRAME = 59,     /** Action frame reception */
    SCL_WLC_E_ACTION_FRAME_COMPLETE = 60,     /** Action frame Tx complete */
    SCL_WLC_E_PRE_ASSOC_IND = 61,     /** assoc request received */
    SCL_WLC_E_PRE_REASSOC_IND = 62,     /** re-assoc request received */
    SCL_WLC_E_CHANNEL_ADOPTED = 63,     /** channel adopted (xxx: obsoleted) */
    SCL_WLC_E_AP_STARTED = 64,     /** AP started */
    SCL_WLC_E_DFS_AP_STOP = 65,     /** AP stopped due to DFS */
    SCL_WLC_E_DFS_AP_RESUME = 66,     /** AP resumed due to DFS */
    SCL_WLC_E_WAI_STA_EVENT = 67,     /** WAI stations event */
    SCL_WLC_E_WAI_MSG = 68,     /** event encapsulating an WAI message */
    SCL_WLC_E_ESCAN_RESULT = 69,     /** escan result event */
    SCL_WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE = 70,     /** action frame off channel complete *//* NOTE - This used to be SCL_WLC_E_WAKE_EVENT */
    SCL_WLC_E_PROBRESP_MSG = 71,     /** probe response received */
    SCL_WLC_E_P2P_PROBREQ_MSG = 72,     /** P2P Probe request received */
    SCL_WLC_E_DCS_REQUEST = 73, SCL_WLC_E_FIFO_CREDIT_MAP = 74,     /** credits for D11 FIFOs. [AC0,AC1,AC2,AC3,BC_MC,ATIM] */
    SCL_WLC_E_ACTION_FRAME_RX = 75,     /** Received action frame event WITH wl_event_rx_frame_data_t header */
    SCL_WLC_E_WAKE_EVENT = 76,     /** Wake Event timer fired, used for wake WLAN test mode */
    SCL_WLC_E_RM_COMPLETE = 77,     /** Radio measurement complete */
    SCL_WLC_E_HTSFSYNC = 78,     /** Synchronize TSF with the host */
    SCL_WLC_E_OVERLAY_REQ = 79,     /** request an overlay IOCTL/iovar from the host */
    SCL_WLC_E_CSA_COMPLETE_IND = 80, SCL_WLC_E_EXCESS_PM_WAKE_EVENT = 81,     /** excess PM Wake Event to inform host  */
    SCL_WLC_E_PFN_SCAN_NONE = 82,     /** no PFN networks around */
    SCL_WLC_E_PFN_SCAN_ALLGONE = 83,     /** last found PFN network gets lost */
    SCL_WLC_E_GTK_PLUMBED = 84, SCL_WLC_E_ASSOC_IND_NDIS = 85,     /** 802.11 ASSOC indication for NDIS only */
    SCL_WLC_E_REASSOC_IND_NDIS = 86,     /** 802.11 REASSOC indication for NDIS only */
    SCL_WLC_E_ASSOC_REQ_IE = 87, SCL_WLC_E_ASSOC_RESP_IE = 88, SCL_WLC_E_ASSOC_RECREATED = 89,     /** association recreated on resume */
    SCL_WLC_E_ACTION_FRAME_RX_NDIS = 90,     /** rx action frame event for NDIS only */
    SCL_WLC_E_AUTH_REQ = 91,     /** authentication request received */
    SCL_WLC_E_TDLS_PEER_EVENT = 92,     /** discovered peer, connected/disconnected peer */
    SCL_WLC_E_MESH_DHCP_SUCCESS = 92,     /** DHCP handshake successful for a mesh interface */
    SCL_WLC_E_SPEEDY_RECREATE_FAIL = 93,     /** fast assoc recreation failed */
    SCL_WLC_E_NATIVE = 94,     /** port-specific event and payload (e.g. NDIS) */
    SCL_WLC_E_PKTDELAY_IND = 95,     /** event for tx pkt delay suddently jump */
    SCL_WLC_E_AWDL_AW = 96,     /** AWDL AW period starts */
    SCL_WLC_E_AWDL_ROLE = 97,     /** AWDL Master/Slave/NE master role event */
    SCL_WLC_E_AWDL_EVENT = 98,     /** Generic AWDL event */
    SCL_WLC_E_NIC_AF_TXS = 99,     /** NIC AF txstatus */
    SCL_WLC_E_NAN = 100,     /** NAN event */
    SCL_WLC_E_BEACON_FRAME_RX = 101, SCL_WLC_E_SERVICE_FOUND = 102,     /** desired service found */
    SCL_WLC_E_GAS_FRAGMENT_RX = 103,     /** GAS fragment received */
    SCL_WLC_E_GAS_COMPLETE = 104,     /** GAS sessions all complete */
    SCL_WLC_E_P2PO_ADD_DEVICE = 105,     /** New device found by p2p offload */
    SCL_WLC_E_P2PO_DEL_DEVICE = 106,     /** device has been removed by p2p offload */
    SCL_WLC_E_WNM_STA_SLEEP = 107,     /** WNM event to notify STA enter sleep mode */
    SCL_WLC_E_TXFAIL_THRESH = 108,     /** Indication of MAC tx failures (exhaustion of 802.11 retries) exceeding threshold(s) */
    SCL_WLC_E_PROXD = 109,     /** Proximity Detection event */
    SCL_WLC_E_IBSS_COALESCE = 110,     /** IBSS Coalescing */
    SCL_WLC_E_MESH_PAIRED = 110,     /** Mesh peer found and paired */
    SCL_WLC_E_AWDL_RX_PRB_RESP = 111,     /** AWDL RX Probe response */
    SCL_WLC_E_AWDL_RX_ACT_FRAME = 112,     /** AWDL RX Action Frames */
    SCL_WLC_E_AWDL_WOWL_NULLPKT = 113,     /** AWDL Wowl nulls */
    SCL_WLC_E_AWDL_PHYCAL_STATUS = 114,     /** AWDL Phycal status */
    SCL_WLC_E_AWDL_OOB_AF_STATUS = 115,     /** AWDL OOB AF status */
    SCL_WLC_E_AWDL_SCAN_STATUS = 116,     /** Interleaved Scan status */
    SCL_WLC_E_AWDL_AW_START = 117,     /** AWDL AW Start */
    SCL_WLC_E_AWDL_AW_END = 118,     /** AWDL AW End */
    SCL_WLC_E_AWDL_AW_EXT = 119,     /** AWDL AW Extensions */
    SCL_WLC_E_AWDL_PEER_CACHE_CONTROL = 120, SCL_WLC_E_CSA_START_IND = 121, SCL_WLC_E_CSA_DONE_IND = 122,
    SCL_WLC_E_CSA_FAILURE_IND = 123, SCL_WLC_E_CCA_CHAN_QUAL = 124,                                                                                               /** CCA based channel quality report */
    SCL_WLC_E_BSSID = 125,     /** to report change in BSSID while roaming */
    SCL_WLC_E_TX_STAT_ERROR = 126,     /** tx error indication */
    SCL_WLC_E_BCMC_CREDIT_SUPPORT = 127,     /** credit check for BCMC supported */
    SCL_WLC_E_PSTA_PRIMARY_INTF_IND = 128,     /** psta primary interface indication */
    SCL_WLC_E_BT_WIFI_HANDOVER_REQ = 130,     /* Handover Request Initiated */
    SCL_WLC_E_SPW_TXINHIBIT = 131,     /* Southpaw TxInhibit notification */
    SCL_WLC_E_FBT_AUTH_REQ_IND = 132,     /* FBT Authentication Request Indication */
    SCL_WLC_E_RSSI_LQM = 133,     /* Enhancement addition for SCL_WLC_E_RSSI */
    SCL_WLC_E_PFN_GSCAN_FULL_RESULT = 134,     /* Full probe/beacon (IEs etc) results */
    SCL_WLC_E_PFN_SWC = 135,     /* Significant change in rssi of bssids being tracked */
    SCL_WLC_E_AUTHORIZED = 136,     /* a STA been authroized for traffic */
    SCL_WLC_E_PROBREQ_MSG_RX = 137,     /* probe req with wl_event_rx_frame_data_t header */
    SCL_WLC_E_PFN_SCAN_COMPLETE = 138,     /* PFN completed scan of network list */
    SCL_WLC_E_RMC_EVENT = 139,     /* RMC Event */
    SCL_WLC_E_DPSTA_INTF_IND = 140,     /* DPSTA interface indication */
    SCL_WLC_E_RRM = 141,                /* RRM Event */
    SCL_WLC_E_ULP = 146,                /* ULP entry event */
    SCL_WLC_E_TKO = 151,                /* TCP Keep Alive Offload Event */
    SCL_WLC_E_LAST = 152,               /* highest val + 1 for range checking */
} scl_event_num_t;

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_TYPES_H_ */
