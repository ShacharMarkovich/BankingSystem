/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2009-2010 Intel Corporation 
********************************************************************************
**                                                                            
**    INTEL CONFIDENTIAL                                                      
**    This file, software, or program is supplied under the terms of a        
**    license agreement and/or nondisclosure agreement with Intel Corporation 
**    and may not be copied or disclosed except in accordance with the        
**    terms of that agreement.  This file, software, or program contains      
**    copyrighted material and/or trade secret information of Intel           
**    Corporation, and must be treated as such.  Intel reserves all rights    
**    in this material, except as the license agreement or nondisclosure      
**    agreement specifically indicate.                                        
**                                                                            
**    All rights reserved.  No part of this program or publication            
**    may be reproduced, transmitted, transcribed, stored in a                
**    retrieval system, or translated into any language or computer           
**    language, in any form or by any means, electronic, mechanical,          
**    magnetic, optical, chemical, manual, or otherwise, without              
**    the prior written permission of Intel Corporation.                      
**                                                                            
**    Intel makes no warranty of any kind regarding this code.  This code     
**    is provided on an "As Is" basis and Intel will not provide any support, 
**    assistance, installation, training or other services.  Intel does not   
**    provide any updates, enhancements or extensions.  Intel specifically    
**    disclaims any warranty of merchantability, noninfringement, fitness     
**    for any particular purpose, or any other warranty.                      
**                                                                            
**    Intel disclaims all liability, including liability for infringement     
**    of any proprietary rights, relating to use of the code.  No license,    
**    express or implied, by estoppel or otherwise, to any intellectual       
**    property rights is granted herein.                                      
**/
/**                                                                            
********************************************************************************
**
**    @file typedefs.h
**
**    @brief  Contains common type declarations used throughout the code
**
**    @author Ranjit Narjala
**
********************************************************************************
*/   

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

// #include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef uint8_t			UINT8;
typedef uint16_t		UINT16;
typedef uint32_t		UINT32;
typedef int8_t			INT8;
typedef int16_t			INT16;
typedef int32_t			INT32;
*/

// NOTE: uintxx_t typedef'd here since it is being used in jhi.h
typedef unsigned char	UINT8, uint8_t;
typedef unsigned short	UINT16, unint16_t;
typedef unsigned int	UINT32, uint32_t;

// Already defined in Windows SDK\basetsd.h
#ifndef _WINRT_DLL
typedef char			int8_t;
#endif // !_WINRT_DLL
typedef short			INT16, int16_t;
typedef int				INT32, int32_t;

#ifndef IN
#define IN		// Defines an input parameter
#endif

#ifndef OUT
#define OUT		// Defines an output parameter
#endif

#ifndef INOUT
#define INOUT	// Defines an input/output parameter
#endif

#ifndef TRUE
#define TRUE  1     // True value for a BOOLEAN
#endif
#ifndef FALSE
#define FALSE 0     // False value for a BOOLEAN
#endif 
#define true  1     // True value for a BOOLEAN
#define false 0     // False value for a BOOLEAN
#define True  1     // True value for a BOOLEAN
#define False 0     // False value for a BOOLEAN

#ifdef __cplusplus
} // extern "C"
#endif

#ifndef NULL
#ifdef  __cplusplus
// Define NULL pointer value under C++
#define NULL    0
#else
// Define NULL pointer value non-C++
#define NULL    ((void *)0)
#endif
#endif

#endif // _TYPEDEFS_H