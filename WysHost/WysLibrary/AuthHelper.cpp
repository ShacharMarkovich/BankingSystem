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
*/
#pragma warning( disable: 4312) //annoying warning from wxutil.h

#ifndef WYSAMULET
#include "AuthHelper.h"

/*
 *  class		:    COPPAuthHelper
 *  description	:    Helper class for authentification mechanism
 */

COPPAuthHelper::COPPAuthHelper
(): m_pCryptoHelper(NULL),
	m_Semantics(CryptoHelper::OPM_CRYPTO_SEMANTICS)
{ 
}

COPPAuthHelper::~COPPAuthHelper
()
{
    //  the respective destructors reset any keys
    if (NULL != m_pCryptoHelper)
    {
        delete m_pCryptoHelper;
        m_pCryptoHelper = NULL;
    }
}

/*
 *  function    :
 *  description :
 *  arguments   :
 *  returns     :
 *  notes       :
 */
//#pragma managed
HRESULT COPPAuthHelper::Initialize
(
    const BYTE* pbCert,
    const DWORD dwCertLen
)
{
	HRESULT     hr = S_OK;
    UINT8*       pbModulus = 0;
    DWORD       cbModulus = 0;
    UINT8*		pbExponent = 0;
	DWORD       cbExponent = 0;

    //  validation
    if ((0 == pbCert) || (0 == dwCertLen))
        return E_INVALIDARG;

    do
    {
        //  set this certificate
        if (FAILED(hr = m_CertHelper.SetCertificate(pbCert, dwCertLen)))
            break;

        //  extract the public key from the certificate
        if (FAILED(hr = m_CertHelper.ExtractPublicKey(&pbModulus, &cbModulus, &pbExponent, &cbExponent)))
            break;

		//Check to see if test driver is running 

		m_Semantics = CryptoHelper::OPM_CRYPTO_SEMANTICS;
		//  initialize the crypto helper
        //  assert we're creating a new object
		m_pCryptoHelper = new CryptoHelper(pbModulus, cbModulus, pbExponent, cbExponent, &hr, m_Semantics);
        if (0 == m_pCryptoHelper)
        {
            hr = E_OUTOFMEMORY;
        }
    }   while (FALSE);

    SafeSecureMemDeleteArray(pbModulus, cbModulus);
	SafeSecureMemDeleteArray(pbExponent, cbExponent);
	
    return hr;
}   //  COPPAuthHelper::Initialize

/*
 *  function    :   COPPAuthHelper::Sign
 *  description :   signs the specified data buffer (OMAC-1)
 *  arguments   :   data buf, data size, out sig buf, sig size
 *  returns     :   hresult
 *  notes       :
 */
HRESULT COPPAuthHelper::Sign
(
    const BYTE* pbData,
    const DWORD cbDataSize,
    BYTE*       pbSignature,
    const DWORD cbSigSize
)
{
    HRESULT     hr = S_OK;
    BYTE        rgbSignature[AES_BLOCKLEN] = {0};

    //  validation
    if ((0 == pbData) || (0 == cbDataSize) ||
        (0 == pbSignature) || (0 == cbSigSize))
        return E_INVALIDARG;

    hr = m_pCryptoHelper->Sign((BYTE*)pbData, cbDataSize, rgbSignature);
    if (SUCCEEDED(hr))
    {
		memcpy_s(pbSignature, sizeof(rgbSignature), rgbSignature, min(cbSigSize, AES_BLOCKLEN));
    }

    return hr;
}   //  COPPAuthHelper::Sign

/*
 *  function    :   COPPAuthHelper::VerifySignature
 *  description :
 *  arguments   :
 *  returns     :
 *  notes       :
 */
HRESULT COPPAuthHelper::VerifySignature
(
    const BYTE* pbData,
    const DWORD cbDataSize,
    const BYTE* pbSignature,
    const DWORD cbSigSize
)
{
    HRESULT     hr = S_OK;
    BYTE        rgbSignature[AES_BLOCKLEN] = {0};
	int			cmpresult;

    //  validation
    if ((0 == pbData) || (0 == cbDataSize) ||
        (0 == pbSignature) || (0 == cbSigSize))
        return E_INVALIDARG;

    if (cbSigSize != AES_BLOCKLEN)
        return E_INVALIDARG;

    do
    {
        //  calculate our own signature
        if (FAILED(hr = m_pCryptoHelper->Sign((BYTE*)pbData, cbDataSize, rgbSignature)))
            break;
		
		cmpresult = memcmp(pbSignature, rgbSignature, min(cbSigSize, AES_BLOCKLEN));

        if (cmpresult)	//pbSignature = rgbSignature
            hr = E_FAIL;


    }   while (FALSE);

    return hr;
}   //  COPPAuthHelper::VerifySignature

/*
 *  function    :   PrepareAuthSignature
 *  description :
 *  arguments   :
 *  returns     :
 *  notes       :
 */

HRESULT COPPAuthHelper::PrepareAuthSignature
(
    BYTE*       pbSignature,
    const DWORD cbSigSize
)
{
	HRESULT     hr = S_OK;
    DWORD       cbOffset = 0;
    BYTE*       pbEncData = 0;
    DWORD       cbEncData = 0;
    DWORD       cbKeyLen = 0;
    DWORD       cbToEncrypt = 0;
    BYTE*       pbAESKey = 0;

    //  validation
    if ((0 == pbSignature) || (0 == cbSigSize))
        return E_INVALIDARG;

    do
    {
        //  AES key
        if (FAILED(hr = m_pCryptoHelper->ExportAESKey(&pbAESKey, &cbKeyLen)))
            break;
		memcpy_s(pbSignature + cbOffset, cbKeyLen, pbAESKey, cbKeyLen); 
		
        SafeSecureMemDeleteArray(pbAESKey, cbKeyLen);
        cbOffset += cbKeyLen;

        //  zero the rest
        SecureZeroMemory(pbSignature + cbOffset, cbSigSize - cbOffset);

        //  encrypt
		cbToEncrypt = cbOffset;
        if (FAILED(hr = m_pCryptoHelper->Encrypt(pbSignature, cbToEncrypt, cbSigSize,
                                                &pbEncData, &cbEncData)))
            break;


        //  transfer result
        SecureZeroMemory(pbSignature, cbSigSize);
		memcpy_s(pbSignature, cbEncData, pbEncData, cbEncData);

    }   while (FALSE);

    SafeSecureMemDeleteArray(pbEncData, cbKeyLen);
    return hr;
}   //  COPPAuthHelper::PrepareAuthSignature

#endif // WYSAMULET