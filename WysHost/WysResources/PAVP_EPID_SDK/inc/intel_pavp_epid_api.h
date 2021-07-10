/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "intel_pavp_types.h"

typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;

#define PAVP_EPID_API_VERSION_MAJOR    3
#define PAVP_EPID_API_VERSION_MINOR    0
#define PAVP_EPID_API_VERSION          ((PAVP_EPID_API_VERSION_MAJOR << 16) | (PAVP_EPID_API_VERSION_MINOR))
#define ECDSA_PRIVKEY_LEN              32
#define ECDSA_PUBKEY_LEN               64
#define ECDSA_SIGNATURE_LEN            64
#define ECDSA_SIGNATURE_DWORDS         16         //number of DWORDS in above
#define ECDSA_MSG_DIGEST_LEN           32
#define ECDSA_MSG_DIGEST_DWORDS        8          //number of DWORDS in above
#define EPID_PARAM_LEN                 876        // EPID cryptosystem context length
#define EPID_CERT_LEN                  392        // EPID certificate length
#define EPID_PUBKEY_LEN                328        // EPID cert length - EC-DSA sig length
#define EPID_SIG_LEN                   569        // EPID signature length
#define SIGMA_PUBLIC_KEY_LEN           64
#define SIGMA_SESSION_KEY_LEN          16
#define SIGMA_MAC_KEY_LEN              16
#define SIGMA_MAC_LEN                  32
#define PAVP_EPID_STREAM_KEY_LEN       16

// PAVP EPID Commands:
#define CMD_GET_HW_ECC_PUBKEY           0x00000003
#define CMD_EXCHG_HW_APP_CERT           0x00000004
#define CMD_CLOSE_SIGMA_SESSION         0x00000005
#define CMD_GET_STREAM_KEY              0x00000006
#define CMD_INV_STREAM_KEY              0x00000007
// PAVP_EPID_API_VERISON 2.0:
#define CMD_GET_PCH_CAPABILITIES        0x00000009
#define CMD_GET_STREAM_KEY_EX           0x0000000e
#define CMD_GET_HANDLE					0x00000010

typedef unsigned char EcDsaPrivKey[ECDSA_PRIVKEY_LEN];
typedef unsigned char EcDsaPubKey[ECDSA_PUBKEY_LEN];
typedef unsigned char EcDsaSig[ECDSA_SIGNATURE_LEN];
typedef unsigned char EpidSig[EPID_SIG_LEN];
typedef unsigned char SigmaPubKey[SIGMA_PUBLIC_KEY_LEN];
typedef unsigned char SigmaSessionKey[SIGMA_SESSION_KEY_LEN];
typedef unsigned char SigmaMacKey[SIGMA_MAC_KEY_LEN];
typedef unsigned char HMAC[SIGMA_MAC_LEN];
typedef unsigned char StreamKey[PAVP_EPID_STREAM_KEY_LEN];
typedef uint32 PAVPStreamId;
typedef uint32 PAVPSessId;

typedef enum 
{
    PAVP_VIDEO_PATH = 0,
    PAVP_AUDIO_PATH
} PAVPPathId;

// PAVP EPID Status Codes
typedef enum
{
    PAVP_STATUS_SUCCESS                         = 0x0000,
    PAVP_STATUS_INTERNAL_ERROR                  = 0x1000,
    PAVP_STATUS_UNKNOWN_ERROR                   = 0x1001,
    PAVP_STATUS_INCORRECT_API_VERSION           = 0x1002,
    PAVP_STATUS_INVALID_FUNCTION                = 0x1003,
    PAVP_STATUS_INVALID_BUFFER_LENGTH           = 0x1004,
    PAVP_STATUS_INVALID_PARAMS                  = 0x1005,
    PAVP_STATUS_EPID_INVALID_PUBKEY             = 0x3000,
    PAVP_STATUS_SIGMA_SESSION_LIMIT_REACHED     = 0x3002,
    PAVP_STATUS_SIGMA_SESSION_INVALID_HANDLE    = 0x3003,
    PAVP_STATUS_SIGMA_PUBKEY_GENERATION_FAILED  = 0x3004,
    PAVP_STATUS_SIGMA_INVALID_3PCERT_HMAC       = 0x3005,
    PAVP_STATUS_SIGMA_INVALID_SIG_INTEL         = 0x3006,
    PAVP_STATUS_SIGMA_INVALID_SIG_CERT          = 0x3007,
    PAVP_STATUS_SIGMA_EXPIRED_3PCERT            = 0x3008,
    PAVP_STATUS_SIGMA_INVALID_SIG_GAGB          = 0x3009,
    PAVP_STATUS_PAVP_EPID_INVALID_PATH_ID       = 0x4000,
    PAVP_STATUS_PAVP_EPID_INVALID_STREAM_ID     = 0x4001,
    PAVP_STATUS_PAVP_EPID_STREAM_SLOT_OWNED     = 0x4002,
    PAVP_STATUS_INVALID_STREAM_KEY_SIG          = 0x4003,
    PAVP_STATUS_INVALID_TITLE_KEY_SIG           = 0x4004,
    PAVP_STATUS_INVALID_TITLE_KEY_TIME          = 0x4005,
    PAVP_STATUS_COMMAND_NOT_AUTHORIZED          = 0x4006,
    PAVP_STATUS_INVALID_DRM_TIME                = 0x4007,
    PAVP_STATUS_INVALID_TIME_SIG                = 0x4009,
    PAVP_STATUS_TIME_OVERFLOW                   = 0x400A,
    // PAVP Device Status Codes
    // Internal errors:
    PAVP_STATUS_BAD_POINTER                     = 0x5000,
    PAVP_STATUS_NOT_SUPPORTED                   = 0x5001,   // PAVP is not supported
    PAVP_STATUS_CRYPTO_DATA_GEN_ERROR           = 0x5002,   // An error happened in the Crypto Data Gen lib
    // Device state errors:
    PAVP_STATUS_PAVP_DEVICE_NOT_INITIALIZED     = 0x6000,   // Cannot perform this function without first initializing the device
    PAVP_STATUS_PAVP_SERVICE_NOT_CREATED        = 0x6001,   // Cannot perform this function without first creating PAVP service
    PAVP_STATUS_PAVP_KEY_NOT_EXCHANGED          = 0x6002,   // Cannot perform this function without first doing a key exchange
    // Key exchange protocol errors:
    PAVP_STATUS_KEY_EXCHANGE_TYPE_NOT_SUPPORTED = 0x7000,   // An invalid key exchange type was used
    PAVP_STATUS_PAVP_SERVICE_CREATE_ERROR       = 0x7001,   // A driver error occured when creating the PAVP service
    PAVP_STATUS_GET_PUBKEY_FAILED               = 0x7002,   // Failed to get g^a from PCH but no detailed error was given
    PAVP_STATUS_DERIVE_SIGMA_KEYS_FAILED        = 0x7003,   // Sigma keys could not be derived
    PAVP_STATUS_CERTIFICATE_EXCHANGE_FAILED     = 0x7004,   // Could not exchange certificates with the PCH but no detailed error was given
    PAVP_STATUS_PCH_HMAC_INVALID                = 0x7005,   // The PCH's HMAC was invalid
    PAVP_STATUS_PCH_CERTIFICATE_INVALID         = 0x7006,   // The PCH's certificate was not valid
    PAVP_STATUS_PCH_EPID_SIGNATURE_INVALID      = 0x7007,   // The PCH's EPID signature was invalid
    PAVP_STATUS_GET_STREAM_KEY_FAILED           = 0x7008,   // Failed to get a stream key from the PCH but no detailed error was given
    PAVP_STATUS_GET_CONNSTATE_FAILED            = 0x7009,   // Failed to get a connection state value from the hardware
    PAVP_STATUS_GET_CAPS_FAILED                 = 0x7010,   // Failed to get PAVP capabilities from the driver
    PAVP_STATUS_GET_FRESHNESS_FAILED            = 0x7011,   // Failed to get a key freshness value from the hardware
    PAVP_STATUS_SET_FRESHNESS_FAILED            = 0x7012,   // Failed to set the key freshness value
    PAVP_STATUS_SET_STREAM_KEY_FAILED           = 0x7013,   // Failed to update the stream key
    PAVP_STATUS_SET_PROTECTED_MEM_FAILED        = 0x7014,   // Failed to set protected memory on/off
    PAVP_STATUS_SET_PLANE_ENABLE_FAILED         = 0x7015,   // Failed to set plane enables
    PAVP_STATUS_SET_WINDOW_POSITION_FAILED      = 0x7016,   // Failed to set the window position
    PAVP_STATUS_AES_DECRYPT_FAILED              = 0x7017,   // Failed to decrypt  
    PAVP_STATUS_AES_ENCRYPT_FAILED              = 0x7018,   // Failed to encrypt  
    PAVP_STATUS_LEGACY_KEY_EXCHANGE_FAILED      = 0x7019,   // Legacy key exchange call failed
    PAVP_STATUS_INVALID_NONCE_RETURNED          = 0x701A,   // Decrypted nonce value was incorrect
    PAVP_STATUS_INVALID_MEMORY_STATUS           = 0x701B,   // Decrypted memory status was invalid
    PAVP_STATUS_API_UNSUPPORTED                 = 0x701C,   // The call is not supported with the API in use
    PAVP_STATUS_WRONG_SESSION_TYPE              = 0x701D,   // The call is not supported for the session type in use
    PAVP_STATUS_GET_HANDLE_FAILED               = 0x701E,   // Get PAVP handle failed
    PAVP_STATUS_GET_PCH_CAPS_FAILED             = 0x701F,   // Get PAVP PCH capabilities failed
} PavpEpidStatus;

#define PAVP_EPID_SUCCESS(Status) (((PavpEpidStatus)(Status)) == PAVP_STATUS_SUCCESS)
#define PAVP_EPID_FAILURE(Status) (((PavpEpidStatus)(Status)) != PAVP_STATUS_SUCCESS)

#pragma pack(push)
#pragma pack(1)

/*
**    3rd Party Certificate
*/
// Certificate Type Values for the 3p Signed CertificateType Field
/// Application's certificate
#define PAVP_EPID_PUBCERT3P_TYPE_APP         0x00000000
/// Server's certificate
#define PAVP_EPID_PUBCERT3P_TYPE_SERVER      0x00000001

// Certificate Type Values for the Intel Signed IntelSignedCertificateType Field (2.0 certificate only)
/// PAVP
#define PAVP_EPID_PUBCERT3P_TYPE_PAVP        0x00000000
/// Media Vault
#define PAVP_EPID_PUBCERT3P_TYPE_MV1_0       0x00000001

// Issuer id: Intel
#define PAVP_EPID_PUBCERT3P_ISSUER_ID        0x00000000

typedef struct tagSignBy3p 
{
    unsigned int    CertificateType;
    unsigned char   TimeValidStart[8];
    unsigned char   TimeValidEnd[8];
    unsigned int    Id3p;
    unsigned int    IssuerId;
    EcDsaPubKey     PubKey3p;
} _SignBy3p;

// PAVP 1.5 certificate structure
typedef struct _Cert3p 
{
    // 3rd Party signed part
    _SignBy3p           SignBy3p;
    EcDsaSig            Sign3p;
    // Intel signed part
    struct _SignByIntel 
    {
        unsigned char   TimeValidStart[8];
        unsigned char   TimeValidEnd[8];
        EcDsaPubKey PubKeyVerify3p;
    } SignByIntel;
    EcDsaSig            SignIntel;
} Cert3p;

// PAVP 2.0 certificate structure
typedef struct _Cert3p20
{
    // 3rd Party signed part
    _SignBy3p            SignBy3p;
    EcDsaSig                Sign3p;
    // Intel signed part
    struct _SignByIntel20
   {
        unsigned short       IntelSignedVersion;
        unsigned char        TimeValidStart[8];
        unsigned char        TimeValidEnd[8];
        unsigned short       IntelSignedCertificateType;
        EcDsaPubKey          PubKeyVerify3p;
    } SignByIntel20;
    EcDsaSig                SignIntel;
} Cert3p20;

/*
**    EPID Certificate
*/
typedef struct _EpidCert 
{
    unsigned char   PubKeyEpid[EPID_PUBKEY_LEN];
    EcDsaSig        SignIntel;
} EpidCert;

/*
**    Input/output message buffer common header
*/
typedef struct _PAVPCmdHdr 
{
    uint32    ApiVersion;
    uint32    CommandId;
    uint32    Status;
    uint32    BufferLength;
} PAVPCmdHdr;

/*
**    Command: CMD_GET_PCH_CAPABILITIES
*/

typedef struct _PAVP_PCH_Capabilities
{
    PAVP_ENCRYPTION_TYPE    AudioMode;
    PAVP_COUNTER_TYPE       AudioCounterType;
    PAVP_ENDIANNESS_TYPE    AudioCounterEndiannessType;
    unsigned int            PAVPVersion;	
    uint8                   Time[8];
} PAVP_PCH_Capabilities;
/*
**    InBuffer:
*/
typedef struct _GetCapsInBuff
{
    PAVPCmdHdr  Header;
} GetCapsInBuff;
/*
**    OutBuffer:
*/
typedef struct _GetCapsOutBuff
{
    PAVPCmdHdr              Header;
    PAVP_PCH_Capabilities   Caps;
} GetCapsOutBuff;

/*
**    Command: CMD_GET_HW_ECC_PUBKEY
**
**    InBuffer:
*/
typedef struct _GetHwEccPubKeyInBuff 
{
    PAVPCmdHdr    Header;
} GetHwEccPubKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _GetHwEccPubKeyOutBuff 
{
    PAVPCmdHdr    Header;
    PAVPSessId    SigmaSessionId;
    SigmaPubKey   Ga;
} GetHwEccPubKeyOutBuff;

/*
**    Command: CMD_EXCHG_HW_APP_CERT
**
**    InBuffer (PAVP 1.5):
*/
typedef struct _HwAppExchgCertsInBuff 
{
    PAVPCmdHdr    Header;
    PAVPSessId    SigmaSessionId;
    SigmaPubKey   Gb;
    Cert3p        Certificate3p;
    HMAC          Certificate3pHmac;
    EcDsaSig      EcDsaSigGaGb;
} HwAppExchgCertsInBuff;
/*
**    InBuffer (PAVP 2.0)
*/
typedef struct _HwAppExchgCertsInBuff20 {
   PAVPCmdHdr     Header;
   PAVPSessId     SigmaSessionId;
   SigmaPubKey    Gb;
   Cert3p20       Certificate3p;
   HMAC           Certificate3pHmac;
   EcDsaSig       EcDsaSigGaGb;
} HwAppExchgCertsInBuff20;
/*
**    OutBuffer:
*/
typedef struct _HwAppExchgCertsOutBuff 
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
    EpidCert    CertificatePch;
    HMAC        CertificatePchHmac;
    EpidSig     EpidSigGaGb;
} HwAppExchgCertsOutBuff;

/*
**    Command: CMD_CLOSE_SIGMA_SESSION
**
**    InBuffer:
*/
typedef struct _CloseSigmaSessionInBuff 
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} CloseSigmaSessionInBuff;
/*
**    OutBuffer:
*/
typedef struct _CloseSigmaSessionOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} CloseSigmaSessionOutBuff;

/*
**    Command: CMD_GET_STREAM_KEY
**
**    InBuffer:
*/
typedef struct _GetStreamKeyInBuff
{
    PAVPCmdHdr      Header;
    PAVPSessId      SigmaSessionId;
    PAVPStreamId    StreamId;
    PAVPPathId      MediaPathId;
} GetStreamKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _GetStreamKeyOutBuff
{
    PAVPCmdHdr   Header;
    PAVPSessId   SigmaSessionId;
    StreamKey    WrappedStreamKey;
    HMAC         WrappedStreamKeyHmac;
} GetStreamKeyOutBuff;

typedef struct _GetStreamKeyExInBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} GetStreamKeyExInBuff;

typedef struct _GetStreamKeyExOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
    StreamKey   WrappedStreamDKey;
    StreamKey   WrappedStreamEKey;
    HMAC        WrappedStreamKeyHmac;
} GetStreamKeyExOutBuff;

/*
**    Command: CMD_INV_STREAM_KEY
**
**    InBuffer:
*/
typedef struct _InvStreamKeyInBuff
{
    PAVPCmdHdr      Header;
    PAVPSessId      SigmaSessionId;
    PAVPStreamId    StreamId;
    PAVPPathId      MediaPathId;
} InvStreamKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _InvStreamKeyOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} InvStreamKeyOutBuff;

typedef struct _SetSessionKeyInBuff
{
    PAVPCmdHdr      Header;
    PAVPSessId      SigmaSessionId;
    SigmaSessionKey SessionKey;
}SetSessionKeyInBuff;

typedef struct _SetSessionKeyOutBuff
{
    PAVPCmdHdr      Header;
}SetSessionKeyOutBuff;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
