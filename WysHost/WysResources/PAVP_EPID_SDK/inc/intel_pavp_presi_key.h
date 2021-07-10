/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/


//  THIS FILE SHOULD NOT BE DISTRIBUTED TO INDEPENDENT SOFTWARE VENDORS.


#ifndef _INTEL_PAVP_PRESI_KEY_H
#define _INTEL_PAVP_PRESI_KEY_H

#include "intel_pavp_epid_api.h"

static const SigmaSessionKey S1 = {0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf};

//For fake session key
#define CMD_SET_SESSION_KEY       0x80000000

#endif