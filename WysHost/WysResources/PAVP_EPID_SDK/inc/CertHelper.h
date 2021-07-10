/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2011 Intel Corporation 
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
#pragma once

#include "CryptoHelper.h"

#define XRML_TAG_MODULUS                ("Modulus")
#define XRML_TAG_EXPONENT               ("Exponent")
#define XRML_TAG_RSAKEYVALUE            ("RSAKeyValue")

#define RSA_MODULUS_BYTE_LENGTH  512



/*
 *  class       :   COPPCertHelper
 *  description :   certificate parsing helper function
 *  inherits    :   
 *  notes       :   
 */
class COPPCertHelper
{
public:
    //  ctors
    COPPCertHelper();
    COPPCertHelper(CryptoHelper::CRYPTO_SEMANTICS csNewSemantics);
    ~COPPCertHelper();

    //  methods
    HRESULT     SetCertificate(const BYTE*  pbCert,
                                const DWORD cbCertLen);

    HRESULT     ExtractPublicKey(BYTE**     ppbMod,
                                DWORD*      pcbModLen,
                                BYTE**        ppbExp,
                                DWORD*      pcbExpLen);

protected:

    HRESULT     FindElement(CHAR**          ppstrElement,
                            DWORD*          pcbElementLen,
                            const CHAR*     pstrTag);

    HRESULT     ResetCertificate();

private:
    BYTE*                            m_pbCertificate;
    DWORD                            m_cbCertificateLength;
    CryptoHelper::CRYPTO_SEMANTICS    m_Semantics;
};  //  class COPPcertHelper

//  other helper functions
HRESULT DecodeBase64RSAKey(const CHAR*  szBase64Modulus, 
                           const DWORD  cchBase64ModulusLen, 
                           const DWORD  cbModulusLen,
                           const CHAR*  szBase64Exponent, 
                           const DWORD  cchBase64ExponentLen, 
                           BYTE**       ppbBinaryRSAKey, 
                           DWORD*       pcbBinaryRSAKeyLen, 
                           DWORD*       pcbRSAExponent);

HRESULT DecodeBase64String(const CHAR*  pszBase64String,
                           const DWORD  cchBase64StringLen,
                           BYTE**       ppbBinaryNumber,
                           DWORD*       pcbBinaryNumberLen);
