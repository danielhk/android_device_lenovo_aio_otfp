/*
*  Copyright (C) 2014 MediaTek Inc.
*
*  Modification based on code covered by the below mentioned copyright
*  and/or permission notice(s).
*/


/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MTK_RIL_H
#define MTK_RIL_H 1

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <telephony/ril_cdma_sms.h>
#include <telephony/ril_nv_items.h>
#include <telephony/ril_msim.h>
#ifndef FEATURE_UNIT_TEST
#include <sys/time.h>
#endif /* !FEATURE_UNIT_TEST */

#include <utils/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define LOGV RLOGV
#define LOGD RLOGD
#define LOGE RLOGE
#define LOGI RLOGI
#define LOGW RLOGW
#endif

#define MAX_GEMINI_SIM_NUM 4 /* Currently max SIM number support up to 4 */

#define MAX_IPV4_ADDRESS_LENGTH 17
//xxx.xxx.xxx.xxx
#define MAX_IPV6_ADDRESS_LENGTH 65
//xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
//xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx

//VoLTE
#define MAX_PCSCF_NUMBER 6

typedef struct {
    //0: QCI is selected by network
    //[1-4]: value range for guaranteed bit rate Traffic Flows
    //[5-9]: value range for non-guarenteed bit rate Traffic Flows
    //[128-254]: value range for Operator-specific QCIs
    int qci; //class of EPS QoS
    int dlGbr; //downlink guaranteed bit rate
    int ulGbr; //uplink guaranteed bit rate
    int dlMbr; //downlink maximum bit rate
    int ulMbr; //uplink maximum bit rate
} Qos;

typedef struct {
    int id;
    int precedence;
    int direction;
    int networkPfIdentifier;
    int bitmap;
    char address[MAX_IPV6_ADDRESS_LENGTH];
    char mask[MAX_IPV6_ADDRESS_LENGTH];
    int protocolNextHeader;
    int localPortLow;
    int localPortHigh;
    int remotePortLow;
    int remotePortHigh;
    int spi;
    int tos;
    int tosMask;
    int flowLabel;
} PktFilter;

typedef struct {
    int authTokenNumber;
    int authTokenList[16];
    int flowIdNumber;
    int flowIdList[4][4];
} AuthToken;

typedef struct {
    int linkedPfNumber;
    int linkedPfList [16];
    int authtokenFlowIdNumber;
    AuthToken authtokenFlowIdList[4];
} TftParameter;

typedef struct {
    int operation;
    int pfNumber;
    PktFilter pfList [16]; //for response, just send necessary length. check responseSetupDedicateDataCall in ril.cpp
    TftParameter tftParameter;
} Tft;

typedef struct {
    int             ddcId;
    int             interfaceId;
    int             primaryCid;
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    int             signalingFlag;
    int             bearerId;
    int             failCause;
    int             hasQos;
    Qos             qos;
    int             hasTft;
    Tft             tft;
    int             hasPcscf;
    char            pcscf[MAX_PCSCF_NUMBER * (MAX_IPV6_ADDRESS_LENGTH + 1)];
} RIL_Dedicate_Data_Call_Struct;

typedef struct {
    int             isValid;
    Qos             qos;
    int             emergency_ind;
    int             pcscf_discovery_flag;
    int             signalingFlag;
    int             isHandover;
    int             assigned_rate;  //assign by MAL
} RIL_Default_Bearer_VA_Config_Struct;

typedef struct {
    int emc_attached_support;
    int emc_call_barred;
    char* ci;               // string type; four byte E-UTRAN cell ID in hexadecimal format
    char* tac;              // string type; two byte tracking area code in hexadecimal format (e.g. "00C3" equals 195 in decimal)
} RIL_EpcNetworkFeatureInfo;

/// M: For 3G VT only @{
typedef enum {
    MSG_ID_WRAP_3GVT_RIL_CONFIG_INIT_IND = 0,
    MSG_ID_WRAP_3GVT_RIL_CONFIG_UPDATE_IND = 1,
    MSG_ID_WRAP_3GVT_RIL_CONFIG_DEINIT_IND = 2,
    MSG_ID_WRAP_3GVT_RIL_ON_USER_INPUT = 3,
} RIL_VT_MsgType;

typedef struct {
    unsigned char   callId;        // identify this call
    char   dtmfKey;
} RIL_VT_MsgParams;
/// @}

#define PROPERTY_3G_SIM "persist.radio.simswitch"

#define CDMA_ALPHA_INFO_BUFFER_LENGTH 64
#define CDMA_NUMBER_INFO_BUFFER_LENGTH 81

#define MAX_RILDS 3
#define MAX_SOCKET_NAME_LENGTH 20
#define MAX_CLIENT_ID_LENGTH 2
#define MAX_QEMU_PIPE_NAME_LENGTH  11
#define MAX_UUID_LENGTH 64

/* <name> in CNAP URC is an up to 80 characters long string containing the calling name  */
#define MAX_CNAP_LENGTH 81
/* <ccidx> in CLCC response ranges from 1 to N.
 * N, the maximum number of simultaneous call control processes is implementation specific.
 * For GSM, N is 7
 */
#define MAX_GSMCALL_CONNECTIONS 7   // only 7 connections allowed in GSM

typedef enum {
    MD_STATE_NORMAL                = 0,
    MD_STATE_ASSERT                = 1
} RIL_MDState;

typedef enum
{
    RIL_CLIENT_ID_RILJ   =  0, //reserve the value 0
    RIL_CLIENT_ID_MAL    = 1001,
    RIL_CLIENT_ID_MAL_AT = 1002,
} RIL_CLIENT_ID;

//RIL_Client_Header must define same as RIL_CLIENT
typedef struct RIL_Client_Header {
    int clientID;
} RIL_Client_Header;

typedef struct RIL_Token_Header {
    RIL_Client_Header * client; //MUST in the beginning
} RIL_Token_Header;

typedef enum {
    RADIO_TEMPSTATE_AVAILABLE = 0,	/* Radio available */
    RADIO_TEMPSTATE_UNAVAILABLE = 1,	/* Radio unavailable temporarily */
} RIL_RadioTempState;

typedef enum {
    ApplyRC_NONEED,
    ApplyRC_SUCCESS,
    APPLYRC_FAIL
} ApplyRadioCapabilityResult;

typedef struct
{
	int	used;
	int total;
} RIL_SMS_Memory_Status;

typedef struct {
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
    char *aidPtr;   /* AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value. */
    int sessionId;    /* Channel, which is used to get files from multi-application */
} RIL_SIM_IO_EX_v6;

/* Used by RIL_UNSOL_CRSS_NOTIFICATION */

typedef struct {
    int    code;            /*
                             * 0: +CCWA
                             * 1: +CDIP
                             * 2: +CLIP
                             * 3: +COLP
                             */
    int    type;            /* type of address octet in integer format (refer GSM 04.08 [8] subclause 10.5.4.7) */
    char * number;          /* string type phone number of format specified by <type> */
    char * alphaid;         /*
                             * optional string type alphanumeric representation of <number>
                             * corresponding to the entry found in phonebook;
                             */
    int    cli_validity;    /* CLI validity value,
                               0: PRESENTATION_ALLOWED,
                               1: PRESENTATION_RESTRICTED,
                               2: PRESENTATION_UNKNOWN
                            */
} RIL_CrssNotification;


#define RIL_HARDWARE_CONFIG_UUID_LENGTH 64

typedef struct {
	int format;
	int vp;
	int pid;
	int dcs;
} RIL_SmsParams;

typedef struct {
    int mode;
    char *channelConfigInfo;
    char *languageConfigInfo;
    int isAllLanguageOn;
} RIL_CBConfigInfo;

typedef struct {
	int warningType;
	int messageId;
	int serialNumber;
	char *plmnId;
	char *securityInfo;
} RIL_CBEtwsNotification;

//[New R8 modem FD]
typedef struct {
    int args_num; //record total number of arguments of this mode
    int mode; //allowed mode:0,1,2,3
    int parameter1; //only mode 2 and 3 has parameter1 this field
    int parameter2;	//only mode2 has parameter2 this field
} RIL_FDModeStructure;

/// M: [mtk03923][20111123][ALPS00093395]ICS Migration - Telephony. @{
typedef enum {
    RIL_PHB_ADN = 0,
    RIL_PHB_FDN = 1,
    RIL_PHB_MSISDN = 2,
    RIL_PHB_ECC = 3
} RIL_PhbStorageType;

typedef struct {
    int type;                       /* type of the entry, refer to RIL_PhbStorageType */
    int index;                     /* the stored index of the entry */
    char *number;              /* the phone number */
    int ton;                        /* type of the number */
    char * alphaId;             /* the alpha ID, using Hexdecimal coding method */
} RIL_PhbEntryStrucutre;
/// @}
typedef struct {
    char * storage;
    int used;
    int total;
} RIL_PHB_MEM_STORAGE_RESPONSE;

typedef struct {
    int index; // values in the range of location numbers of phonebook memory
    char * number; // phone number of format <type>
    int type; // type of address octet in integer format; default 145 when number include "+", otherwise 129
    char * text; // associated with the number
    int hidden; //0
    char * group;//indicating a group the entry may belong to
    char * adnumber; // an additional number of format<adtype>
    int adtype;//
    char * secondtext; // a second text field associated with the number
    char * email;// email field
} RIL_PHB_ENTRY;

//General SIM Authentication
typedef struct {
    int sessionId;
    int mode;
    char * param1;
    char * param2;
    int tag;
} RIL_SimAuthStructure;

// External SIM [Start]
typedef struct VsimMessage {
    uint32_t transaction_id;
    uint32_t message_id;
    uint32_t slotId;
    uint32_t length;
} __attribute__((packed)) VsimMessage;

typedef struct RIL_VsimEvent {
    uint32_t transaction_id;
    uint32_t eventId;
    uint32_t sim_type; //0: local sim, 1: remote sim
} RIL_VsimEvent;

typedef struct RIL_VsimOperationEvent {
    uint32_t transaction_id;
    uint32_t eventId; //0: ATR, 1: APDU
    int32_t result; // 0: successful, < 0: error cause
    uint32_t data_length;
    char *data;  // It is ATR in case of reset response, APDU incase of APDU request
} RIL_VsimOperationEvent;

enum MessageId {
    MSG_ID_INITIALIZATION = 1,
    MSG_ID_GET_PLATFORM_CAPABILITY = 2,
    MSG_ID_EVENT_REQUEST = 3,
    MSG_ID_UICC_RESET_RESPONSE = 4,
    MSG_ID_UICC_APDU_RESPONSE = 5,
    MSG_ID_UICC_POWER_DOWN_RESPONSE = 6,
    MSG_ID_GET_SERVICE_STATE_REQUEST = 7,

    MSG_ID_INITIALIZATION_RESPONSE = 1001,
    MSG_ID_GET_PLATFORM_CAPABILITY_RESPONSE = 1002,
    MSG_ID_EVENT_RESPONSE = 1003,
    MSG_ID_UICC_RESET_REQUEST = 1004,
    MSG_ID_UICC_APDU_REQUEST = 1005,
    MSG_ID_UICC_POWER_DOWN_REQUEST = 1006,
    MSG_ID_GET_SERVICE_STATE_RESPONSE = 1007,
};

enum EventTypeId {
    REQUEST_TYPE_ENABLE_EXTERNAL_SIM = 1,
    REQUEST_TYPE_DISABLE_EXTERNAL_SIM = 2,
    REQUEST_TYPE_PLUG_OUT = 3,
    REQUEST_TYPE_PLUG_IN = 4,
    REQUEST_TYPE_ATR_EVENT = 1,
    REQUEST_TYPE_APDU_EVENT = 2,
    REQUEST_TYPE_CARD_POWER_DOWN = 3,
};
// External SIM [End]

/// M: [C2K] IRAT feature. @{
typedef struct {
	int sourceRat;	// Source RAT
	int targetRat;	// Target RAT
	int action;		// IRAT action
	int type;		// IRAT type, from source RAT to target RAT
}RIL_Pdn_IratInfo;
/// @}

typedef enum {
    GSMPHONE,
    IMSPHONE
} DIAL_SOURCE;

#define RIL_REQUEST_VENDOR_BASE 2000

/**
 * RIL_REQUEST_GET_COLP
 *
 * Gets current COLP status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.8
 * ((int *)data)[1] is "m" parameter from TS 27.007 7.8
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_COLP (RIL_REQUEST_VENDOR_BASE + 0)


/**
 * RIL_REQUEST_SET_COLP
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.8
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_COLP (RIL_REQUEST_VENDOR_BASE + 1)

/**
 * RIL_REQUEST_GET_COLR
 *
 * Gets current COLR status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter for provision status (0: Not Provisioned 1: Provisioned 2: Unknown)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_COLR (RIL_REQUEST_VENDOR_BASE + 2)

/**
 * RIL_REQUEST_GET_CCM
 *
 * get current call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the CCM
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CCM (RIL_REQUEST_VENDOR_BASE + 3)

/**
 * RIL_REQUEST_GET_ACM
 *
 * get accumulated call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the ACM
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_ACM (RIL_REQUEST_VENDOR_BASE + 4)

/**
 * RIL_REQUEST_GET_ACMMAX
 *
 * get the maximum of accumulated call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the ACMMAX
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_ACMMAX (RIL_REQUEST_VENDOR_BASE + 5)

/**
 * RIL_REQUEST_GET_PPU_AND_CURRENCY
 *
 * get price per unit and currency
 *
 * "data" is NULL
 * "response" is a const char * containing the
 * price per unit and currency
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_PPU_AND_CURRENCY (RIL_REQUEST_VENDOR_BASE + 6)

/**
 * RIL_REQUEST_SET_ACMMAX
 *
 * set maximum value of ACM
 *
 * ((const char **)data)[0] = acmmax
 * ((const char **)data)[1] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_ACMMAX (RIL_REQUEST_VENDOR_BASE + 7)

/**
 * RIL_REQUEST_RESET_ACM
 *
 * reset value of ACM
 *
 * ((const char **)data)[0] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_RESET_ACM (RIL_REQUEST_VENDOR_BASE + 8)

/**
 * RIL_REQUEST_SET_PPU_AND_CURRENCY
 *
 * set ppu and currency
 *
 * ((const char **)data)[0] = currency
 * ((const char **)data)[1] = ppu
 * ((const char **)data)[2] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_PPU_AND_CURRENCY (RIL_REQUEST_VENDOR_BASE + 9)


/**
 * RIL_REQUEST_MODEM_POWEROFF
 *
 * Power off modem
 * "data" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_MODEM_POWEROFF (RIL_REQUEST_VENDOR_BASE + 10)

/**
 * RIL_REQUEST_DUAL_SIM_MODE_SWITCH
 *
 * Dual SIM mode switch
 * "data" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DUAL_SIM_MODE_SWITCH (RIL_REQUEST_VENDOR_BASE + 11)

/**
 * RIL_REQUEST_QUERY_PHB_STORAGE_INFO
 *
 * query the information of specified storage
 *
 * "data" is int *
 * ((int *)data)[0] is the type of the storage, refer to RIL_PhbStorageType
 *
 * "response" is a "int *"
 * ((int *)response)[0] is current number of used entries of the storage.
 * ((int *)response)[1] is number of total entries of the storage.
 * ((int *)response)[2] is maximum supported length of phone number.
 * ((int *)response)[3] is maximum supported length of alpha id.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_PHB_STORAGE_INFO (RIL_REQUEST_VENDOR_BASE + 12)

/**
 * RIL_REQUEST_WRITE_PHB_ENTRY
 *
 * Write a phb entry into specified storage
 *
 * "data" is a const RIL_PhbEntryStrucutre *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  DIAL_STRING_TOO_LONG
 *  TEXT_STRING_TOO_LONG
 *  SIM_PIN2
 *  SIM_PUK2
 *  SIM_MEM_FULL
 */
#define RIL_REQUEST_WRITE_PHB_ENTRY (RIL_REQUEST_VENDOR_BASE + 13)

/**
 * RIL_REQUEST_READ_PHB_ENTRY
 *
 * Read a phb entry from specified storage and index
 *
 * "data" is int *
 * ((int *)data)[0] is the type of the storage, refer to RIL_PhbStorageType
 * ((int *)data)[1] is the begin index of the storage to be read
 * ((int *)data)[2] is the end index of the storage to be read
 *
 * "response" is "RIL_PhbEntryStrucutre **"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_READ_PHB_ENTRY (RIL_REQUEST_VENDOR_BASE + 14)

/**
 * RIL_REQUEST_SET_GPRS_CONNECT_TYPE
 *
 * Set GPRS connect type
 *
 * "data" is a int
  * ((int *)data)[0]  0:WHEN_NEEDED , 1: ALWAYS
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_GPRS_CONNECT_TYPE (RIL_REQUEST_VENDOR_BASE + 15)

/**
 * RIL_REQUEST_SET_GPRS_TRANSFER_TYPE
 *
 * Set GPRS connect type
 *
 * "data" is a int
  * ((int *)data)[0]  0:data prefer , 1: call prefer
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_GPRS_TRANSFER_TYPE (RIL_REQUEST_VENDOR_BASE + 16)

#define RIL_REQUEST_MOBILEREVISION_AND_IMEI  (RIL_REQUEST_VENDOR_BASE + 17)
#define RIL_REQUEST_QUERY_SIM_NETWORK_LOCK	(RIL_REQUEST_VENDOR_BASE + 18)
#define RIL_REQUEST_SET_SIM_NETWORK_LOCK	(RIL_REQUEST_VENDOR_BASE + 19)
#define RIL_REQUEST_SET_SCRI	(RIL_REQUEST_VENDOR_BASE + 20)

#define RIL_REQUEST_BTSIM_CONNECT     (RIL_REQUEST_VENDOR_BASE + 21)
#define RIL_REQUEST_BTSIM_DISCONNECT_OR_POWEROFF   (RIL_REQUEST_VENDOR_BASE + 22)
#define RIL_REQUEST_BTSIM_POWERON_OR_RESETSIM   (RIL_REQUEST_VENDOR_BASE + 23)
#define RIL_REQUEST_BTSIM_TRANSFERAPDU   (RIL_REQUEST_VENDOR_BASE + 24)

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT
 *
  * Manually select a specified network.
  *
  * "data" is const char * specifying MCCMNC of network to select (eg "310170")
  * and a int specifying Act type. (eg 0 is GSM)
  * "response" is NULL
  *
  * This request must not respond until the new operator is selected
  * and registered
  *
  * Valid errors:
  *  SUCCESS
  *  RADIO_NOT_AVAILABLE
  *  ILLEGAL_SIM_OR_ME
  *  GENERIC_FAILURE
  *
  * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
  * 	  no retries needed, such as illegal SIM or ME.
  * 	  Returns GENERIC_FAILURE for all other causes that might be
  * 	  fixed by retries.
  *
  */
#define RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT   (RIL_REQUEST_VENDOR_BASE + 25)

/**
 * RIL_REQUEST_QUERY_ICCID
 *
  */
#define RIL_REQUEST_QUERY_ICCID   (RIL_REQUEST_VENDOR_BASE + 26)
#define RIL_REQUEST_USIM_AUTHENTICATION      (RIL_REQUEST_VENDOR_BASE + 27)

#define RIL_REQUEST_MODEM_POWERON   (RIL_REQUEST_VENDOR_BASE + 28)

/**
 * RIL_REQUEST_GET_SMS_SIM_MEM_STATUS
 *
  * GET SMS SIM Card Memory's total and used number.
  *
  * "data" is const RIL_SMS_Memory_Status, contain the total and used SIM Card SMS number.
  * "response" is NULL.
  *
  * Valid errors:
  *  SUCCESS
  *  RADIO_NOT_AVAILABLE
  *  GENERIC_FAILURE
  */
#define RIL_REQUEST_GET_SMS_SIM_MEM_STATUS   (RIL_REQUEST_VENDOR_BASE + 29)
#define RIL_REQUEST_GET_PHONE_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 30)
#define RIL_REQUEST_SET_PHONE_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 31)
#define RIL_REQUEST_GET_POL_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 32)
#define RIL_REQUEST_GET_POL_LIST (RIL_REQUEST_VENDOR_BASE + 33)
#define RIL_REQUEST_SET_POL_ENTRY (RIL_REQUEST_VENDOR_BASE + 34)
#define RIL_REQUEST_QUERY_UPB_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 35)
#define RIL_REQUEST_EDIT_UPB_ENTRY (RIL_REQUEST_VENDOR_BASE + 36)
#define RIL_REQUEST_DELETE_UPB_ENTRY (RIL_REQUEST_VENDOR_BASE + 37)
#define RIL_REQUEST_READ_UPB_GAS_LIST (RIL_REQUEST_VENDOR_BASE + 38)
#define RIL_REQUEST_READ_UPB_GRP  (RIL_REQUEST_VENDOR_BASE + 39)
#define RIL_REQUEST_WRITE_UPB_GRP  (RIL_REQUEST_VENDOR_BASE + 40)
#define RIL_REQUEST_SET_SIM_RECOVERY_ON (RIL_REQUEST_VENDOR_BASE + 41)
#define RIL_REQUEST_GET_SIM_RECOVERY_ON (RIL_REQUEST_VENDOR_BASE + 42)
#define RIL_REQUEST_SET_TRM (RIL_REQUEST_VENDOR_BASE + 43)
#define RIL_REQUEST_DETECT_SIM_MISSING (RIL_REQUEST_VENDOR_BASE + 44)

/* whether download calibration data or not */
#define RIL_REQUEST_GET_CALIBRATION_DATA (RIL_REQUEST_VENDOR_BASE + 45)
#define RIL_REQUEST_GET_PHB_STRING_LENGTH (RIL_REQUEST_VENDOR_BASE + 46)

/**
 * at+cpbs?
 */
#define RIL_REQUEST_GET_PHB_MEM_STORAGE (RIL_REQUEST_VENDOR_BASE + 47)

/**
 * at+cpbs=<storage><passwork>
 */
#define RIL_REQUEST_SET_PHB_MEM_STORAGE (RIL_REQUEST_VENDOR_BASE + 48)

/**
 * at+cpbr=<index1>,<index2>
 * +CPBR:<indexn>,<number>,<type>,<text>,<hidden>,<group>,<adnumber>,<adtype>,<secondtext>,<email>
 * see RIL_PHB_ENTRY
 */
#define RIL_REQUEST_READ_PHB_ENTRY_EXT (RIL_REQUEST_VENDOR_BASE + 49)
/**
 * at+cpbw=<index>,<number>,<type>,<text>,<hidden>,<group>,<adnumber>,<adtype>,<secondtext>,<email>
 * see RIL_PHB_ENTRY
 */
#define RIL_REQUEST_WRITE_PHB_ENTRY_EXT (RIL_REQUEST_VENDOR_BASE + 50)

//support LGE API, 3gpp 27.007
/*
* Get sms revelant parameters from EFsmsp
*/
#define RIL_REQUEST_GET_SMS_PARAMS (RIL_REQUEST_VENDOR_BASE + 51)

/*
* Set sms revelant parameters into EFsmsp
*/
#define RIL_REQUEST_SET_SMS_PARAMS (RIL_REQUEST_VENDOR_BASE + 52)

// NFC SEEK start
// "data" is a const RIL_SIM_IO *
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_TRANSMIT_BASIC (RIL_REQUEST_VENDOR_BASE + 53)

// "data" is a const RIL_SIM_IO *
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_TRANSMIT_CHANNEL (RIL_REQUEST_VENDOR_BASE + 54)

// "data" is NULL *
// "response" is a const char * containing ATR in hexadecimal format
#define RIL_REQUEST_SIM_GET_ATR (RIL_REQUEST_VENDOR_BASE + 55)
// NFC SEEK end

// SMS part, CB extension
#define RIL_REQUEST_SET_CB_CHANNEL_CONFIG_INFO   (RIL_REQUEST_VENDOR_BASE + 56)
#define RIL_REQUEST_SET_CB_LANGUAGE_CONFIG_INFO  (RIL_REQUEST_VENDOR_BASE + 57)
#define RIL_REQUEST_GET_CB_CONFIG_INFO           (RIL_REQUEST_VENDOR_BASE + 58)
#define RIL_REQUEST_SET_ALL_CB_LANGUAGE_ON       (RIL_REQUEST_VENDOR_BASE + 59)
// SMS part, CB extension

#define RIL_REQUEST_SET_ETWS (RIL_REQUEST_VENDOR_BASE + 60)

//[New R8 modem FD]
//"data" is a const RIL_FD_MODE *
//"response" is a int
#define RIL_REQUEST_SET_FD_MODE (RIL_REQUEST_VENDOR_BASE + 61)

// detach PS service request
#define RIL_REQUEST_DETACH_PS (RIL_REQUEST_VENDOR_BASE + 62)

// NFC seek
// "data" is a const char * containing the AID of the applet
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_OPEN_CHANNEL_WITH_SW (RIL_REQUEST_VENDOR_BASE + 63) // NFC SEEK

#define RIL_REQUEST_SET_REG_SUSPEND_ENABLED (RIL_REQUEST_VENDOR_BASE + 64)
#define RIL_REQUEST_RESUME_REGISTRATION (RIL_REQUEST_VENDOR_BASE + 65)
#define RIL_REQUEST_STORE_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 66)
#define RIL_REQUEST_QUERY_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 67)

#define RIL_REQUEST_SIM_INTERFACE_SWITCH (RIL_REQUEST_VENDOR_BASE + 68)

// WiFi Calling start
#define RIL_REQUEST_UICC_SELECT_APPLICATION (RIL_REQUEST_VENDOR_BASE + 69)

#define RIL_REQUEST_UICC_DEACTIVATE_APPLICATION (RIL_REQUEST_VENDOR_BASE + 70)

#define RIL_REQUEST_UICC_APPLICATION_IO (RIL_REQUEST_VENDOR_BASE + 71)

#define RIL_REQUEST_UICC_AKA_AUTHENTICATE (RIL_REQUEST_VENDOR_BASE + 72)

#define RIL_REQUEST_UICC_GBA_AUTHENTICATE_BOOTSTRAP (RIL_REQUEST_VENDOR_BASE + 73)

#define RIL_REQUEST_UICC_GBA_AUTHENTICATE_NAF (RIL_REQUEST_VENDOR_BASE + 74)

#define RIL_REQUEST_STK_EVDL_CALL_BY_AP (RIL_REQUEST_VENDOR_BASE + 75)

// Femtocell (CSG)
#define RIL_REQUEST_GET_FEMTOCELL_LIST (RIL_REQUEST_VENDOR_BASE + 76)
#define RIL_REQUEST_ABORT_FEMTOCELL_LIST (RIL_REQUEST_VENDOR_BASE + 77)
#define RIL_REQUEST_SELECT_FEMTOCELL (RIL_REQUEST_VENDOR_BASE + 78)
//Network OPLMN
#define RIL_REQUEST_SEND_OPLMN (RIL_REQUEST_VENDOR_BASE + 79)
#define RIL_REQUEST_GET_OPLMN_VERSION (RIL_REQUEST_VENDOR_BASE + 80)
//PLMN List abort
#define RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS (RIL_REQUEST_VENDOR_BASE + 81)
// Dial up for CSD
#define RIL_REQUEST_DIAL_UP_CSD (RIL_REQUEST_VENDOR_BASE + 82)
#define RIL_REQUEST_SET_TELEPHONY_MODE (RIL_REQUEST_VENDOR_BASE + 83)

/* M: call control part start */
/**
 * RIL_REQUEST_HANGUP_ALL
 *
 * Hang up all (like ATH, but use AT+CHLD=6 to prevent channel limitation)
 * For ATH, the channel usd to setup call and release must be the same.
 * AT+CHLD=6 has no such limitation
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_HANGUP_ALL (RIL_REQUEST_VENDOR_BASE + 84)
#define RIL_REQUEST_FORCE_RELEASE_CALL (RIL_REQUEST_VENDOR_BASE + 85)
#define RIL_REQUEST_SET_CALL_INDICATION (RIL_REQUEST_VENDOR_BASE + 86)

/**
 * RIL_REQUEST_EMERGENCY_DIAL
 *
 * Initiate emergency call
 *
 * "data" is const RIL_Dial *
 * "response" is NULL
 *
 * This method is never used for supplementary service codes
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_EMERGENCY_DIAL (RIL_REQUEST_VENDOR_BASE + 87)
#define RIL_REQUEST_SET_ECC_SERVICE_CATEGORY (RIL_REQUEST_VENDOR_BASE + 88)
#define RIL_REQUEST_SET_ECC_LIST (RIL_REQUEST_VENDOR_BASE + 89)
/* M: call control part end */

//New SIM Authentication
#define RIL_REQUEST_GENERAL_SIM_AUTH (RIL_REQUEST_VENDOR_BASE + 90)
//ISIM
#define RIL_REQUEST_OPEN_ICC_APPLICATION (RIL_REQUEST_VENDOR_BASE + 91)
#define RIL_REQUEST_GET_ICC_APPLICATION_STATUS (RIL_REQUEST_VENDOR_BASE + 92)
#define RIL_REQUEST_SIM_IO_EX (RIL_REQUEST_VENDOR_BASE + 93)

// IMS
#define RIL_REQUEST_SET_IMS_ENABLE (RIL_REQUEST_VENDOR_BASE + 94)

/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT
 *
 * Scans for available networks with Act
 *
 * "data" is NULL
 * "response" is const char ** that should be an array of n*4 strings, where
 *    n is the number of available networks
 * For each available network:
 *
 * ((const char **)response)[n+0] is long alpha ONS or EONS (present only if NITZ operator name exist)
 * ((const char **)response)[n+1] is short alpha ONS or EONS (present only if NITZ operator name exist)
 * ((const char **)response)[n+2] is 5 or 6 digit numeric code (MCC + MNC)
 * ((const char **)response)[n+3] is a string value of the status:
 *           "unknown"
 *           "available"
 *           "current"
 *           "forbidden"
 * ((const char **)response)[n+4] is a string value of the Act: "2G","3G","4G"
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  RIL_E_CANCELLED
 *
 */
#define RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT (RIL_REQUEST_VENDOR_BASE + 95)

/* M: SS part */
///M: For query CNAP
#define RIL_REQUEST_SEND_CNAP (RIL_REQUEST_VENDOR_BASE + 96)
#define RIL_REQUEST_SET_CLIP (RIL_REQUEST_VENDOR_BASE + 97)
/* M: SS part end */

/** M: VoLTE data start */
#define RIL_REQUEST_SETUP_DEDICATE_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 98)
#define RIL_REQUEST_DEACTIVATE_DEDICATE_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 99)
#define RIL_REQUEST_MODIFY_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 100)
#define RIL_REQUEST_ABORT_SETUP_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 101)
#define RIL_REQUEST_PCSCF_DISCOVERY_PCO (RIL_REQUEST_VENDOR_BASE + 102)
#define RIL_REQUEST_CLEAR_DATA_BEARER (RIL_REQUEST_VENDOR_BASE + 103)
/** M: VoLTE data end */

// SMS part, CB extension
#define RIL_REQUEST_REMOVE_CB_MESSAGE (RIL_REQUEST_VENDOR_BASE + 104)
// SMS part, CB extension

// NAS configuration for voice call
// 0: voice centric
// 1: data centric
#define RIL_REQUEST_SET_DATA_CENTRIC (RIL_REQUEST_VENDOR_BASE + 105)

/// M: IMS feature. @{
#define RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER (RIL_REQUEST_VENDOR_BASE + 106)
#define RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER (RIL_REQUEST_VENDOR_BASE + 107)
#define RIL_REQUEST_DIAL_WITH_SIP_URI (RIL_REQUEST_VENDOR_BASE + 108)
#define RIL_REQUEST_RESUME_CALL (RIL_REQUEST_VENDOR_BASE + 109)
/// @}

//M: call control part - Speech Codec Info for HD voice icon
#define RIL_REQUEST_SET_SPEECH_CODEC_INFO (RIL_REQUEST_VENDOR_BASE + 110)

/// M: CC33 LTE
#define RIL_REQUEST_SET_DATA_ON_TO_MD (RIL_REQUEST_VENDOR_BASE + 111)
#define RIL_REQUEST_SET_REMOVE_RESTRICT_EUTRAN_MODE (RIL_REQUEST_VENDOR_BASE + 112)

/* M: call control part start */
#define RIL_REQUEST_SET_IMS_CALL_STATUS (RIL_REQUEST_VENDOR_BASE + 113)
/* M: call control part end */

/// M: For 3G VT only @{
#define RIL_REQUEST_VT_DIAL (RIL_REQUEST_VENDOR_BASE + 114)
#define RIL_REQUEST_VOICE_ACCEPT (RIL_REQUEST_VENDOR_BASE + 115)
#define RIL_REQUEST_REPLACE_VT_CALL (RIL_REQUEST_VENDOR_BASE + 116)
/// @}

// C2K SVLTE remote SIM access
#define RIL_REQUEST_CONFIG_MODEM_STATUS (RIL_REQUEST_VENDOR_BASE + 117)

/// M: [C2K] MD IRAT feature. @{
#define RIL_REQUEST_SET_ACTIVE_PS_SLOT (RIL_REQUEST_VENDOR_BASE + 118)
#define RIL_REQUEST_CONFIRM_INTER_3GPP_IRAT_CHANGE (RIL_REQUEST_VENDOR_BASE + 119)
/// @}

/// M: [C2K][SVLTE] Set the SVLTE RAT mode. @{
#define RIL_REQUEST_SET_SVLTE_RAT_MODE (RIL_REQUEST_VENDOR_BASE + 120)
/// M: [C2K][SVLTE] Set the SVLTE RAT mode. @}

/// M: [C2K] AP IRAT feature. @{
#define RIL_REQUEST_TRIGGER_LTE_BG_SEARCH (RIL_REQUEST_VENDOR_BASE + 121)
/// @}

/// M: VoLTE conference dial feature. @{
#define RIL_REQUEST_CONFERENCE_DIAL (RIL_REQUEST_VENDOR_BASE + 122)
/// @}


/// M: CC072: Add Customer proprietary-IMS RIL interface. @{
/* To transfer IMS call context to modem */
#define RIL_REQUEST_SET_SRVCC_CALL_CONTEXT_TRANSFER (RIL_REQUEST_VENDOR_BASE + 123)
/* To update IMS registration status to modem */
#define RIL_REQUEST_UPDATE_IMS_REGISTRATION_STATUS (RIL_REQUEST_VENDOR_BASE + 124)
/// @}

#define RIL_REQUEST_RELOAD_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 125)

/// M: IMS feature. @{
#define RIL_REQUEST_HOLD_CALL (RIL_REQUEST_VENDOR_BASE + 126)
/// @}

/// M: [C2K][SVLTE] Set the STK UTK mode. @{
#define RIL_REQUEST_SET_STK_UTK_MODE (RIL_REQUEST_VENDOR_BASE + 127)
/// M: [C2K][SVLTE] Set the STK UTK mode. @}

#define RIL_REQUEST_SWITCH_ANTENNA (RIL_REQUEST_VENDOR_BASE + 128)

/// M: [C2K] Deactivate link down PDN for CT requirement. @{
#define RIL_REQUEST_DEACTIVATE_LINK_DOWN_PDN (RIL_REQUEST_VENDOR_BASE + 129)
///@}

#define RIL_REQUEST_AT_COMMAND_WITH_PROXY (RIL_REQUEST_VENDOR_BASE + 130)
/// M: [C2K] Switch ruim to sim or sim to ruim @{
#define RIL_REQUEST_SWITCH_CARD_TYPE (RIL_REQUEST_VENDOR_BASE + 131)
/// @}

/// M: [C2K] Enable or disable MD3 Sleep @{
#define RIL_REQUEST_ENABLE_MD3_SLEEP (RIL_REQUEST_VENDOR_BASE + 132)
/// @}

/// M: ViLTE feature. @{
#define RIL_REQUEST_VIDEO_CALL_ACCEPT (RIL_REQUEST_VENDOR_BASE + 133)
/// @}

// M: [LTE][Low Power][UL traffic shaping] Start
#define RIL_REQUEST_SET_LTE_ACCESS_STRATUM_REPORT (RIL_REQUEST_VENDOR_BASE + 134)
#define RIL_REQUEST_SET_LTE_UPLINK_DATA_TRANSFER (RIL_REQUEST_VENDOR_BASE + 135)
// M: [LTE][Low Power][UL traffic shaping] End

/// M: [EPDG] Hand over ind @{
#define RIL_REQUEST_HANDOVER_IND (RIL_REQUEST_VENDOR_BASE + 136)
/// @}

/******************************************************************/

#define RIL_LOCAL_REQUEST_VENDOR_BASE 2500

#define RIL_LOCAL_REQUEST_SIM_AUTHENTICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 0)
#define RIL_LOCAL_REQUEST_USIM_AUTHENTICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 1)
#define RIL_LOCAL_REQUEST_QUERY_MODEM_THERMAL (RIL_LOCAL_REQUEST_VENDOR_BASE + 2)
#define RIL_LOCAL_REQUEST_RILD_READ_IMSI (RIL_LOCAL_REQUEST_VENDOR_BASE + 3)

/* SIM ME Lock added request id */
#define RIL_LOCAL_REQUEST_GET_SHARED_KEY (RIL_LOCAL_REQUEST_VENDOR_BASE + 4)
#define RIL_LOCAL_REQUEST_UPDATE_SIM_LOCK_SETTINGS (RIL_LOCAL_REQUEST_VENDOR_BASE + 5)
#define RIL_LOCAL_REQUEST_GET_SIM_LOCK_INFO (RIL_LOCAL_REQUEST_VENDOR_BASE + 6)
#define RIL_LOCAL_REQUEST_RESET_SIM_LOCK_SETTINGS (RIL_LOCAL_REQUEST_VENDOR_BASE + 7)
#define RIL_LOCAL_REQUEST_GET_MODEM_STATUS (RIL_LOCAL_REQUEST_VENDOR_BASE + 8)

/* Request modem on/off from thermal */
#define RIL_LOCAL_REQUEST_SET_MODEM_THERMAL (RIL_LOCAL_REQUEST_VENDOR_BASE + 9)
/* External SIM request id*/
#define RIL_LOCAL_REQUEST_VSIM_NOTIFICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 10)
#define RIL_LOCAL_REQUEST_VSIM_OPERATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 11)

/******************************************************************/

#define RIL_UNSOL_VENDOR_BASE 3000

#define RIL_UNSOL_NEIGHBORING_CELL_INFO (RIL_UNSOL_VENDOR_BASE + 0)

#define RIL_UNSOL_NETWORK_INFO (RIL_UNSOL_VENDOR_BASE + 1)

#define RIL_UNSOL_PHB_READY_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 2)

#define RIL_UNSOL_SIM_INSERTED_STATUS (RIL_UNSOL_VENDOR_BASE + 3)

#define RIL_UNSOL_RADIO_TEMPORARILY_UNAVAILABLE (RIL_UNSOL_VENDOR_BASE + 4)

#define RIL_UNSOL_ME_SMS_STORAGE_FULL (RIL_UNSOL_VENDOR_BASE + 5)

#define RIL_UNSOL_SMS_READY_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 6)

#define RIL_UNSOL_SCRI_RESULT (RIL_UNSOL_VENDOR_BASE + 7)

#define RIL_UNSOL_SIM_MISSING (RIL_UNSOL_VENDOR_BASE + 8)

#define RIL_UNSOL_GPRS_DETACH (RIL_UNSOL_VENDOR_BASE + 9)

#define RIL_UNSOL_ATCI_RESPONSE (RIL_UNSOL_VENDOR_BASE + 10)

#define RIL_UNSOL_SIM_RECOVERY (RIL_UNSOL_VENDOR_BASE + 11)
#define RIL_UNSOL_VIRTUAL_SIM_ON (RIL_UNSOL_VENDOR_BASE + 12)
#define RIL_UNSOL_VIRTUAL_SIM_OFF (RIL_UNSOL_VENDOR_BASE + 13)
#define RIL_UNSOL_INVALID_SIM (RIL_UNSOL_VENDOR_BASE + 14)
#define RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED (RIL_UNSOL_VENDOR_BASE + 15)

#define RIL_UNSOL_RESPONSE_ACMT (RIL_UNSOL_VENDOR_BASE + 16)

#define RIL_UNSOL_EF_CSP_PLMN_MODE_BIT (RIL_UNSOL_VENDOR_BASE + 17)
#define RIL_UNSOL_IMEI_LOCK (RIL_UNSOL_VENDOR_BASE + 18)

#define RIL_UNSOL_RESPONSE_MMRR_STATUS_CHANGED (RIL_UNSOL_VENDOR_BASE + 19)
#define RIL_UNSOL_SIM_PLUG_OUT (RIL_UNSOL_VENDOR_BASE + 20)
#define RIL_UNSOL_SIM_PLUG_IN (RIL_UNSOL_VENDOR_BASE + 21)
#define RIL_UNSOL_RESPONSE_ETWS_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 22)

#define RIL_UNSOL_RESPONSE_PLMN_CHANGED (RIL_UNSOL_VENDOR_BASE + 23)
#define RIL_UNSOL_RESPONSE_REGISTRATION_SUSPENDED (RIL_UNSOL_VENDOR_BASE + 24)

#define RIL_UNSOL_STK_EVDL_CALL (RIL_UNSOL_VENDOR_BASE + 25)

#define RIL_UNSOL_DATA_PACKETS_FLUSH (RIL_UNSOL_VENDOR_BASE + 26)

#define RIL_UNSOL_FEMTOCELL_INFO (RIL_UNSOL_VENDOR_BASE + 27)
#define RIL_UNSOL_STK_SETUP_MENU_RESET (RIL_UNSOL_VENDOR_BASE + 28)
#define RIL_UNSOL_APPLICATION_SESSION_ID_CHANGED (RIL_UNSOL_VENDOR_BASE + 29)
#define RIL_UNSOL_ECONF_SRVCC_INDICATION (RIL_UNSOL_VENDOR_BASE + 30)

// IMS
#define RIL_UNSOL_IMS_ENABLE_DONE (RIL_UNSOL_VENDOR_BASE + 31)
#define RIL_UNSOL_IMS_DISABLE_DONE (RIL_UNSOL_VENDOR_BASE + 32)
#define RIL_UNSOL_IMS_REGISTRATION_INFO (RIL_UNSOL_VENDOR_BASE + 33)

//VoLTE
#define RIL_UNSOL_DEDICATE_BEARER_ACTIVATED (RIL_UNSOL_VENDOR_BASE + 34)
#define RIL_UNSOL_DEDICATE_BEARER_MODIFIED (RIL_UNSOL_VENDOR_BASE + 35)
#define RIL_UNSOL_DEDICATE_BEARER_DEACTIVATED (RIL_UNSOL_VENDOR_BASE + 36)

// RAC update indication //sm cause rac
#define RIL_UNSOL_RAC_UPDATE (RIL_UNSOL_VENDOR_BASE + 37)

//[VoLTE]Conf. call merged/added result
#define RIL_UNSOL_ECONF_RESULT_INDICATION (RIL_UNSOL_VENDOR_BASE + 38)

//Remote SIM ME lock related APIs [Start]
#define RIL_UNSOL_MELOCK_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 39)
//Remote SIM ME lock related APIs [End]

/* M: call control part start */
#define RIL_UNSOL_CALL_FORWARDING (RIL_UNSOL_VENDOR_BASE + 40)
#define RIL_UNSOL_CRSS_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 41)
#define RIL_UNSOL_INCOMING_CALL_INDICATION (RIL_UNSOL_VENDOR_BASE + 42)
#define RIL_UNSOL_CIPHER_INDICATION (RIL_UNSOL_VENDOR_BASE + 43)
#define RIL_UNSOL_CNAP (RIL_UNSOL_VENDOR_BASE + 44) //obsolete
/* M: call control part end */
#define RIL_UNSOL_SIM_COMMON_SLOT_NO_CHANGED (RIL_UNSOL_VENDOR_BASE + 45)
//Notify if do combine attach
#define RIL_UNSOL_DATA_ALLOWED (RIL_UNSOL_VENDOR_BASE + 46)
#define RIL_UNSOL_STK_CALL_CTRL (RIL_UNSOL_VENDOR_BASE + 47)

// IMS VoLTE
// gives the last IMS Voice Over PS sessions (IMSVOPS) supported indicatyion from the network
#define RIL_UNSOL_VOLTE_EPS_NETWORK_FEATURE_SUPPORT (RIL_UNSOL_VENDOR_BASE + 48)

/// M: IMS feature. @{
#define RIL_UNSOL_CALL_INFO_INDICATION (RIL_UNSOL_VENDOR_BASE + 49)
/// @}

// IMS client on AP get the information of EPS network feature support from NAS signaling and SIB message
#define RIL_UNSOL_VOLTE_EPS_NETWORK_FEATURE_INFO (RIL_UNSOL_VENDOR_BASE + 50)
// IMS clinet on AP be notified for SRVCC handover event ; HandOver Types : Start / Complete / Fail / Canceled
#define RIL_UNSOL_SRVCC_HANDOVER_INFO_INDICATION (RIL_UNSOL_VENDOR_BASE + 51)
//M: call control part - Speech Codec Info for HD voice icon
#define RIL_UNSOL_SPEECH_CODEC_INFO (RIL_UNSOL_VENDOR_BASE + 52)


//MTK-START for MD state change
#define RIL_UNSOL_MD_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 53)
/**
* RIL_UNSOL_MD_STATE_CHANGE
*
* Called when modem assert
*
* "data" is const int *
* ((const int *)data)[0] contains the modem state
*
*  0: modem reset stop (MD NORMAL)// not used
*  1: modem reset start (MD ASSERT)
*
*/
//MTK-END for MD state change
// M: CC33 URC
#define RIL_UNSOL_REMOVE_RESTRICT_EUTRAN (RIL_UNSOL_VENDOR_BASE + 54)

// IMS client on AP shall get the information of MO Data Barring and SSAC barring
#define RIL_UNSOL_MO_DATA_BARRING_INFO (RIL_UNSOL_VENDOR_BASE + 55)
#define RIL_UNSOL_SSAC_BARRING_INFO (RIL_UNSOL_VENDOR_BASE + 56)

// IMS VOLTE : send SIP related error message to AP
#define RIL_UNSOL_SIP_CALL_PROGRESS_INDICATOR (RIL_UNSOL_VENDOR_BASE + 57)

#define RIL_UNSOL_ABNORMAL_EVENT (RIL_UNSOL_VENDOR_BASE + 58)

/// M: CC071: Add Customer proprietary-IMS RIL interface. @{
#define RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY (RIL_UNSOL_VENDOR_BASE + 59)
/// @}

/// M: [C2K] MD IRAT feature. @{
#define RIL_UNSOL_INTER_3GPP_IRAT_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 60)
/// @}
/// M: [C2K] AP IRAT feature. @{
#define RIL_UNSOL_LTE_BG_SEARCH_STATUS (RIL_UNSOL_VENDOR_BASE + 61)
/// @}

/// M: for GMSS RAT
#define RIL_UNSOL_GMSS_RAT_CHANGED (RIL_UNSOL_VENDOR_BASE + 62)

#define RIL_UNSOL_CDMA_CARD_TYPE (RIL_UNSOL_VENDOR_BASE + 63)

// IMS
#define RIL_UNSOL_IMS_ENABLE_START (RIL_UNSOL_VENDOR_BASE + 64)
#define RIL_UNSOL_IMS_DISABLE_START (RIL_UNSOL_VENDOR_BASE + 65)

#define RIL_UNSOL_IMSI_REFRESH_DONE (RIL_UNSOL_VENDOR_BASE + 66)

// M: Notify RILJ that the AT+EUSIM was received
#define RIL_UNSOL_EUSIM_READY (RIL_UNSOL_VENDOR_BASE + 67)
/// M: BIP {
/**
 * RIL_UNSOL_STK_BIP_PROACTIVE_COMMAND
 *
 * Indicate when SIM issue a BIP proactive command to applications
 *
 * "data" is a const char * containing SAT/USAT proactive command
 * in hexadecimal format string starting with command tag
 *
 */
#define RIL_UNSOL_STK_BIP_PROACTIVE_COMMAND (RIL_UNSOL_VENDOR_BASE + 68)
/// M: BIP }

//WorldMode
#define RIL_UNSOL_WORLD_MODE_CHANGED (RIL_UNSOL_VENDOR_BASE + 69)

/// M: For 3G VT only @{
#define RIL_UNSOL_VT_STATUS_INFO (RIL_UNSOL_VENDOR_BASE + 70)
#define RIL_UNSOL_VT_RING_INFO (RIL_UNSOL_VENDOR_BASE + 71)
/// @}

//External SIM [Start]
#define RIL_UNSOL_VSIM_OPERATION_INDICATION (RIL_UNSOL_VENDOR_BASE + 72)
//External SIM [End]

// M: Reset Attach APN
#define RIL_UNSOL_SET_ATTACH_APN (RIL_UNSOL_VENDOR_BASE + 73)
/// M: MAL @{
#define RIL_UNSOL_MAL_AT_INFO (RIL_UNSOL_VENDOR_BASE + 74)
//info the current 3G SIM
#define RIL_UNSOL_MAIN_SIM_INFO (RIL_UNSOL_VENDOR_BASE + 75)
/// @}
// M: The event for pluggin in the tray in common slot project
#define RIL_UNSOL_TRAY_PLUG_IN (RIL_UNSOL_VENDOR_BASE + 76)

// IMS VOLTE : send call mode change message to AP
#define RIL_UNSOL_CALLMOD_CHANGE_INDICATOR (RIL_UNSOL_VENDOR_BASE + 77)

// IMS VILTE : send video capability message to AP
#define RIL_UNSOL_VIDEO_CAPABILITY_INDICATOR (RIL_UNSOL_VENDOR_BASE + 78)

// M: [LTE][Low Power][UL traffic shaping] Start
#define RIL_UNSOL_LTE_ACCESS_STRATUM_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 79)
// M: [LTE][Low Power][UL traffic shaping] End

/// M: IMS/EPDG @{
#define RIL_UNSOL_SETUP_DATA_CALL_RESPONSE (RIL_UNSOL_VENDOR_BASE + 80)
/// M: IMS/EPDG @}

typedef struct {
    int sw1;
    int sw2;
    char *simResponse;          /* Response in Base64 format, see 3GPP TS 31.102 7.1.2 */
} RIL_SimAuthenticationResponse;

#ifdef RIL_SHLIB
/**
 *  For backward compatible, RIL implementations must defined RIL_GetRilVersion
 *  Return RIL_VERSION
 *
 *
 */
int RIL_MultiSocketInRild(void);

#else /* RIL_SHLIB */

/**
 * Set Whether Srvcc Procedure for AP is on-going or done
 * @param enable is the status for AP srvcc procedure
 */
void IMS_isSrvccProcedure(int enable);

/*
get the 3G SIM info , reduce the usage of read of persist property
*/
int RIL_get3GSimInfo();

#endif /* RIL_SHLIB */

#ifdef __cplusplus
}
#endif

#endif /*MTK_RIL_H*/
