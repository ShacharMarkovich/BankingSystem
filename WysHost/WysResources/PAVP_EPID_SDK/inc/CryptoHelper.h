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
/* 
 *  file        :   CryptoHelper.h
 *  description :   CryptoHelper declarations
 *
 *
 *  notes       :   
 */
#pragma once

#include <windows.h>
#include <wincrypt.h>


#define AES_BLOCKLEN    (16)
#define AES_KEYSIZE     (32)

#define AES_KEYSIZE_128 (16)
#define AES_KEYSIZE_192 (24)
#define AES_KEYSIZE_256 (32)

#define RSA_PADDING 11

#define CALG_AES_128			(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_AES_128)
#define PROV_RSA_AES		24
#define ALG_SID_AES_128		14

/* 
 *  class       :   CryptoHelper
 *  description :   handles encryption/decryption/signing of messages
 *  inherits    :   
 *  notes       :   uses public CryptoAPI; compatible with BSafe API
 */
class CryptoHelper
{
public:
	// In we need to use RSA-OAEP encryption so we must distinguish between OPM and COPP 
	enum CRYPTO_SEMANTICS
    {
        COPP_CRYPTO_SEMANTICS = 0,
        OPM_CRYPTO_SEMANTICS = 1,
    };	

    //  ctors
    CryptoHelper(const BYTE*    pbRSAPublicKey, 
                const DWORD     cbRSAKeyLen, 
                const DWORD     dwRSAExponent, 
                HRESULT*        phr);
	
	CryptoHelper(const BYTE*    pbRSAModulus, 
                const DWORD     cbRSAModulus, 
				const BYTE*		pbRSAExponent,
                const DWORD     cbRSAExponent, 
                HRESULT*        phr,
				CRYPTO_SEMANTICS csNewSemantics);

    //  dtor
    ~CryptoHelper();


    //
    //  methods
    //

    //  function    :   Encrypt
    //  description :   encrypts (RSA) input buffer into an output ciphertext
    //  parameters  :   plain text
    //                  plain text length
    //                  bytes to encrypt
    //                  pointer to buffer to receive encrypted data (allocated by the function)
    //                  pointer to number of bytes encrypted
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         Encrypt(const BYTE* pbPlainText, 
                            const DWORD cbPlainTextLen, 
                            const DWORD cbToEncrypt,
                            BYTE**      ppbCipherText, 
                            DWORD*      pcbCipherTextLen);

    //  function    :   Decrypt
    //  description :   decrypts (RSA) specified ciphertext
    //  parameters  :   ciphertext
    //                  ciphertext length
    //                  pointer to clear text (allocated by the function)
    //                  pointer to number of bytes decrypted
    //
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         Decrypt(const BYTE* pbCipherText, 
                            const DWORD cbCipherText,
                            BYTE**      ppbPlainText, 
                            DWORD*      pcbPlainText);

    //  function    :   Sign
    //  description :   signs (AES based OMAC) input buffer
    //  parameters  :   plain text
    //                  plain text length
    //                  count of bytes to sign
    //                  pointer to buffer receiving signature (allocated by the function)
    //                  pointer to number of bytes signed
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         Sign(const BYTE*    pbPlainText,
                        const DWORD     cbPlainText,
                        BYTE            rgbSignature[AES_BLOCKLEN]);

    //  function    :   ExportAESKey
    //  description :   exports AES key
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         ExportAESKey(BYTE**         ppbAESKey, 
                                DWORD*          pcbAESKeyLen);

    //  function    :   GenerateRandom
    //  description :       "
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         GenerateRandom(BYTE*        pbNumber, 
                                  const DWORD   cbLen);

protected:

    //  function    :   Initialize
    //  description :   sets RSA public key, generates AES key
    //  parameters  :   public RSA key 
    //                  public RSA key length
    //                  public RSA key exponent
    //                  AES block length
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         Initialize(const BYTE*     pbRSAModulus,
								const DWORD     cbRSAModulus,
								const BYTE*     pbRSAExponent,
								const DWORD		cbRSAExponent,
								const DWORD     cbAESKey);

    //  function    :   ResetRSAKey
    //  description :   resets RSA public key
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         ResetRSAKey();

    //  function    :   ResetAESKey
    //  description :   resets AES public key
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         ResetAESKey();

    //  function    :   Reset
    //  description :   
    //  parameters  :   
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         Reset();

    enum AESHELPER_PROCESS_OPTION
    {
        DECRYPT = 0,
        ENCRYPT = 1,
    };

    //  function    :   SetRSAPublicKey
    //  description :   sets RSA public key
    //  parameters  :   public RSA key 
    //                  public RSA key length
    //                  public RSA key exponent
    //  returns     :   hresult
    //  notes       :   
    //  
    HRESULT         SetRSAPublicKey( const BYTE*     pbRSAModulus,
										const DWORD     cbRSAModulus,
										const BYTE*     pbRSAExponent,
										const DWORD		cbRSAExponent);

    //
    //  function    :   CBCProcessData
    //  description :   encrypt/decrypt (AES) data 
    //  parameters  :   pointer to buffer containing the input data
    //                  pointer to data buffer length (output)
    //                  data buffer length (input)
    //  returns     :   hresult
    //  notes       :   
    //
    HRESULT         CBCProcessData(AESHELPER_PROCESS_OPTION     Op,
                                    BYTE*                       pbData,
                                    const DWORD                 cbBlockSize,
                                    const DWORD                 cbBufferLength);


    //
    //  function    :   _GenerateSignTag
    //  description :   helper function (OMAC signature)
    //  parameters  :   pointer to buffer containing the input data
    //                  data buffer length (input)
    //                  temporary buffers for calculating OMAC constants
    //  returns     :   hresult
    //  notes       :   
    //
    HRESULT         _GenerateSignTag(const BYTE*    pbData,
                                    const DWORD     cbData,
                                    const BYTE      rgbLU[AES_BLOCKLEN],
                                    const BYTE      rgbLU_1[AES_BLOCKLEN],
                                    BYTE            rgbTag[AES_BLOCKLEN]);

    //
    //  function    :   _GenerateSignInfo
    //  description :   calculates OMAC constants
    //  parameters  :   
    //  returns     :   hresult
    //  notes       :   
    //
    HRESULT         _GenerateSignInfo(BYTE          rgbLU[AES_BLOCKLEN],
                                    BYTE            rgbLU_1[AES_BLOCKLEN]);

private:
    CryptoHelper()                      {}
    CryptoHelper(const CryptoHelper&)   {}

	CRYPTO_SEMANTICS	m_Semantics;
	
	//for RSA-OAEP (needed in OPM key exchange)
	BCRYPT_ALG_HANDLE	m_hCALG;
	BCRYPT_KEY_HANDLE	m_hRSAOAEPKey;

	//for RSA-PKCSv1 (needed in legacy COPP key exchange)
	HCRYPTPROV			m_hCSP;
    HCRYPTKEY			m_hRSAKey;
    HCRYPTKEY			m_hAESKey;
	//BCRYPT_KEY_HANDLE	m_hAESKey_1;
    TCHAR FAR *			m_szKeyContainer;

};  //  CryptoHelper

//
//  function    :   XOR
//  description :   XORs lhs w/ rhs
//  parameters  :   
//  returns     :   
//  notes       :   
//
inline void 
XOR
(
    BYTE*       lpbLHS, 
    BYTE*       lpbRHS, 
    const DWORD cbSize = AES_BLOCKLEN 
)
{
    for (DWORD i = 0; i < cbSize; i++)
    {
        lpbLHS[i] ^= lpbRHS[i];
    }
}   //  XOR

//
//  function    :   LShift
//  description :   left shifts lpbRes into lpbOpd
//  parameters  :   
//  returns     :   
//  notes       :   
//
inline void 
LShift
(
    BYTE        lpbOpd[AES_BLOCKLEN], 
    BYTE        lpbRes[AES_BLOCKLEN] 
)
{
    for (DWORD i = 0; i < AES_BLOCKLEN; i++)
    {
        lpbRes[i] = lpbOpd[i] << 1;
        if (i < AES_BLOCKLEN - 1)
        {
            lpbRes[i] |= ((unsigned char)lpbOpd[i+1]) >> 7;
        }
    }
}   //  LShift

//
//  function    :   RShift
//  description :   right shifts lpbRes into lpbOpd
//  parameters  :   
//  returns     :   
//  notes       :   
//
inline void 
RShift
(  
    BYTE        lpbOpd[AES_BLOCKLEN], 
    BYTE        lpbRes[AES_BLOCKLEN]
)
{
    for (DWORD i = 0; i < AES_BLOCKLEN; i++)
    {
        lpbRes[i] = ((unsigned char)lpbOpd[i]) >> 1;
        if (i > 0)
        {
            lpbRes[i] |= (lpbOpd[i-1]) << 7;
        }
    }
}   //  RShift

//
//  function    :   SafeDeleteArray
//  description :   
//  parameters  :   
//  returns     :   
//  notes       :   
//
template <class T>
inline
void
SafeDeleteArray
(
     T* pT
)
{
    if (0 != pT)
    {
        delete [] pT;
        pT = 0;
    }
}

//
//  function    :   SafeSecureMemDeleteArray
//  description :   
//  parameters  :   
//  returns     :   
//  notes       :   
//
template <class T>
inline
void
SafeSecureMemDeleteArray
(
    T*          pT,
    const DWORD cbSize
)
{
    if (0 == cbSize)
        return;

    if (0 != pT)
    {
        SecureZeroMemory(pT, cbSize);
        delete pT;
        pT = 0;
    }
}

//
//  function    :   GetCryptErrMsg
//  description :   
//  parameters  :   
//  returns     :   
//  notes       :   
//
void    GetCryptErrMsg(CHAR*    szMsg, const DWORD cchMsgLen, const DWORD dwCryptErr);

//
//  function    :   LogDebugErrorMessage
//  description :   
//  parameters  :   
//  returns     :   error code
//  notes       :   
//
DWORD   LogDebugErrorMessage(CHAR* szMsg, HRESULT hr = S_OK);

//
//  function    :   PrintByteArray
//  description :   
//  parameters  :   
//  returns     :   
//  notes       :   
//
void    PrintByteArray(const BYTE* pbKey, const DWORD cbKeyLen);


