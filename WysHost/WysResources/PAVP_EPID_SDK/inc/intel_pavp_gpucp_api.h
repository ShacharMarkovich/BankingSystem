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


static const GUID GUID_Zero =
{ 0x0000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

/*
**    GPU-CP CryptoSession Key Exchange GUIDs
*/
// Cantiga {B15AFE59-70BF-4063-A733-EAE1A2E78242}
static const GUID D3DKEYEXCHANGE_CANTIGA = 
{ 0xb15afe59, 0x70bf, 0x4063, { 0xa7, 0x33, 0xea, 0xe1, 0xa2, 0xe7, 0x82, 0x42 } };

// EagleLake {A75334C7-080C-4539-8E2D-E6A0D2B10FF0}
static const GUID D3DKEYEXCHANGE_EAGLELAKE  = 
{ 0xa75334c7, 0x80c, 0x4539, { 0x8e, 0x2d, 0xe6, 0xa0, 0xd2, 0xb1, 0xf, 0xf0 } };

// Intel Enhanced Privacy ID (DAA) {D13D3283-9154-43A2-903A-F7E0F92D0AB5} 
static const GUID D3DKEYEXCHANGE_EPID = 
{ 0xD13D3283, 0x9154, 0x43a2, { 0x90, 0x3A, 0xF7, 0xE0, 0xF9, 0x2D, 0x0A, 0xB5 } };

// Key exchange for heavy mode protection {45F55235-1D9F-4c9e-8C05-80A96F1FD9AD}
static const GUID D3DKEYEXCHANGE_HW_PROTECT = 
{ 0x45f55235, 0x1d9f, 0x4c9e, { 0x8c, 0x5, 0x80, 0xa9, 0x6f, 0x1f, 0xd9, 0xad } };

// Key exchange for Isolated Decode on BXT+ 
static const GUID D3DKEYEXCHANGE_ISOLATED_DECODE =
{ 0xa17cf9e7, 0xb56, 0x4491, { 0x9b, 0xae, 0x10, 0x08, 0x2e, 0x15, 0x1b, 0xc5 } };

// Proprietary GUID denoting AES-128 CTR with a 32-bit counter
// {E29FAF83-AABF-48C8-A3AB-85FEDD5E60C4}
static const GUID D3DCRYPTOTYPE_INTEL_AES128_CTR  =
{ 0xe29faf83, 0xaabf, 0x48c8, { 0xa3, 0xab, 0x85, 0xfe, 0xdd, 0x5e, 0x60, 0xC4 } };

// Proprietary GUID indicating content is encrypted in standard CBC format
// {DA3538FA-B5CB-4d58-86C3-BD6C00F758A3}
static const GUID D3DCRYPTOTYPE_INTEL_AES128_CBC = 
{ 0xda3538fa, 0xb5cb, 0x4d58, { 0x86, 0xc3, 0xbd, 0x6c, 0x0, 0xf7, 0x58, 0xa3 } };

// Proprietary GUID indicating content is encrypted in DECE format
static const GUID D3DCRYPTOTYPE_INTEL_DECE_AES128_CTR  =
{ 0x8DD4D67F, 0xABCE, 0x4D9B, { 0xB7, 0xC8, 0x2C, 0xCF, 0xB9, 0x41, 0x8C, 0x7B } };

static const GUID GUID_INTEL_TRANSCODE_SESSION = 
{ 0xefcf2910, 0xd0f0, 0x4e49, { 0x8d, 0x3c, 0x13, 0xdb, 0x6c, 0x68, 0x1b, 0x3b } };

static const GUID GUID_INTEL_PROPRIETARY_FUNCTION = 
{ 0xe8c7e79f, 0x10b7, 0x43d6, { 0xa1, 0x77, 0xdf, 0xa3, 0x9c, 0x6b, 0x25, 0x25 } };

static const GUID INTEL_QUERY_WIDI_STATUS = 
{ 0xe4588ff4, 0x0055, 0x4986, { 0x9d, 0x08, 0xef, 0xec, 0xbf, 0x86, 0x42, 0x12 } };

static const GUID D3DAUTHENTICATEDQUERY_PAVP_SESSION_STATUS	= 
{ 0xfee9dd76, 0x1673, 0x4598, {0x8e, 0x58, 0x81, 0x0b, 0x6f, 0x6a, 0xe5, 0x73 } };

static const GUID INTEL_CONFIG_WIDI_SETFRAMERATE = 
{ 0xd9e6c700, 0xdc2d, 0x4812, { 0xbf, 0xde, 0x0f, 0x18, 0x32, 0xf4, 0x78, 0x7f } };

// Key exchange for WYSIWYS heavy mode protection {D1F4F0D1-3C16-4DA7-86FA-C1A69139ADD6}
static const GUID D3DKEYEXCHANGE_WYSIWYS =
{ 0xd1f4f0d1, 0x3c16, 0x4da7, { 0x86, 0xfa, 0xc1, 0xa6, 0x91, 0x39, 0xad, 0xd6 } };

#pragma pack(push)
#pragma pack(8)


typedef struct _tagGPUCP_CRYPTOSESSION_PAVP_KEYEXCHANGE
{
    GUID    PAVPKeyExchangeGUID;
    UINT    DataSize;
    VOID*   pKeyExchangeParams;
} GPUCP_CRYPTOSESSION_PAVP_KEYEXCHANGE;

typedef struct _tagGPUCP_PROPRIETARY_FUNCTION_PARAMS
{
    UINT    uiPavpFunctionId;
    VOID*   pInput;
    UINT    uiInputSize;
    VOID*   pOutput;
    UINT    uiOutputSize;
} GPUCP_PROPRIETARY_FUNCTION_PARAMS;

typedef struct tagPAVP_EPID_EXCHANGE_PARAMS
{
    BYTE    *pInput;
    ULONG   ulInputSize;
    BYTE    *pOutput;
    ULONG   ulOutputSize;
} PAVP_EPID_EXCHANGE_PARAMS;

typedef struct tagD3DAUTHENTICATEDCHANNELQUERY_PAVP_STATUS_INPUT
{
    D3DAUTHENTICATEDCHANNEL_QUERY_INPUT Parameters;
    BOOL    bSkipOmac;
} D3DAUTHENTICATEDCHANNELQUERY_PAVP_STATUS_INPUT;

typedef struct tagD3DAUTHENTICATEDCHANNELQUERY_PAVP_STATUS_OUTPUT
{
    D3DAUTHENTICATEDCHANNEL_QUERY_OUTPUT Output;
    BOOL    bSessionAlive;
} D3DAUTHENTICATEDCHANNELQUERY_PAVP_STATUS_OUTPUT;

typedef struct tagD3DAUTHENTICATEDQUERY_WIDI_STATUS_OUTPUT
{
    D3DAUTHENTICATEDCHANNEL_QUERY_OUTPUT Output;
    UINT    WidiVersion;
    UINT    HardwareID;
    BOOL    bWidiDisplayActive;
    UINT    NumActiveWirelessDisplays;
    UINT    Reserved;
} D3DAUTHENTICATEDQUERY_WIDI_STATUS_OUTPUT;

typedef struct _tagD3DAUTHENTICATEDCHANNEL_CONFIGURE_WIDI_FRAMERATE_INPUT
{
    D3DAUTHENTICATEDCHANNEL_CONFIGURE_INPUT Parameters;
    FLOAT   Framerate;
    UINT    Reserved;
} D3DAUTHENTICATEDCHANNEL_CONFIGURE_WIDI_FRAMERATE_INPUT;

#pragma pack(pop)


#ifdef __cplusplus
}
#endif
