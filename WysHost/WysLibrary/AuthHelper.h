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
/*
 *  file        :   AuthenticationHelper
 *  description :   declaration of hte authentication helper
 *  history     :   
 */

#pragma once
//#include "Wys.h"
#include "CryptoHelper.h"
#include "ippdefs.h"

#define XRML_TAG_MODULUS                ("Modulus")
#define XRML_TAG_EXPONENT               ("Exponent")
#define XRML_TAG_RSAKEYVALUE            ("RSAKeyValue")

#define RSA_MODULUS_BYTE_LENGTH  512


#include <windows.h>
#include "CertHelper.h"
//#include "ippcp.h"


// Cantiga B2 and later
static const DWORD   s_dwFixedKeyCtg[4]             = {0x1d593900, 0xa3310324, 0x20e774c4, 0x6b32bd3c};
static const DWORD   s_dwEncryptedFixedKeyCtg[4]    = {0xb0b03c49, 0x1653d48f, 0xca39be75, 0x5e6ea5dc};

// Eaglelake A2 and later
static const DWORD   s_dwFixedKeyEgl[4]             = {0x43489E03, 0x2FE2969E, 0xAF158415, 0xE472EB55};
static const DWORD   s_dwEncryptedFixedKeyEgl[4]    = {0xAF2896BF, 0xCA3E16E8, 0xFA8AC85C, 0xD2ECC9FF};

// ILK B0 (ES1) and later
static const DWORD   s_dwFixedKeyIlk[4]             = {0x87a6e010, 0x06e9cb0e, 0xbedca4bc, 0x826ca3d6};
static const DWORD   s_dwEncryptedFixedKeyIlk[4]    = {0xbbeef0bb, 0x0f656c66, 0x1453fd78, 0x49cb02a0};

// Sandybridge A step
static const DWORD   s_dwFixedKeySnbAstep[4]             = {0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1, 0xa1a1a1a1};
static const DWORD   s_dwEncryptedFixedKeySnbAstep[4]    = {0xc29566fd, 0x6526d11f, 0xa8b6f2c9, 0x49536861};

// Sandybridge B0 and later (CPT keys)
static const DWORD   s_dwFixedKeySnb[4]             = {0x87a6e010, 0x06e9cb0e, 0xbedca4bc, 0x826ca3d6};
static const DWORD   s_dwEncryptedFixedKeySnb[4]    = {0xe505579d, 0x0d7faf78, 0x54ce9a6b, 0x64839bd5};

// Sandybridge B0 and later (Ibx keys)
static const DWORD   s_dwFixedKeySnbIbx[4]             = {0x87a6e010, 0x06e9cb0e, 0xbedca4bc, 0x826ca3d6};
static const DWORD   s_dwEncryptedFixedKeySnbIbx[4]    = {0xbbeef0bb, 0x0f656c66, 0x1453fd78, 0x49cb02a0};

// Test mode
static const DWORD   s_dwTestModeFixedKey[4]        = {0xa829902e, 0x103de435, 0xd7e19abb, 0x13f6d017};
static const DWORD   s_dwEncryptedTestModeFixedKey[4]   = {0x72e443d6, 0x0bfd6b14, 0x515382ab, 0x1a80617f};

static const DWORD   s_dwFixedKeyDummy[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const DWORD   s_dwEncryptedFixedKeyDummy[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};

static const GUID ZERO_GUID = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };


/*
 *  class       :   COPPCertHelper
 *  description :   certificate parsing helper function
 *  inherits    :   
 *  notes       :   
 */
class COPPAuthHelper
{
public:

    //  construction
    COPPAuthHelper();
    COPPAuthHelper(CryptoHelper::CRYPTO_SEMANTICS csNewSemantics);
    ~COPPAuthHelper();

    HRESULT     Initialize(const BYTE*  pbGHCert, 
                            const DWORD cbGHCert);

    //  functionality
    HRESULT     PrepareSignature(BYTE*      pbSignature, 
                                const DWORD cbSignature, 
                                const GUID* pgGHRandom, 
                                const DWORD dwStatusSeqIdx, 
                                const DWORD dwCmdSeqIdx); 

    HRESULT        PrepareAuthSignature(BYTE*       pbSignature,
                                const DWORD cbSigSize);

    HRESULT        PreparePAVPSignature(BYTE* pbSignature, 
                                const DWORD cbSigSize,
                                GUID  KeyExchangeType);

    HRESULT     Sign(const BYTE*    pbData, 
                    const DWORD     cbData, 
                    BYTE*           pbSignature, 
                    const DWORD     cbSignature);

    HRESULT     VerifySignature(const BYTE* pbData, 
                                const DWORD cbData, 
                                const BYTE* pbSignature, 
                                const DWORD cbSignature);

    //  tools
    HRESULT     GenerateRandomNumber
    (
        BYTE*       pbNumber, 
        const DWORD cbSize
    )    
    {   
        return m_pCryptoHelper->GenerateRandom(pbNumber, cbSize);
    }

private:

    CryptoHelper*                    m_pCryptoHelper;
    COPPCertHelper                    m_CertHelper;
    CryptoHelper::CRYPTO_SEMANTICS    m_Semantics;

};  //  class COPPAuthHelper

void    PrintByteArray(const BYTE*  pbArray, 
                       const DWORD  cbArray);

void    AesEncryptEcb(Ipp8u* key, 
                      Ipp8u* ptext, 
                      Ipp8u* pOutput, 
                      DWORD dwSize);

void    RandomNumber(DWORD* pRandNum, 
                     DWORD dwSize);
