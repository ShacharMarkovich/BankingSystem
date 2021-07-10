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

typedef enum
{
    // Available encryption types
    PAVP_ENCRYPTION_NONE        = 1,
    PAVP_ENCRYPTION_AES128_CTR  = 2,
    PAVP_ENCRYPTION_AES128_CBC  = 4,
    PAVP_ENCRYPTION_AES128_ECB  = 8,
    PAVP_ENCRYPTION_DECE        = 16
} PAVP_ENCRYPTION_TYPE;

typedef enum
{
    // Available counter types
    PAVP_COUNTER_TYPE_A = 1,    // 32-bit counter, 96 zeroes (CTG, ELK, ILK, SNB)
    PAVP_COUNTER_TYPE_B = 2,    // 64-bit counter, 32-bit Nonce, 32-bit zero (SNB)
    PAVP_COUNTER_TYPE_C = 4,    // Standard AES counter
    PAVP_COUNTER_DECE   = 8     // DECE 128-bit (IVB)
} PAVP_COUNTER_TYPE;

typedef enum
{
    // Available endianness types
    PAVP_LITTLE_ENDIAN  = 1,
    PAVP_BIG_ENDIAN     = 2,
} PAVP_ENDIANNESS_TYPE;

#ifdef __cplusplus
}
#endif
