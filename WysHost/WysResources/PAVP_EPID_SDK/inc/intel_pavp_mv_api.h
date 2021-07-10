/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/


//  THIS FILE SHOULD NOT BE DISTRIBUTED TO INDEPENDENT SOFTWARE VENDORS.
//  THIS FILE IS FOR MEDIA VAULT PARTNERS ONLY.
//
//  This file is dependent on intel_pavp_epid_api.h being included first.








#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Media Vault 1.0
#define CMD_SET_STREAM_KEY              0x00040000
#define CMD_BIND_STREAM_KEY             0x00040001
#define CMD_LOAD_BOUND_STREAM_KEY       0x00040002
#define CMD_SET_TIME_AND_CLOSE_SIGMA    0x00040003

// Certificate Type Values for the Intel Signed IntelSignedCertificateType Field (2.0 certificate only)
/// PAVP
#define PAVP_EPID_PUBCERT3P_TYPE_PAVP        0x00000000
/// Media Vault
#define PAVP_EPID_PUBCERT3P_TYPE_MV1_0       0x00000001


#pragma pack(push)
#pragma pack(1)

/*
**    Command: CMD_SET_STREAM_KEY
**
**    InBuffer:
*/
typedef struct _SetStreamKeyInBuff
{
    PAVPCmdHdr      Header;
    PAVPSessId      SigmaSessionId;
    PAVPStreamId    StreamId;
    PAVPPathId      MediaPathId;
    StreamKey       WrappedStreamKey;
    HMAC            WrappedStreamKeyHmac;
} SetStreamKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _SetStreamKeyOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} SetStreamKeyOutBuff;

/*
**    Command: CMD_BIND_STREAM_KEY
*/
typedef struct _PAVPTitleKeyBlob
{	
    uint8       IV[16];
    StreamKey   VEK;
    StreamKey   AEK;
    uint8       ValidStartTime[8];
    uint8       ValidEndTime[8];
    uint8       ReservedData[32];	
    HMAC        ICV;
} PAVPTitleKeyBlob;
/*
**    InBuffer:
*/
typedef struct _BindStreamKeyInBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
    StreamKey   VEK;
    StreamKey   AEK;
    uint8       ValidStartTime[8];
    uint8       ValidEndTime[8];
    uint8       ReservedData[32];
    HMAC        TitleKeyHmac;
} BindStreamKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _BindStreamKeyOutBuff
{
    PAVPCmdHdr          Header;
    PAVPSessId          SigmaSessionId;
    PAVPTitleKeyBlob    TitleKeys;
} BindStreamKeyOutBuff;

/*
**    Command: CMD_LOAD_BOUND_STREAM_KEY
**
**    InBuffer:
*/
typedef struct _LoadBoundKeyInBuff
{
    PAVPCmdHdr          Header;
    PAVPSessId          SigmaSessionId;
    PAVPStreamId        StreamIdVideo;
    PAVPStreamId        StreamIdAudio;
    PAVPTitleKeyBlob    TitleKeys;
    BOOLEAN             VerifyOnly;
} LoadBoundKeyInBuff;
/*
**    OutBuffer:
*/
typedef struct _LoadBoundKeyOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
    StreamKey   WrappedAudioKey;
    HMAC        WrappedStreamKeyHmac;
} LoadBoundKeyOutBuff;

/*
**    Command: CMD_SET_TIME_AND_CLOSE_SIGMA
**
**    InBuffer:
*/
typedef struct _SetTimeAndCloseInBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
    uint8       Time[8];
    HMAC        TimeHmac;
} SetTimeAndCloseInBuff;
/*
**    OutBuffer:
*/
typedef struct _SetTimeAndCloseOutBuff
{
    PAVPCmdHdr  Header;
    PAVPSessId  SigmaSessionId;
} SetTimeAndCloseOutBuff;


#pragma pack(pop)

#ifdef __cplusplus
}
#endif
