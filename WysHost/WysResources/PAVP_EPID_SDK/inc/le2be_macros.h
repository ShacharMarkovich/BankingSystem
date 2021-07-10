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

/*
 *		Endianess translation macros used for SafeId serialization/deserialization
 */
// LE<->BE translation of DWORD
#define SwapEndian_DW(dw)	( (((dw) & 0x000000ff) << 24) |  (((dw) & 0x0000ff00) << 8) | (((dw) & 0x00ff0000) >> 8) | (((dw) & 0xff000000) >> 24) )
// LE<->BE translation of 8 byte big number
#define SwapEndian_8B(ptr)													\
{																			\
	unsigned int Temp = 0;													\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[0]);						\
	((unsigned int*)(ptr))[0] = SwapEndian_DW(((unsigned int*)(ptr))[1]);	\
	((unsigned int*)(ptr))[1] = Temp;										\
}																			\
// LE<->BE translation of 10 byte (80 bit) big number
#define SwapEndian_10B(ptr)																									\
{																															\
	unsigned int Temp1 = 0;																									\
	unsigned int Temp2 = 0;																									\
	Temp1 = SwapEndian_DW( (unsigned int)( (((unsigned char*)(ptr))[8] << 16 ) | ( ((unsigned char*)(ptr))[9] << 24) ) );	\
	Temp2 = SwapEndian_DW(((unsigned int*)(ptr))[1]);																		\
	*((unsigned int*)((unsigned char*)(ptr) + 6)) = SwapEndian_DW(((unsigned int*)(ptr))[0]);								\
	*((unsigned int*)((unsigned char*)(ptr) + 2)) = Temp2;																	\
	((unsigned int*)(ptr))[0] = (Temp1 & 0x0000ffff) | (((unsigned int*)(ptr))[0] & 0xffff0000);							\
}
// LE<->BE translation of 16 byte (128-bit) big number
#define SwapEndian_16B(ptr)													\
{																			\
	unsigned int Temp = 0;													\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[0]);						\
	((unsigned int*)(ptr))[0] = SwapEndian_DW(((unsigned int*)(ptr))[3]);	\
	((unsigned int*)(ptr))[3] = Temp;										\
    Temp = SwapEndian_DW(((unsigned int*)(ptr))[1]);						\
	((unsigned int*)(ptr))[1] = SwapEndian_DW(((unsigned int*)(ptr))[2]);	\
	((unsigned int*)(ptr))[2] = Temp;										\
}				
// LE<->BE translation of 32 byte big number
#define SwapEndian_32B(ptr)													\
{																			\
	unsigned int Temp = 0;													\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[0]);						\
	((unsigned int*)(ptr))[0] = SwapEndian_DW(((unsigned int*)(ptr))[7]);	\
	((unsigned int*)(ptr))[7] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[1]);						\
	((unsigned int*)(ptr))[1] = SwapEndian_DW(((unsigned int*)(ptr))[6]);	\
	((unsigned int*)(ptr))[6] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[2]);						\
	((unsigned int*)(ptr))[2] = SwapEndian_DW(((unsigned int*)(ptr))[5]);	\
	((unsigned int*)(ptr))[5] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[3]);						\
	((unsigned int*)(ptr))[3] = SwapEndian_DW(((unsigned int*)(ptr))[4]);	\
	((unsigned int*)(ptr))[4] = Temp;										\
}
// LE<->BE translation of 64 byte big number
#define SwapEndian_64B(ptr)													\
{																			\
	unsigned int Temp = 0;													\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[0]);						\
	((unsigned int*)(ptr))[0] = SwapEndian_DW(((unsigned int*)(ptr))[15]);	\
	((unsigned int*)(ptr))[15] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[1]);						\
	((unsigned int*)(ptr))[1] = SwapEndian_DW(((unsigned int*)(ptr))[14]);	\
	((unsigned int*)(ptr))[14] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[2]);						\
	((unsigned int*)(ptr))[2] = SwapEndian_DW(((unsigned int*)(ptr))[13]);	\
	((unsigned int*)(ptr))[13] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[3]);						\
	((unsigned int*)(ptr))[3] = SwapEndian_DW(((unsigned int*)(ptr))[12]);	\
	((unsigned int*)(ptr))[12] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[4]);						\
	((unsigned int*)(ptr))[4] = SwapEndian_DW(((unsigned int*)(ptr))[11]);	\
	((unsigned int*)(ptr))[11] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[5]);						\
	((unsigned int*)(ptr))[5] = SwapEndian_DW(((unsigned int*)(ptr))[10]);	\
	((unsigned int*)(ptr))[10] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[6]);						\
	((unsigned int*)(ptr))[6] = SwapEndian_DW(((unsigned int*)(ptr))[9]);	\
	((unsigned int*)(ptr))[9] = Temp;										\
	Temp = SwapEndian_DW(((unsigned int*)(ptr))[7]);						\
	((unsigned int*)(ptr))[7] = SwapEndian_DW(((unsigned int*)(ptr))[8]);	\
	((unsigned int*)(ptr))[8] = Temp;										\
}
// LE<->BE translation of 75 byte (600 bit) big number
#define SwapEndian_75B(ptr)																																				\
{																																										\
	unsigned int Temp1 = 0;																																				\
	unsigned int Temp2  = 0;																																			\
	unsigned char i = 0;																																				\
	Temp1 = SwapEndian_DW( (unsigned int)( ( ((unsigned char*)(ptr))[72] << 8 ) | ( ((unsigned char*)(ptr))[73] << 16 ) | ( ((unsigned char*)(ptr))[74] << 24 ) ) );	\
	Temp2 = (((unsigned int*)(ptr))[17] & 0xff000000);																													\
	*((unsigned int*)((unsigned char*)(ptr) + 71)) = SwapEndian_DW(((unsigned int*)(ptr))[0]);																			\
	((unsigned int*)(ptr))[0] = Temp1;																																	\
	Temp1 = SwapEndian_DW(((unsigned int*)(ptr))[1]);																													\
	*((unsigned int*)((unsigned char*)(ptr) + 3 )) = SwapEndian_DW( ( (((unsigned int*)(ptr))[17] & 0x00ffffff) | Temp2 ) );											\
	for(i = 0; i < 8; i++) {																																			\
		Temp2 = (((unsigned int*)(ptr))[16 - i] & 0xff000000);																											\
		*((unsigned int*)((unsigned char*)(ptr) + 67 - 4*i)) = Temp1;																									\
		Temp1 = SwapEndian_DW(((unsigned int*)(ptr))[2 + i]);																											\
		*((unsigned int*)((unsigned char*)(ptr) + 7 + 4*i)) = SwapEndian_DW( ( (((unsigned int*)(ptr))[16 - i] & 0x00ffffff) | Temp2 ) );								\
	}																																									\
}


/*
 *	Some structures useful during the convertion of SafeId data
 */

typedef struct _G1Point {
	unsigned char x[32];
	unsigned char y[32];
} G1Point;

typedef struct _G2Point {
	unsigned char x0[32];
	unsigned char x1[32];
	unsigned char x2[32];
	unsigned char y0[32];
	unsigned char y1[32];
	unsigned char y2[32];
} G2Point;

typedef G1Point G3Point;

typedef struct _GTPoint {
	unsigned char x0[32];
	unsigned char x1[32];
	unsigned char x2[32];
	unsigned char x3[32];
	unsigned char x4[32];
	unsigned char x5[32];
} GTPoint;

#ifdef __cplusplus
}
#endif
