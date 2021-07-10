/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#pragma once

/* 
**	GUID for Intel Generic Auxiliary Device Interface
**	See Auxiliary API/DDI design for more information
**	{A74CCAE2-F466-45ae-86F5-AB8BE8AF8483}
*/
const GUID DXVA2_Intel_Auxiliary_Device =
{ 0xa74ccae2, 0xf466, 0x45ae, { 0x86, 0xf5, 0xab, 0x8b, 0xe8, 0xaf, 0x84, 0x83 } };

/*
** Intel Auxiliary Function IDs
*/
typedef enum tagAUXDEV_FUNCTION_ID
{
    AUXDEV_GET_ACCEL_GUID_COUNT         = 1,
    AUXDEV_GET_ACCEL_GUIDS	            = 2,
    AUXDEV_GET_ACCEL_RT_FORMAT_COUNT	= 3,
    AUXDEV_GET_ACCEL_RT_FORMATS         = 4,
    AUXDEV_GET_ACCEL_FORMAT_COUNT       = 5,
    AUXDEV_GET_ACCEL_FORMATS            = 6,
    AUXDEV_QUERY_ACCEL_CAPS             = 7,
    AUXDEV_CREATE_ACCEL_SERVICE         = 8,
    AUXDEV_DESTROY_ACCEL_SERVICE        = 9
} AUXDEV_FUNCTION_ID;