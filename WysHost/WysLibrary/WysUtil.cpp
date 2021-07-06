/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2010-2011 Intel Corporation 
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
/***************************************************************************************//**
**
**    @file WysUtil.cpp
**
**    @brief  Implements for the WYS Util class and helper functions
**
**    @author Pradeep Sebestian
**
********************************************************************************************/ 
#include <stdio.h>
#include <process.h>
#include <sys/stat.h>
#include "WYSUtil.h"

bool WysUtil::m_bUseBmp2Mpeg2 = false;//true;

unsigned __int32 WysUtil::bytesToInt(INT_BYTES bytes)
{
   unsigned int value = 0;

   value |= (bytes.MSWMSB << 24);
   value |= (bytes.MSWLSB << 16);
   value |= (bytes.LSWMSB << 8);
   value |= (bytes.LSWLSB);

   return value;
}

unsigned __int16 WysUtil::bytesToShort(SHORT_BYTES bytes)
{
   unsigned short value = 0;

   value |= (bytes.MSB << 8);
   value |= (bytes.LSB);

   return value;
}

void WysUtil::shortToBytes(unsigned short value, SHORT_BYTES *pBytes)
{
   pBytes->MSB = (value & 0xFF00) >> 8;
   pBytes->LSB = (value & 0x00FF);
}

void WysUtil::intToBytes(unsigned int value, INT_BYTES *pBytes)
{
   pBytes->MSWMSB = (value & 0xFF000000) >> 24;
   pBytes->MSWLSB = (value & 0x00FF0000) >> 16;
   pBytes->LSWMSB = (value & 0x0000FF00) >> 8;
   pBytes->LSWLSB = (value & 0x000000FF);
}

