/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#ifndef _CRYPT_DATA_GEN_H
#define _CRYPT_DATA_GEN_H

#include "cdg_status_codes.h"

#ifdef CRYPTODATAGEN_EXPORTS
#define CRYPTODATAGEN_API //__declspec(dllexport)
#else
#define CRYPTODATAGEN_API //__declspec(dllimport)
#endif

static int current_gid;

/*	Function:
 *		InitailizeCgd
 *	Description:
 *		Initialize the library - set the context and create dummy Intel's key pair
 *	Input arguments:
 *		None
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus InitializeCdg();



/*	Function:
 *		VerifyCert3pSig3p
 *	Description:
 *		Verify 3rd party's signature of the 3rd party certificate
 *	Input arguments:
 *		IN	*PubCert3p			- pointer to the buffer containing the certificate (type: Cert3p)
 *		IN	PubCert3pLen		- certificate buffer length = sizeof(Cert3p)
 *		OUT	*VerifRes			- pointer to the verification result
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus VerifyCert3pSig3p(unsigned char*	PubCert3p,
						    int				PubCert3pLen,
						    CdgResult*		VerifRes);

/*	Function:
 *		MessageSign3p
 *	Description:
 *		Sign the message with 3rd party private key
 *	Input arguments:
 *		IN	*PrivKey3p			- pointer to the buffer containing 3rd party private key
 *		IN	PrivKey3pLen		- private key buffer length
 *		IN	*Message			- pointer to the buffer containing the message to be signed
 *		IN	MessageLen			- message buffer length
 *		OUT	*Signature			- pointer to the buffer for generated signature
 *		IN	SignatureLen		- signature buffer length
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus MessageSign3p(unsigned char*	PrivKey3p,
						int				PrivKey3pLen,
						unsigned char*	Message,
						int				MessageLen,
						unsigned char*	Signature,
						int				SignatureLen);

/*	Function:
 *		VerifyCertPchSigIntel
 *	Description:
 *		Verify Intel's signature of the PCH EPID certificate
 *	Input arguments:
 *		IN	*PubCertPch			- pointer to the buffer containing PCH certificate
 *		IN	PubCertPchLen		- certificate buffer length
 *		OUT	*VerifRes			- pointer to the verification result
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus VerifyCertPchSigIntel(unsigned char*	PubCertPch,
								int				PubCertPchLen,
								CdgResult*		VerifRes);


/*	Function:
 *		MessageVerifyPch
 *	Description:
 *		Verify the PCH EPID signature of the message
 *	Input arguments:
 *		IN	*PubKeyPch			- pointer to the buffer containing PCH EPID public key (this is used to deserialize the private key)
 *		IN	PubKeyPchLen		- public key buffer length
 *		IN	*Message			- pointer to the buffer containing the message to be signed
 *		IN	MessageLen			- message buffer length
 *		IN	*Bsn				- pointer to the buffer containing the base name for the signature
 *		IN	BsnLen				- length of the base name buffer
 *		IN	*Signature			- pointer to the buffer for generated signature
 *		IN	SignatureLen		- signature buffer length
 *		OUT	*VerifRes			- pointer to the verification result
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus MessageVerifyPch(unsigned char*	PubKeyPch,
						   int				PubKeyPchLen,
						   unsigned char*	Message,
						   int				MessageLen,
						   unsigned char*	Bsn,
						   int				BsnLen,
						   unsigned char*	Signature,
						   int				SignatureLen,
						   CdgResult*		VerifRes);

/*	Function:
 *		CreateHmac
 *	Descritpion:
 *		Create HMAC of the message using SHA256 algorithm
 *	Input arguments:
 *		IN	*Message		- pointer to the buffer containing the message
 *		IN	MessageLen		- length of the message buffer
 *		IN	*Mk				- pointer to the buffer containing MAC key
 *		IN	MkLen			- MAC key buffer length
 *		OUT	*Hmac			- pointer to the buffer for generated HMAC of the message
 *		IN	HmacLen			- HMAC buffer length >= 32 bytes
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus CreateHmac(unsigned char*		Message,
					 int				MessageLen,
					 unsigned char*		Mk,
					 int				MkLen,
					 unsigned char*		Hmac,
					 int				HmacLen);


/*	Function:
 *		VerifyHmac
 *	Description:
 *		Verify the HMAC of the message using SHA256 algorithm
 *	Input arguments:
 *		IN	*Message			- pointer to the buffer containing the message which HMAC will be verified
 *		IN	MessageLen			- message buffer length
 *		IN	*Hmac				- pointer to the buffer containing the HMAC of the message
 *		IN	HmacLen				- length of the HMAC buffer
  *		IN	*MacKey				- pointer to the buffer containing the MAC key which was used to compute oryginal HMAC
 *		IN	MacKeyLen			- length of the MAC key buffer
 *		OUT	*VerifRes			- pointer to the verification result
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus VerifyHmac(unsigned char*	Message,
					 int			MessageLen,
					 unsigned char*	Hmac,
					 int			HmacLen,
					 unsigned char*	MacKey,
					 int			MacKeyLen,
					 CdgResult*		VerifRes);

/*	Function:
 *		DeriveSigmaKeys
 *	Description:
 *		Derive SIGMA Session Key and MAC key using serialized Ga from PCH
 *	Input arguments:
 *		IN	*Ga					- pioter to the buffer containing serialized Ga
 *		IN	GaLen				- Ga buffer length
 *		OUT	*Gb					- pointer to the buffer for computed and serialized Gb
 *		IN	GbLen				- Gb buffer length
 *		OUT	*Sk					- pointer to the buffer for derived and serialized SIGMA Session Key
 *		IN	SkLen				- Session Key buffer length
 *		OUT	*Mk					- pointer to the buffer for derived and serialized SIGMA MAC Key
 *		IN	MkLen				- MAC Key buffer length
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus DeriveSigmaKeys(unsigned char*	Ga,
						  int				GaLen,
						  unsigned char*	Gb,
						  int				GbLen,
						  unsigned char*	Sk,
						  int				SkLen,
						  unsigned char*	Mk,
						  int				MkLen);

/*	Function:
 *		Aes128EcbEncrypt
 *	Description:
 *		Encrypt a message using AES-128 ECB
 *	Input arguments:
 *		IN	*ClrMsg				- pointer to the buffer containing a message to be encrypted
 *		IN	ClrMsgLen			- clear message buffer length
 *		OUT	*EncMsg				- pointer to the buffer for encrypted message
 *		IN	EncMsgLen			- encrypted message buffer length
 *		IN	*AesKey				- pointer to the buffer containing AES encryption key
 *		IN	AesKeyLen			- AES encryption key length
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus Aes128EcbEncrypt(unsigned char*	ClrMsg,
						   int				ClrMsgLen,
						   unsigned char*	EncMsg,
						   int				EncMsgLen,
						   unsigned char*	AesKey,
						   int				AesKeyLen);

/*	Function:
 *		Aes128EcbDecrypt
 *	Description:
 *		Decrypt a message using AES-128 ECB
 *	Input arguments:
 *		OUT	*EncMsg				- pointer to the buffer containing a message to be decrypted
 *		IN	EncMsgLen			- encrypted message buffer length
 *		IN	*ClrMsg				- pointer to the buffer for decrypted message
 *		IN	ClrMsgLen			- clear message buffer length
 *		IN	*AesKey				- pointer to the buffer containing AES decryption key
 *		IN	AesKeyLen			- AES decryption key length
 *	Return values:
 *		Operation status code (see cdg_status.h for details)
 */
CRYPTODATAGEN_API
CdgStatus Aes128EcbDecrypt(unsigned char*	EncMsg,
						   int				EncMsgLen,
						   unsigned char*	ClrMsg,
						   int				ClrMsgLen,
						   unsigned char*	AesKey,
						   int				AesKeyLen);

CRYPTODATAGEN_API
CdgStatus Aes128CbcEncrypt(unsigned char*	ClrMsg,
						   int				ClrMsgLen,
						   unsigned char*	EncMsg,
						   int				EncMsgLen,
						   unsigned char*	AesKey,
						   int				AesKeyLen,
                           unsigned int     IvVal[4]);

CRYPTODATAGEN_API
CdgStatus Aes128CbcDecrypt(unsigned char*	EncMsg,
						   int				EncMsgLen,
						   unsigned char*	ClrMsg,
						   int				ClrMsgLen,
						   unsigned char*	AesKey,
						   int				AesKeyLen,
                           unsigned int     IvVal[4]);

CRYPTODATAGEN_API
CdgStatus Aes128CtrEncrypt(unsigned char* ClrMsg, int ClrMsgLen, unsigned char* EncMsg, int EncMsgLen, unsigned char* AesKey, int AesKeyLen, unsigned int CtrVal[4], int CtrSize);

CRYPTODATAGEN_API
CdgStatus Aes128CtrDecrypt(unsigned char* EncMsg, int EncMsgLen, unsigned char* ClrMsg, int ClrMsgLen, unsigned char* AesKey, int AesKeyLen, unsigned int CtrVal[4], int CtrSize);

// <<<<< cpt audio
CRYPTODATAGEN_API
CdgStatus Aes128CtrEncryptLe(unsigned char* ClrMsg, int ClrMsgLen, unsigned char* EncMsg, int EncMsgLen, unsigned char* AesKey, int AesKeyLen, unsigned int CtrVal[4]);
// >>>>> cpt audio
#endif