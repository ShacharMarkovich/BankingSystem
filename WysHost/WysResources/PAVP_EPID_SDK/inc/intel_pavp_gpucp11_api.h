/*===================== begin_copyright_notice ==================================

INTEL CONFIDENTIAL
Copyright 2010-2011
Intel Corporation All Rights Reserved.

The source code contained or described herein and all documents related to the 
source code ("Material") are owned by Intel Corporation or its suppliers or 
licensors. Title to the Material remains with Intel Corporation or its suppliers 
and licensors. The Material contains trade secrets and proprietary and confidential 
information of Intel or its suppliers and licensors. The Material is protected by 
worldwide copyright and trade secret laws and treaty provisions. No part of the 
Material may be used, copied, reproduced, modified, published, uploaded, posted, 
transmitted, distributed, or disclosed in any way without Intel’s prior express 
written permission. 

No license under any patent, copyright, trade secret or other intellectual 
property right is granted to or conferred upon you by disclosure or delivery 
of the Materials, either expressly, by implication, inducement, estoppel 
or otherwise. Any license under such intellectual property rights must be 
express and approved by Intel in writing. 

File Name: intel_pavp_gpucp_api.h

Abstract:

Environment:

Notes:

======================= end_copyright_notice ==================================*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif


// DX11 GUIDs:

// {BFB8C6C3-E088-4547-86AA-C6D95C769317}
static const GUID D3D11_AUTHENTICATED_QUERY_PAVP_SESSION_STATUS = 
{ 0xbfb8c6c3, 0xe088, 0x4547, { 0x86, 0xaa, 0xc6, 0xd9, 0x5c, 0x76, 0x93, 0x17 } };

// {849D003F-9AF4-47b4-B550-4E3F5F271846}
static const GUID D3D11_AUTHENTICATED_QUERY_INTEL_WIDI_STATUS = 
{ 0x849d003f, 0x9af4, 0x47b4, { 0xb5, 0x50, 0x4e, 0x3f, 0x5f, 0x27, 0x18, 0x46 } };


#pragma pack(push)
#pragma pack(8)

// DX11 Structs:

typedef struct tagD3D11_AUTHENTICATED_QUERY_PAVP_STATUS_INPUT
{
    D3D11_AUTHENTICATED_QUERY_INPUT     Parameters;
    BOOL                                bSkipOmac;
} D3D11_AUTHENTICATED_QUERY_PAVP_STATUS_INPUT;

typedef struct tagD3D11_AUTHENTICATED_QUERY_PAVP_STATUS_OUTPUT
{
    D3D11_AUTHENTICATED_QUERY_OUTPUT    Output;
    BOOL                                bSessionAlive;
} D3D11_AUTHENTICATED_QUERY_PAVP_STATUS_OUTPUT;

typedef struct tagD3D11_AUTHENTICATED_QUERY_WIDI_STATUS_OUTPUT
{
    D3D11_AUTHENTICATED_QUERY_OUTPUT    Output;
    UINT                                WidiVersion;
    UINT                                HardwareID;
    BOOL                                bWidiDisplayActive;
    UINT                                NumActiveWirelessDisplays;
    UINT                                Reserved;
} D3D11_AUTHENTICATED_QUERY_WIDI_STATUS_OUTPUT;

#pragma pack(pop)


#ifdef __cplusplus
}
#endif
