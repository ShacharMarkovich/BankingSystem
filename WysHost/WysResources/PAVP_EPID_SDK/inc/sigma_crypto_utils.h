/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/
#ifndef _ECDSA_KEYGEN_H
#define _ECDSA_KEYGEN_H

#include "ippcp.h"

// IPP BN + ECPF
#include "bignumber.h"
#include "ecpf.h"

// SafeID SIK headers
#include "epid_errors.h"
#include "epid_types.h"
#include "epid_macros.h"
#include <epid_verifier.h>

#include <serialization.h>
#include <ecdsa_crypto.h>
#include <prng.h>

#include "cdg_status_codes.h"

// LE2BE translation macros
#include <le2be_macros.h>

using namespace std;

//Intel's public key to verify signatures from the DCP signing facility
const unsigned char INTEL_ECDSA_PUBKEY[ECDSA_PUBLIC_KEY_SIZE] = {
    0x7b, 0xe2, 0xc7, 0xc7, 0xe6, 0xc4, 0xb4, 0x3c, 
    0x02, 0x8b, 0x67, 0xcc, 0xd0, 0x1e, 0x80, 0xc2,
    0x35, 0x2d, 0x1a, 0x4e, 0x4a, 0x81, 0x6e, 0x06, 
    0x00, 0x4c, 0xb0, 0xa4, 0x2e, 0x65, 0xbd, 0x27,
    0x52, 0x07, 0x71, 0x62, 0xcb, 0xb9, 0x3c, 0x48, 
    0x42, 0xb2, 0x2a, 0xc7, 0x84, 0xa6, 0xca, 0x12,
    0x0f, 0xc7, 0x9f, 0x6c, 0x9f, 0xbd, 0x3f, 0x38, 
    0xd5, 0xc6, 0x1b, 0x44, 0x16, 0x3e, 0xc8, 0xae
};


// SIGMA Crypto Utilities
class SigmaCryptoUtils {
public:
	SigmaCryptoUtils();
	~SigmaCryptoUtils();

	// Initialize the generator
	CdgStatus Init();

	//GID
	static int current_gid;

	CdgStatus MsgSign3p(Ipp8u* PrivKey3p, int PrivKey3pLen, Ipp8u* Msg, int MsgLen, Ipp8u* Signature, int SignatureLen);
	CdgStatus VerifyCertPchSigIntel(Ipp8u* PubCertPch, int PubCertPchLen, CdgResult* VerifRes);
    CdgStatus MsgVerifyPch(Ipp8u* PubKeyPch, int PubKeyPchLen, Ipp8u* Msg, int MsgLen, Ipp8u* Bsn, int BsnLen, Ipp8u* Signature, int SignatureLen, CdgResult* VerifRes);
	CdgStatus DeriveSkMk(Ipp8u* Ga, int GaLen, Ipp8u* Gb, int GbLen, Ipp8u* Sk, int SkLen, Ipp8u* Mk, int MkLen);

	// HMAC calculation/verification
	static CdgStatus MsgCreateHmac(Ipp8u* Msg, int MsgLen, Ipp8u* Mk, int MkLen, Ipp8u* Hmac, int HmacLen);
	static CdgStatus MsgVerifyHmac(Ipp8u* Msg, int MsgLen, Ipp8u* Hmac, int HmacLen, Ipp8u* MacKey, int MacKeyLen, CdgResult* VerifRes);

	// AES-ECB encryption/decryption
	static CdgStatus AesEcbEncrypt(Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* EncMsg, int EncMsgLen, Ipp8u* AesKey, int AesKeyLen);
	static CdgStatus AesEcbDecrypt(Ipp8u* EncMsg, int EncMsgLen, Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* AesKey, int AesKeyLen);

	// AES-CBC encryption/decryption
	static CdgStatus AesCbcEncrypt(Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* EncMsg, int EncMsgLen, Ipp8u* AesKey, int AesKeyLen, Ipp32u IvVal[4]);
	static CdgStatus AesCbcDecrypt(Ipp8u* EncMsg, int EncMsgLen, Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* AesKey, int AesKeyLen, Ipp32u IvVal[4]);

	// AES-CTR encryption/decryption
	static CdgStatus AesCtrEncrypt(Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* EncMsg, int EncMsgLen, Ipp8u* AesKey, int AesKeyLen, Ipp32u CtrVal[4], int CtrSize);
	static CdgStatus AesCtrDecrypt(Ipp8u* EncMsg, int EncMsgLen, Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* AesKey, int AesKeyLen, Ipp32u CtrVal[4], int CtrSize);
   // <<<<< cpt audio
   static CdgStatus AesCtrEncryptLe(Ipp8u* ClrMsg, int ClrMsgLen, Ipp8u* EncMsg, int EncMsgLen, Ipp8u* AesKey, int AesKeyLen, Ipp32u CtrVal[4]);
   // >>>>> cpt audio
private:
	bool					_initialized;

	// >>>> SAFEID SIK MIGRATION <<<<
	// Parameter set for SafeId scheme
	EPIDParameterCertificateBlob	EpidParamsCert;
	// PRNG
	IppsPRNGState*			PrngCtx;
	// G3 elliptic curve used for SIGMA and EC-DSA operations
	G3ECPrimeField			G3;
	G3Element*				IntelVerifKey;

	// Initialize/delete PRNG context for EC-DSA key generation
	void InitPrngCtx();
	void FreePrngCtx();

	// Generate EC-DSA key pair
	CdgStatus EcDsaGenKeyPair(BigNum& EcDsaPrivKey, G3Element& EcDsaPubKey);

	// EC-DSA keys serialization/deserialization
	CdgStatus EcDsaPrivKeySerialize(BigNum& EcDsaPrivKey, Ipp8u* SerialPrivKey, int PrivKeyLen);
	CdgStatus EcDsaPrivKeyDeserialize(BigNum& EcDsaPrivKey, Ipp8u* SerialPrivKey, int PrivKeyLen);
	CdgStatus EcDsaPubKeySerialize(G3Element& EcDsaPubKey, Ipp8u* SerialPubKey, int PubKeyLen);
	CdgStatus EcDsaPubKeyDeserialize(G3Element& EcDsaPubKey, Ipp8u* SerialPubKey, int PubKeyLen);

	// EC-DSA sign/verify
	CdgStatus EcDsaSign(BigNum& EcDsaPrivKey, Ipp8u* Msg, int MsgLen, Ipp8u* Signature, int SignatureLen);
	CdgStatus EcDsaVerify(G3Element& EcDsaPubKey, Ipp8u* Msg, int MsgLen, Ipp8u* Signature, int SignatureLen, CdgResult* VerifRes);
};

#endif