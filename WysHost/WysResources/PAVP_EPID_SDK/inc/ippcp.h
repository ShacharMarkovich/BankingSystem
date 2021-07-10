/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2009 Intel Corporation. All Rights Reserved.
//
//              Intel(R) Integrated Performance Primitives
//                  Cryptographic Primitives (ippCP)
//
*/

#if !defined( __IPPCP_H__ ) || defined( _OWN_BLDPCS )
#define __IPPCP_H__


#if defined (_WIN32_WCE) && defined (_M_IX86) && defined (__stdcall)
  #define _IPP_STDCALL_CDECL
  #undef __stdcall
#endif


#ifndef __IPPDEFS_H__
  #include "ippdefs.h"
#endif

#ifdef  __cplusplus
extern "C" {
#endif


/* /////////////////////////////////////////////////////////////////////////////
//  Name:       ippcpGetLibVersion
//  Purpose:    getting of the library version
//  Returns:    the structure of information about version of ippCP library
//  Parameters:
//
//  Notes:      not necessary to release the returned structure
*/
IPPAPI( const IppLibraryVersion*, ippcpGetLibVersion, (void) )



/*
// Symmetric Ciphers
*/

#if !defined( _OWN_BLDPCS )

typedef enum {
    NONE  = 0, IppsCPPaddingNONE  = 0,
    PKCS7 = 1, IppsCPPaddingPKCS7 = 1,
    ZEROS = 2, IppsCPPaddingZEROS = 2
} IppsCPPadding;

typedef struct _cpDES         IppsDESSpec;
typedef struct _cpBlowfish    IppsBlowfishSpec;
typedef struct _cpTwofish     IppsTwofishSpec;
typedef struct _cpRijndael128 IppsRijndael128Spec;
typedef struct _cpRijndael192 IppsRijndael192Spec;
typedef struct _cpRijndael256 IppsRijndael256Spec;
typedef struct _cpRC5_64      IppsARCFive64Spec;
typedef struct _cpRC5_128     IppsARCFive128Spec;

typedef struct _cpRijndael128GCM IppsRijndael128GCMState;
typedef struct _cpRijndael128CCM IppsRijndael128CCMState;

#endif /* _OWN_BLDPCS */

/*
// DES/TDES
*/
#define  DES_BLOCKSIZE  (64)  /* cipher blocksize (bits) */
#define TDES_BLOCKSIZE  DES_BLOCKSIZE

#define  DES_KEYSIZE    (64) /*     cipher keysize (bits) */
#define TDES_KEYSIZE    DES_KEYSIZE

IPPAPI(IppStatus, ippsDESGetSize,(int *size))
IPPAPI(IppStatus, ippsDESInit,(const Ipp8u* pKey, IppsDESSpec* pCtx))

IPPAPI(IppStatus, ippsDESEncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsDESSpec* pCtx,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsDESDecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsDESSpec* pCtx,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsDESEncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsDESSpec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsDESDecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsDESSpec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsDESEncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsDESSpec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsDESDecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsDESSpec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsDESEncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                     const IppsDESSpec* pCtx,
                                     Ipp8u* pIV))
IPPAPI(IppStatus, ippsDESDecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                     const IppsDESSpec* pCtx,
                                     Ipp8u* pIV))

IPPAPI(IppStatus, ippsDESEncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsDESSpec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsDESDecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsDESSpec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))


IPPAPI(IppStatus, ippsTDESEncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      IppsCPPadding padding))
IPPAPI(IppStatus, ippsTDESDecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      IppsCPPadding padding))

IPPAPI(IppStatus, ippsTDESEncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      const Ipp8u* pIV,
                                      IppsCPPadding padding))
IPPAPI(IppStatus, ippsTDESDecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      const Ipp8u* pIV,
                                      IppsCPPadding padding))

IPPAPI(IppStatus, ippsTDESEncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      const Ipp8u* pIV,
                                      IppsCPPadding padding))
IPPAPI(IppStatus, ippsTDESDecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                      const IppsDESSpec* pCtx1, const IppsDESSpec* pCtx2, const IppsDESSpec* pCtx3,
                                      const Ipp8u* pIV,
                                      IppsCPPadding padding))

IPPAPI(IppStatus, ippsTDESEncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                     const IppsDESSpec* pCtx1,
                                     const IppsDESSpec* pCtx2,
                                     const IppsDESSpec* pCtx3,
                                     Ipp8u* pIV))
IPPAPI(IppStatus, ippsTDESDecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                     const IppsDESSpec* pCtx1,
                                     const IppsDESSpec* pCtx2,
                                     const IppsDESSpec* pCtx3,
                                     Ipp8u* pIV))

IPPAPI(IppStatus, ippsTDESEncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsDESSpec* pCtx1,
                                      const IppsDESSpec* pCtx2,
                                      const IppsDESSpec* pCtx3,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsTDESDecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsDESSpec* pCtx1,
                                      const IppsDESSpec* pCtx2,
                                      const IppsDESSpec* pCtx3,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))

/*
// Blowfish
*/
#define BF_BLOCKSIZE    (64)  /* cipher blocksize (bits) */
#define BF_KEYSIZE_MAX  (448) /* max cipher keysize (bits) */

IPPAPI(IppStatus, ippsBlowfishGetSize,(int *pSize))
IPPAPI(IppStatus, ippsBlowfishInit,(const Ipp8u* pKey, int keyLen, IppsBlowfishSpec* pCtx))

IPPAPI(IppStatus, ippsBlowfishEncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          IppsCPPadding padding))
IPPAPI(IppStatus, ippsBlowfishDecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          IppsCPPadding padding))

IPPAPI(IppStatus, ippsBlowfishEncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          const Ipp8u* pIV,
                                          IppsCPPadding padding))
IPPAPI(IppStatus, ippsBlowfishDecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          const Ipp8u* pIV,
                                          IppsCPPadding padding))

IPPAPI(IppStatus, ippsBlowfishEncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                          const IppsBlowfishSpec* pCtx,
                                          const Ipp8u* pIV,
                                          IppsCPPadding padding))
IPPAPI(IppStatus, ippsBlowfishDecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                          const IppsBlowfishSpec* pCtx,
                                          const Ipp8u* pIV,
                                          IppsCPPadding padding))

IPPAPI(IppStatus, ippsBlowfishEncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                          const IppsBlowfishSpec* pCtx,
                                          Ipp8u* pIV))
IPPAPI(IppStatus, ippsBlowfishDecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                          const IppsBlowfishSpec* pCtx,
                                          Ipp8u* pIV))

IPPAPI(IppStatus, ippsBlowfishEncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsBlowfishDecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                          const IppsBlowfishSpec* pCtx,
                                          Ipp8u* pCtrValue, int ctrNumBitSize))

/*
// Twofish
*/
#define TF_BLOCKSIZE    (128) /* cipher blocksize (bits) */
#define TF_KEYSIZE_MIN  (128) /* min cipher keysize (bits) */
#define TF_KEYSIZE_MAX  (256) /* max cipher keysize (bits) */

IPPAPI(IppStatus, ippsTwofishGetSize,(int *pSize))
IPPAPI(IppStatus, ippsTwofishInit,(const Ipp8u* pKey, int keyLen, IppsTwofishSpec* pCtx))

IPPAPI(IppStatus, ippsTwofishEncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         IppsCPPadding padding))
IPPAPI(IppStatus, ippsTwofishDecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         IppsCPPadding padding))

IPPAPI(IppStatus, ippsTwofishEncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         const Ipp8u* pIV,
                                         IppsCPPadding padding))
IPPAPI(IppStatus, ippsTwofishDecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         const Ipp8u* pIV,
                                         IppsCPPadding padding))

IPPAPI(IppStatus, ippsTwofishEncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                         const IppsTwofishSpec* pCtx,
                                         const Ipp8u* pIV,
                                         IppsCPPadding padding))
IPPAPI(IppStatus, ippsTwofishDecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                         const IppsTwofishSpec* pCtx,
                                         const Ipp8u* pIV,
                                         IppsCPPadding padding))

IPPAPI(IppStatus, ippsTwofishEncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                         const IppsTwofishSpec* pCtx,
                                         Ipp8u* pIV))
IPPAPI(IppStatus, ippsTwofishDecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                         const IppsTwofishSpec* pCtx,
                                         Ipp8u* pIV))

IPPAPI(IppStatus, ippsTwofishEncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsTwofishDecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                         const IppsTwofishSpec* pCtx,
                                         Ipp8u* pCtrValue, int ctrNumBitSize))

/*
// Rijndael
*/
#define RIJNDAEL128_BLOCKSIZE (128) /* cipher blocksizes (bits) */
#define RIJNDAEL192_BLOCKSIZE (192)
#define RIJNDAEL256_BLOCKSIZE (256)

#if !defined( _OWN_BLDPCS )

typedef enum {
   IppsRijndaelKey128 = 128, /* 128-bit key */
   IppsRijndaelKey192 = 192, /* 192-bit key */
   IppsRijndaelKey256 = 256  /* 256-bit key */
} IppsRijndaelKeyLength;


#endif /* _OWN_BLDPCS */

/* Rijndael of 128-bit block size (AES) */
IPPAPI(IppStatus, ippsRijndael128GetSize,(int *pSize))
IPPAPI(IppStatus, ippsRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128Spec* pCtx))
IPPAPI(IppStatus, ippsSafeRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128Spec* pCtx))

IPPAPI(IppStatus, ippsRijndael128EncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael128Spec* pCtx,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael128DecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael128Spec* pCtx,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael128EncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael128Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael128DecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael128Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael128EncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael128Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael128DecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael128Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael128EncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael128Spec* pCtx,
                                             Ipp8u* pIV))
IPPAPI(IppStatus, ippsRijndael128DecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael128Spec* pCtx,
                                             Ipp8u* pIV))

IPPAPI(IppStatus, ippsRijndael128EncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael128Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsRijndael128DecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael128Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))

IPP_DEPRECATED("use ippsRijndael128CCMEncryptMessage function instead of this one")\
IPPAPI(IppStatus, ippsRijndael128EncryptCCM,(const Ipp8u* pNonce,  Ipp32u lenN,
                                             const Ipp8u* pHeader, Ipp64u lenH,
                                             const Ipp8u* pPtext,  Ipp64u lenP,
                                             int macLen,
                                                   Ipp8u* pCtext,
                                             const IppsRijndael128Spec* pCtx))
IPP_DEPRECATED("use ippsRijndael128CCMDecryptMessage function instead of this one")\
IPPAPI(IppStatus, ippsRijndael128DecryptCCM,(const Ipp8u* pNonce,  Ipp32u lenN,
                                             const Ipp8u* pHeader, Ipp64u lenH,
                                             const Ipp8u* pCtext,  Ipp64u lenC,
                                             int macLen,
                                                   Ipp8u* pPtext,  IppBool* pResult,
                                             const IppsRijndael128Spec* pCtx))

IPP_DEPRECATED("use ippsRijndael128CCMEncryptMessage function instead of this one")\
IPPAPI(IppStatus, ippsRijndael128EncryptCCM_u8,(const Ipp8u* pNonce,  int lenN,
                                                const Ipp8u* pHeader, int lenH,
                                                const Ipp8u* pPtext,  int lenP,
                                                int macLen,
                                                      Ipp8u* pCtext,
                                                const IppsRijndael128Spec* pCtx))
IPP_DEPRECATED("use ippsRijndael128CCMDecryptMessage function instead of this one")\
IPPAPI(IppStatus, ippsRijndael128DecryptCCM_u8,(const Ipp8u* pNonce,  int lenN,
                                                const Ipp8u* pHeader, int lenH,
                                                const Ipp8u* pCtext,  int lenC,
                                                int macLen,
                                                      Ipp8u* pPtext,  IppBool* pResult,
                                                const IppsRijndael128Spec* pCtx))

/* AES-CCM authentication & confidelity */
IPPAPI(IppStatus, ippsRijndael128CCMGetSize,(Ipp32u* pSize))
IPPAPI(IppStatus, ippsRijndael128CCMInit,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                       IppsRijndael128CCMState* pState))

IPPAPI(IppStatus, ippsRijndael128CCMMessageLen,(Ipp64u msgLen, IppsRijndael128CCMState* pState))
IPPAPI(IppStatus, ippsRijndael128CCMTagLen,(Ipp32u tagLen, IppsRijndael128CCMState* pState))

IPPAPI(IppStatus, ippsRijndael128CCMStart,(const Ipp8u* pIV,  Ipp32u ivLen,
                                       const Ipp8u* pAAD, Ipp32u aadLen,
                                       IppsRijndael128CCMState* pState))
IPPAPI(IppStatus, ippsRijndael128CCMEncrypt,(const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                       IppsRijndael128CCMState* pState))
IPPAPI(IppStatus, ippsRijndael128CCMDecrypt,(const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                       IppsRijndael128CCMState* pState))
IPPAPI(IppStatus, ippsRijndael128CCMGetTag,(Ipp8u* pTag, Ipp32u tagLen,
                                       const IppsRijndael128CCMState* pState))

IPPAPI(IppStatus, ippsRijndael128CCMEncryptMessage,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                        const Ipp8u* pIV,  Ipp32u ivLen,
                                        const Ipp8u* pAAD, Ipp32u aadLen,
                                        const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                              Ipp8u* pTag, Ipp32u tagLen))
IPPAPI(IppStatus, ippsRijndael128CCMDecryptMessage,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                        const Ipp8u* pIV,  Ipp32u ivLen,
                                        const Ipp8u* pAAD, Ipp32u aadLen,
                                        const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                              Ipp8u* pTag, Ipp32u tagLen))

/* AES-GCM authentication & confidelity */
IPPAPI(IppStatus, ippsRijndael128GCMGetSize,(Ipp32u* pSize))
IPPAPI(IppStatus, ippsRijndael128GCMInit,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                       IppsRijndael128GCMState* pState))
IPPAPI(IppStatus, ippsRijndael128GCMStart,(const Ipp8u* pIV,  Ipp32u ivLen,
                                       const Ipp8u* pAAD, Ipp32u aadLen,
                                       IppsRijndael128GCMState* pState))
IPPAPI(IppStatus, ippsRijndael128GCMEncrypt,(const Ipp8u* pSrc, Ipp8u* pDst,  Ipp32u len,
                                       IppsRijndael128GCMState* pState))
IPPAPI(IppStatus, ippsRijndael128GCMDecrypt,(const Ipp8u* pSrc, Ipp8u* pDst,  Ipp32u len,
                                       IppsRijndael128GCMState* pState))
IPPAPI(IppStatus, ippsRijndael128GCMGetTag,(Ipp8u* pDstTag, Ipp32u tagLen,
                                       const IppsRijndael128GCMState* pState))
IPPAPI(IppStatus, ippsRijndael128GCMEncryptMessage,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                       const Ipp8u* pIV,  Ipp32u ivLen,
                                       const Ipp8u* pAAD, Ipp32u aadLen,
                                       const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                       Ipp8u* pTag, Ipp32u tagLen))
IPPAPI(IppStatus, ippsRijndael128GCMDecryptMessage,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                       const Ipp8u* pIV,  Ipp32u ivLen,
                                       const Ipp8u* pAAD, Ipp32u aadLen,
                                       const Ipp8u* pSrc, Ipp8u* pDst, Ipp32u len,
                                             Ipp8u* pDstTag, Ipp32u tagLen))

/* Rijndael of 192-bit block size */
IPPAPI(IppStatus, ippsRijndael192GetSize,(int *pSize))
IPPAPI(IppStatus, ippsRijndael192Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen, IppsRijndael192Spec* pCtx))

IPPAPI(IppStatus, ippsRijndael192EncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael192Spec* pCtx,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael192DecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael192Spec* pCtx,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael192EncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael192Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael192DecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael192Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael192EncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael192Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael192DecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael192Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael192EncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael192Spec* pCtx,
                                             Ipp8u* pIV))
IPPAPI(IppStatus, ippsRijndael192DecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael192Spec* pCtx,
                                             Ipp8u* pIV))

IPPAPI(IppStatus, ippsRijndael192EncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael192Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsRijndael192DecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael192Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))

/* Rijndael of 256-bit block size */
IPPAPI(IppStatus, ippsRijndael256GetSize,(int *pSize))
IPPAPI(IppStatus, ippsRijndael256Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen, IppsRijndael256Spec* pCtx))

IPPAPI(IppStatus, ippsRijndael256EncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael256Spec* pCtx,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael256DecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael256Spec* pCtx,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael256EncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael256Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael256DecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                             const IppsRijndael256Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael256EncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael256Spec* pCtx,
                                             Ipp8u* pIV))
IPPAPI(IppStatus, ippsRijndael256DecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int ofbBlkSize,
                                             const IppsRijndael256Spec* pCtx,
                                             Ipp8u* pIV))

IPPAPI(IppStatus, ippsRijndael256EncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael256Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))
IPPAPI(IppStatus, ippsRijndael256DecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int len, int cfbBlkSize,
                                             const IppsRijndael256Spec* pCtx,
                                             const Ipp8u* pIV,
                                             IppsCPPadding padding))

IPPAPI(IppStatus, ippsRijndael256EncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael256Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsRijndael256DecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                      const IppsRijndael256Spec* pCtx,
                                      Ipp8u* pCtrValue, int ctrNumBitSize))

/*
// RC5 of 64--bit block size
*/
#define ARCFIVE_64_BLOCKSIZE  (64)  /* cipher blockizes (bits) */
#define ARCFIVE_64_KEY_MAX    (255) /* cipher key max len (bytes) */
#define ARCFIVE_64_ROUNDS_MAX (255) /* max num of rounds */

IPPAPI(IppStatus, ippsARCFive64GetSize,(int rounds, int* pSize))
IPPAPI(IppStatus, ippsARCFive64Init,(const Ipp8u *pKey, int keylen,
                                     int rounds,
                                     IppsARCFive64Spec* pCtx))

IPPAPI(IppStatus, ippsARCFive64EncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive64Spec* pCtx,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive64DecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive64Spec* pCtx,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive64EncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive64Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive64DecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive64Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive64EncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive64Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive64DecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive64Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive64EncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive64Spec* pCtx,
                                     Ipp8u* pIV))
IPPAPI(IppStatus, ippsARCFive64DecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive64Spec* pCtx,
                                     Ipp8u* pIV))

IPPAPI(IppStatus, ippsARCFive64EncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsARCFive64Spec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsARCFive64DecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsARCFive64Spec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))

/*
// RC5 of 128-bit block size
*/
#define ARCFIVE_128_BLOCKSIZE    (128) /* cipher blocksize (bits) */
#define ARCFIVE_128_KEY_MAX      (255) /* cipher key max len (bytes) */
#define ARCFIVE_128_ROUNDS_MAX   (255) /* max num of rounrs */

IPPAPI(IppStatus, ippsARCFive128GetSize,(int rounds, int *size))
IPPAPI(IppStatus, ippsARCFive128Init,(const Ipp8u *pKey, int keylen,
                                      int rounds,
                                      IppsARCFive128Spec* pCtx))

IPPAPI(IppStatus, ippsARCFive128EncryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive128Spec* pCtx,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive128DecryptECB,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive128Spec* pCtx,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive128EncryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive128Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive128DecryptCBC,(const Ipp8u* pSrc, Ipp8u* pDst, int length,
                                     const IppsARCFive128Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive128EncryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive128Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))
IPPAPI(IppStatus, ippsARCFive128DecryptCFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive128Spec* pCtx,
                                     const Ipp8u* pIV,
                                     IppsCPPadding padding))

IPPAPI(IppStatus, ippsARCFive128EncryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive128Spec* pCtx,
                                     Ipp8u* pIV))
IPPAPI(IppStatus, ippsARCFive128DecryptOFB,(const Ipp8u* pSrc, Ipp8u* pDst, int length, int cfbBlkSize,
                                     const IppsARCFive128Spec* pCtx,
                                     Ipp8u* pIV))

IPPAPI(IppStatus, ippsARCFive128EncryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsARCFive128Spec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))
IPPAPI(IppStatus, ippsARCFive128DecryptCTR,(const Ipp8u* pSrc, Ipp8u* pDst, int len,
                                     const IppsARCFive128Spec* pCtx,
                                     Ipp8u* pCtrValue, int ctrNumBitSize))


/*
// Stream Ciphers
*/

/* ARCFOUR */
#define MAX_ARCFOUR_KEY_LEN   (256)  /* max key length (bytes) */

#if !defined( _OWN_BLDPCS )

typedef struct _cpARCfour  IppsARCFourState;

#endif /* _OWN_BLDPCS */

IPPAPI(IppStatus, ippsARCFourCheckKey, (const Ipp8u *pKey, int keyLen, IppBool* pIsWeak))

IPPAPI(IppStatus, ippsARCFourGetSize, (int* pSize))
IPPAPI(IppStatus, ippsARCFourInit, (const Ipp8u *pKey, int keyLen, IppsARCFourState *pCtx))
IPPAPI(IppStatus, ippsARCFourReset, (IppsARCFourState* pCtx))

IPPAPI(IppStatus, ippsARCFourEncrypt, (const Ipp8u *pSrc, Ipp8u *pDst, int length, IppsARCFourState *pCtx))
IPPAPI(IppStatus, ippsARCFourDecrypt, (const Ipp8u *pSrc, Ipp8u *pDst, int length, IppsARCFourState *pCtx))


/*
// One-Way Hash Functions
*/
#define   SHA1_DIGEST_BITSIZE  160   /* digest size (bits) */
#define SHA256_DIGEST_BITSIZE  256
#define SHA224_DIGEST_BITSIZE  224
#define SHA384_DIGEST_BITSIZE  384
#define SHA512_DIGEST_BITSIZE  512
#define    MD5_DIGEST_BITSIZE  128
                                     /* obsolete:          */
#define   SHA1_DIGEST_LENGTH   160   /* digest size (bits) */
#define SHA256_DIGEST_LENGTH   256
#define SHA224_DIGEST_LENGTH   224
#define SHA384_DIGEST_LENGTH   384
#define SHA512_DIGEST_LENGTH   512
#define    MD5_DIGEST_LENGTH   128

#if !defined( _OWN_BLDPCS )

typedef struct _cpSHA1     IppsSHA1State;
typedef struct _cpSHA224   IppsSHA224State;
typedef struct _cpSHA256   IppsSHA256State;
typedef struct _cpSHA512   IppsSHA384State;
typedef struct _cpSHA512   IppsSHA512State;
typedef struct _cpMD5      IppsMD5State;

/* MGF */
typedef IppStatus (__STDCALL *IppMGF)(const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen);

/* HASH functions */
typedef IppStatus (__STDCALL *IppHASH)(const Ipp8u* pMsg, int len, Ipp8u* pMD);

#endif /* _OWN_BLDPCS */

/* SHA1 Hash Primitives */
IPPAPI(IppStatus, ippsSHA1GetSize,(int* pSize))
IPPAPI(IppStatus, ippsSHA1Init,(IppsSHA1State* pCtx))
IPPAPI(IppStatus, ippsSHA1Duplicate,(const IppsSHA1State* pSrcCtx, IppsSHA1State* pDstCtx))

IPPAPI(IppStatus, ippsSHA1Update,(const Ipp8u* pSrc, int len, IppsSHA1State* pCtx))
IPPAPI(IppStatus, ippsSHA1GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsSHA1State* pCtx))
IPPAPI(IppStatus, ippsSHA1Final,(Ipp8u* pMD, IppsSHA1State* pCtx))
IPPAPI(IppStatus, ippsSHA1MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* SHA224 Hash Primitives */
IPPAPI(IppStatus, ippsSHA224GetSize,(int* pSize))
IPPAPI(IppStatus, ippsSHA224Init,(IppsSHA224State* pCtx))
IPPAPI(IppStatus, ippsSHA224Duplicate,(const IppsSHA224State* pSrcCtx, IppsSHA224State* pDstCtx))

IPPAPI(IppStatus, ippsSHA224Update,(const Ipp8u* pSrc, int len, IppsSHA224State* pCtx))
IPPAPI(IppStatus, ippsSHA224GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsSHA224State* pCtx))
IPPAPI(IppStatus, ippsSHA224Final,(Ipp8u* pMD, IppsSHA224State* pCtx))
IPPAPI(IppStatus, ippsSHA224MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* SHA256 Hash Primitives */
IPPAPI(IppStatus, ippsSHA256GetSize,(int* pSize))
IPPAPI(IppStatus, ippsSHA256Init,(IppsSHA256State* pCtx))
IPPAPI(IppStatus, ippsSHA256Duplicate,(const IppsSHA256State* pSrcCtx, IppsSHA256State* pDstCtx))

IPPAPI(IppStatus, ippsSHA256Update,(const Ipp8u* pSrc, int len, IppsSHA256State* pCtx))
IPPAPI(IppStatus, ippsSHA256GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsSHA256State* pCtx))
IPPAPI(IppStatus, ippsSHA256Final,(Ipp8u* pMD, IppsSHA256State* pCtx))
IPPAPI(IppStatus, ippsSHA256MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* SHA384 Hash Primitives */
IPPAPI(IppStatus, ippsSHA384GetSize,(int* pSize))
IPPAPI(IppStatus, ippsSHA384Init,(IppsSHA384State* pCtx))
IPPAPI(IppStatus, ippsSHA384Duplicate,(const IppsSHA384State* pSrcCtx, IppsSHA384State* pDstCtx))

IPPAPI(IppStatus, ippsSHA384Update,(const Ipp8u* pSrc, int len, IppsSHA384State* pCtx))
IPPAPI(IppStatus, ippsSHA384GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsSHA384State* pCtx))
IPPAPI(IppStatus, ippsSHA384Final,(Ipp8u* pMD, IppsSHA384State* pCtx))
IPPAPI(IppStatus, ippsSHA384MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* SHA512 Hash Primitives */
IPPAPI(IppStatus, ippsSHA512GetSize,(int* pSize))
IPPAPI(IppStatus, ippsSHA512Init,(IppsSHA512State* pCtx))
IPPAPI(IppStatus, ippsSHA512Duplicate,(const IppsSHA512State* pSrcCtx, IppsSHA512State* pDstCtx))

IPPAPI(IppStatus, ippsSHA512Update,(const Ipp8u* pSrc, int len, IppsSHA512State* pCtx))
IPPAPI(IppStatus, ippsSHA512GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsSHA512State* pCtx))
IPPAPI(IppStatus, ippsSHA512Final,(Ipp8u* pMD, IppsSHA512State* pCtx))
IPPAPI(IppStatus, ippsSHA512MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* MD5 Hash Primitives */
IPPAPI(IppStatus, ippsMD5GetSize,(int* pSize))
IPPAPI(IppStatus, ippsMD5Init,(IppsMD5State* pCtx))
IPPAPI(IppStatus, ippsMD5Duplicate,(const IppsMD5State* pSrcCtx, IppsMD5State* pDstCtx))

IPPAPI(IppStatus, ippsMD5Update,(const Ipp8u* pSrc, int len, IppsMD5State* pCtx))
IPPAPI(IppStatus, ippsMD5GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsMD5State* pCtx))
IPPAPI(IppStatus, ippsMD5Final,(Ipp8u* pMD, IppsMD5State* pCtx))
IPPAPI(IppStatus, ippsMD5MessageDigest,(const Ipp8u* pMsg, int len, Ipp8u* pMD))

/* HASH based MFG Primitives */
IPPAPI(IppStatus, ippsMGF_SHA1,  (const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))
IPPAPI(IppStatus, ippsMGF_SHA224,(const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))
IPPAPI(IppStatus, ippsMGF_SHA256,(const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))
IPPAPI(IppStatus, ippsMGF_SHA384,(const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))
IPPAPI(IppStatus, ippsMGF_SHA512,(const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))
IPPAPI(IppStatus, ippsMGF_MD5,   (const Ipp8u* pSeed, int seedLen, Ipp8u* pMask, int maskLen))


/*
// Keyed-Hash Message Authentication Codes
*/
#if !defined( _OWN_BLDPCS )

typedef struct _cpHMACSHA1    IppsHMACSHA1State;
typedef struct _cpHMACSHA256  IppsHMACSHA256State;
typedef struct _cpHMACSHA224  IppsHMACSHA224State;
typedef struct _cpHMACSHA384  IppsHMACSHA384State;
typedef struct _cpHMACSHA512  IppsHMACSHA512State;
typedef struct _cpHMACMD5     IppsHMACMD5State;

#endif /* _OWN_BLDPCS */

/* SHA1 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACSHA1GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACSHA1Init,(const Ipp8u* pKey, int keyLen, IppsHMACSHA1State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA1Duplicate,(const IppsHMACSHA1State* pSrcCtx, IppsHMACSHA1State* pDstCtx))

IPPAPI(IppStatus, ippsHMACSHA1Update,(const Ipp8u* pSrc, int len, IppsHMACSHA1State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA1GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACSHA1State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA1Final, (Ipp8u* pMD, int mdLen, IppsHMACSHA1State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA1MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                             const Ipp8u* pKey, int keyLen,
                                             Ipp8u* pMD, int mdLen))
/* SHA256 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACSHA256GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACSHA256Init,(const Ipp8u* pKey, int keyLen, IppsHMACSHA256State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA256Duplicate,(const IppsHMACSHA256State* pSrcCtx, IppsHMACSHA256State* pDstCtx))

IPPAPI(IppStatus, ippsHMACSHA256Update,(const Ipp8u* pSrc, int len, IppsHMACSHA256State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA256GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACSHA256State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA256Final, (Ipp8u* pMD, int mdLen, IppsHMACSHA256State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA256MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                               const Ipp8u* pKey, int keyLen,
                                               Ipp8u* pMD, int mdLen))
/* SHA224 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACSHA224GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACSHA224Init,(const Ipp8u* pKey, int keyLen, IppsHMACSHA224State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA224Duplicate,(const IppsHMACSHA224State* pSrcCtx, IppsHMACSHA224State* pDstCtx))

IPPAPI(IppStatus, ippsHMACSHA224Update,(const Ipp8u* pSrc, int len, IppsHMACSHA224State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA224GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACSHA224State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA224Final, (Ipp8u* pMD, int mdLen, IppsHMACSHA224State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA224MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                               const Ipp8u* pKey, int keyLen,
                                               Ipp8u* pMD, int mdLen))
/* SHA384 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACSHA384GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACSHA384Init,(const Ipp8u* pKey, int keyLen, IppsHMACSHA384State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA384Duplicate,(const IppsHMACSHA384State* pSrcCtx, IppsHMACSHA384State* pDstCtx))

IPPAPI(IppStatus, ippsHMACSHA384Update,(const Ipp8u* pSrc, int len, IppsHMACSHA384State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA384GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACSHA384State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA384Final, (Ipp8u* pMD, int mdLen, IppsHMACSHA384State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA384MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                               const Ipp8u* pKey, int keyLen,
                                               Ipp8u* pMD, int mdLen))
/* SHA512 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACSHA512GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACSHA512Init,(const Ipp8u* pKey, int keyLen, IppsHMACSHA512State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA512Duplicate,(const IppsHMACSHA512State* pSrcCtx, IppsHMACSHA512State* pDstCtx))

IPPAPI(IppStatus, ippsHMACSHA512Update,(const Ipp8u* pSrc, int len, IppsHMACSHA512State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA512GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACSHA512State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA512Final, (Ipp8u* pMD, int mdLen, IppsHMACSHA512State* pCtx))
IPPAPI(IppStatus, ippsHMACSHA512MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                               const Ipp8u* pKey, int keyLen,
                                               Ipp8u* pMD, int mdLen))
/* MD5 based HMAC Primitives */
IPPAPI(IppStatus, ippsHMACMD5GetSize,(int* size))
IPPAPI(IppStatus, ippsHMACMD5Init,(const Ipp8u* pKey, int keyLen, IppsHMACMD5State* pCtx))
IPPAPI(IppStatus, ippsHMACMD5Duplicate,(const IppsHMACMD5State* pSrcCtx, IppsHMACMD5State* pDstCtx))

IPPAPI(IppStatus, ippsHMACMD5Update,(const Ipp8u* pSrc, int len, IppsHMACMD5State* pCtx))
IPPAPI(IppStatus, ippsHMACMD5GetTag,(Ipp8u* pTag, int tagLen, const IppsHMACMD5State* pCtx))
IPPAPI(IppStatus, ippsHMACMD5Final, (Ipp8u* pMD, int mdLen, IppsHMACMD5State* pCtx))
IPPAPI(IppStatus, ippsHMACMD5MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                            const Ipp8u* pKey, int keyLen,
                                            Ipp8u* pMD, int mdLen))


/*
// Data Authentication Codes
*/

#if !defined( _OWN_BLDPCS )

typedef struct _cpDAADES            IppsDAADESState;
typedef struct _cpDAATDES           IppsDAATDESState;
typedef struct _cpDAABlowfish       IppsDAABlowfishState;
typedef struct _cpDAATwofish        IppsDAATwofishState;
typedef struct _cpDAARijndael128    IppsDAARijndael128State;
typedef struct _cpDAARijndael192    IppsDAARijndael192State;
typedef struct _cpDAARijndael256    IppsDAARijndael256State;
typedef struct _cpXCBCRijndael128   IppsXCBCRijndael128State;

#endif /* _OWN_BLDPCS */

/* DES based DAA Primitives */
IPPAPI(IppStatus, ippsDAADESGetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAADESInit,(const Ipp8u* pKey,
                                  IppsDAADESState* pCtx))

IPPAPI(IppStatus, ippsDAADESUpdate,(const Ipp8u* pSrc, int len, IppsDAADESState* pCtx))
IPPAPI(IppStatus, ippsDAADESFinal, (Ipp8u* pMD, int mdLen, IppsDAADESState* pCtx))
IPPAPI(IppStatus, ippsDAADESMessageDigest,(const Ipp8u* pMsg, int msgLen,
                                           const Ipp8u* pKey,
                                           Ipp8u* pMD, int mdLen))
/* TDES based DAA Primitives */
IPPAPI(IppStatus, ippsDAATDESGetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAATDESInit,(const Ipp8u* pKey1, const Ipp8u* pKey2, const Ipp8u* pKey3,
                                   IppsDAATDESState* pCtx))

IPPAPI(IppStatus, ippsDAATDESUpdate,(const Ipp8u* pSrc, int len, IppsDAATDESState* pCtx))
IPPAPI(IppStatus, ippsDAATDESFinal,(Ipp8u* pMD, int mdLen, IppsDAATDESState* pCtx))
IPPAPI(IppStatus, ippsDAATDESMessageDigest,(const Ipp8u* pMsg, int msgLen,
                                            const Ipp8u* pKey1, const Ipp8u* pKey2, const Ipp8u* pKey3,
                                            Ipp8u* pMD, int mdLen))
/* Blowfish based DAA Primitives */
IPPAPI(IppStatus, ippsDAABlowfishGetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAABlowfishInit,(const Ipp8u* pKey, int keyLen,
                                       IppsDAABlowfishState* pCtx))

IPPAPI(IppStatus, ippsDAABlowfishUpdate,(const Ipp8u* pSrc, int len, IppsDAABlowfishState* pCtx))
IPPAPI(IppStatus, ippsDAABlowfishFinal,(Ipp8u* pMD, int mdLen, IppsDAABlowfishState* pCtx))
IPPAPI(IppStatus, ippsDAABlowfishMessageDigest,(const Ipp8u* pMsg, int msgLen,
                                                const Ipp8u* pKey, int keyLen,
                                                Ipp8u* pMD, int mdLen))
/* Twofish based DAA Primitives */
IPPAPI(IppStatus, ippsDAATwofishGetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAATwofishInit,(const Ipp8u* pKey, int keyLen,
                                      IppsDAATwofishState* pCtx))

IPPAPI(IppStatus, ippsDAATwofishUpdate,(const Ipp8u* pSrc, int len, IppsDAATwofishState* pCtx))
IPPAPI(IppStatus, ippsDAATwofishFinal,(Ipp8u* pMD, int mdLen, IppsDAATwofishState* pCtx))
IPPAPI(IppStatus, ippsDAATwofishMessageDigest,(const Ipp8u* pMsg, int msgLen,
                                               const Ipp8u* pKey, int keyLen,
                                               Ipp8u* pMD, int mdLen))
/* Rijndael128 based DAA Primitives */
IPPAPI(IppStatus, ippsDAARijndael128GetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAARijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsDAARijndael128State* pCtx))
IPPAPI(IppStatus, ippsDAASafeRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsDAARijndael128State* pCtx))

IPPAPI(IppStatus, ippsDAARijndael128Update,(const Ipp8u* pSrc, int len, IppsDAARijndael128State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael128Final,(Ipp8u* pMD, int mdLen, IppsDAARijndael128State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael128MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                                   const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                                   Ipp8u* pMD, int mdLen))
/* Rijndael192 based DAA Primitives */
IPPAPI(IppStatus, ippsDAARijndael192GetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAARijndael192Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsDAARijndael192State* pCtx))

IPPAPI(IppStatus, ippsDAARijndael192Update,(const Ipp8u* pSrc, int len, IppsDAARijndael192State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael192Final,(Ipp8u* pMD, int mdLen, IppsDAARijndael192State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael192MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                                   const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                                   Ipp8u* pMD, int mdLen))
/* Rijndael256 based DAA Primitives */
IPPAPI(IppStatus, ippsDAARijndael256GetSize,(int* pSize))
IPPAPI(IppStatus, ippsDAARijndael256Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsDAARijndael256State* pCtx))

IPPAPI(IppStatus, ippsDAARijndael256Update,(const Ipp8u* pSrc, int len, IppsDAARijndael256State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael256Final,(Ipp8u* pMD, int mdLen, IppsDAARijndael256State* pCtx))
IPPAPI(IppStatus, ippsDAARijndael256MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                                   const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                                   Ipp8u* pMD, int mdLen))
/* Rijndael128 based MAC Primitives */
IPPAPI(IppStatus, ippsXCBCRijndael128GetSize,(Ipp32u* pSize))
IPPAPI(IppStatus, ippsXCBCRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                           IppsXCBCRijndael128State* pState))


IPPAPI(IppStatus, ippsXCBCRijndael128Update,(const Ipp8u* pSrc, Ipp32u len, IppsXCBCRijndael128State* pState))
IPPAPI(IppStatus, ippsXCBCRijndael128GetTag,(Ipp8u* pTag, Ipp32u tagLen, const IppsXCBCRijndael128State* pState))
IPPAPI(IppStatus, ippsXCBCRijndael128Final,(Ipp8u* pTag, Ipp32u tagLen, IppsXCBCRijndael128State* pState))
IPPAPI(IppStatus, ippsXCBCRijndael128MessageTag,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                                 Ipp8u* pTag, Ipp32u tagLen))

/*
// CMAC
*/

#if !defined( _OWN_BLDPCS )

typedef struct _cpCMACRijndael128 IppsCMACRijndael128State;

#endif /* _OWN_BLDPCS */

IPPAPI(IppStatus, ippsCMACRijndael128GetSize,(int* pSize))
IPPAPI(IppStatus, ippsCMACRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsCMACRijndael128State* pState))
IPPAPI(IppStatus, ippsCMACSafeRijndael128Init,(const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                          IppsCMACRijndael128State* pState))

IPPAPI(IppStatus, ippsCMACRijndael128Update,(const Ipp8u* pSrc, int len, IppsCMACRijndael128State* pState))
IPPAPI(IppStatus, ippsCMACRijndael128Final,(Ipp8u* pMD, int mdLen, IppsCMACRijndael128State* pState))
IPPAPI(IppStatus, ippsCMACRijndael128MessageDigest,(const Ipp8u* pMsg, int msgLen,
                                                    const Ipp8u* pKey, IppsRijndaelKeyLength keyLen,
                                                    Ipp8u* pMD, int mdLen))


/*
// Big Numbers
*/
#if !defined( _OWN_BLDPCS )

typedef enum {
   IppsBigNumNEG=0,
   IppsBigNumPOS=1
} IppsBigNumSGN;

typedef enum {
   IppsBinaryMethod=0,
   IppsSlidingWindows=1
} IppsExpMethod;

typedef struct _cpBigNum      IppsBigNumState;
typedef struct _cpMontgomery  IppsMontState;
typedef struct _cpPRNG        IppsPRNGState;
typedef struct _cpPrime       IppsPrimeState;

/*  External Bit Supplier */
typedef IppStatus (__STDCALL *IppBitSupplier)(Ipp32u* pRand, int nBits, void* pEbsParams);

#endif /* _OWN_BLDPCS */

#define IS_ZERO 0
#define GREATER_THAN_ZERO 1
#define LESS_THAN_ZERO 2
#define IS_PRIME 3
#define IS_COMPOSITE 4
#define IS_VALID_KEY 5
#define IS_INVALID_KEY 6
#define IS_INCOMPLETED_KEY 7

/* Unsigned BigNum Operations */
IPPAPI(IppStatus, ippsSetOctString_BNU,(const Ipp8u* pStr, int strLen,
                                        Ipp32u* pBNU, int *pBNUsize))
IPPAPI(IppStatus, ippsGetOctString_BNU,(const Ipp32u* pBNU, int bnuSize,
                                        Ipp8u* pStr, int strLen))

IPPAPI(IppStatus, ippsAdd_BNU,(const Ipp32u* pA, const Ipp32u* pB,
                               Ipp32u* pR, int n, Ipp32u *carry))
IPPAPI(IppStatus, ippsSub_BNU,(const Ipp32u* pA, const Ipp32u* pB,
                               Ipp32u* pR, int n, Ipp32u *carry))

IPPAPI(IppStatus, ippsMulOne_BNU,  (const Ipp32u* pA, Ipp32u* pR, int n,
                                    Ipp32u w, Ipp32u *carry))
IPPAPI(IppStatus, ippsMACOne_BNU_I,(const Ipp32u* pA, Ipp32u* pR, int n,
                                    Ipp32u w, Ipp32u *carry))

IPPAPI(IppStatus, ippsMul_BNU4,(const Ipp32u* pA, const Ipp32u* pB, Ipp32u* pR))
IPPAPI(IppStatus, ippsMul_BNU8,(const Ipp32u* pA, const Ipp32u* pB, Ipp32u* pR))
IPPAPI(IppStatus, ippsSqr_BNU4,(const Ipp32u* pA, Ipp32u* pR))
IPPAPI(IppStatus, ippsSqr_BNU8,(const Ipp32u* pA, Ipp32u* pR))

IPPAPI(IppStatus, ippsDiv_64u32u,(Ipp64u a, Ipp32u b, Ipp32u* pQ, Ipp32u* pR))
IPPAPI(IppStatus, ippsSqr_32u64u,(const Ipp32u* pSrc, int n, Ipp64u* pDst))

/* Signed BigNum Operations */
IPPAPI(IppStatus, ippsBigNumGetSize,(int length, int* pSize))
IPPAPI(IppStatus, ippsBigNumInit,(int length, IppsBigNumState* pBN))

IPPAPI(IppStatus, ippsCmpZero_BN,(const IppsBigNumState* pBN, Ipp32u* pResult))
IPPAPI(IppStatus, ippsCmp_BN,(const IppsBigNumState* pA, const IppsBigNumState* pB, Ipp32u* pResult))

IPPAPI(IppStatus, ippsGetSize_BN,(const IppsBigNumState* pBN, int* pSize))
IPPAPI(IppStatus, ippsSet_BN,(IppsBigNumSGN sgn,
                              int length, const Ipp32u* pData,
                              IppsBigNumState* pBN))
IPPAPI(IppStatus, ippsGet_BN,(IppsBigNumSGN* pSgn,
                              int* pLength, Ipp32u* pData,
                              const IppsBigNumState* pBN))
IPPAPI(IppStatus, ippsRef_BN,(IppsBigNumSGN *sgn, int *bitSize, Ipp32u** const ppData,
                              const IppsBigNumState *x))
IPPAPI(IppStatus, ippsExtGet_BN,(IppsBigNumSGN* pSgn,
                              int* pBitSize, Ipp32u* pData,
                              const IppsBigNumState* pBN))

IPPAPI(IppStatus, ippsAdd_BN,   (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsSub_BN,   (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsMul_BN,   (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsMAC_BN_I, (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsDiv_BN,   (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pQ, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsMod_BN,   (IppsBigNumState* pA, IppsBigNumState* pM, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsGcd_BN,   (IppsBigNumState* pA, IppsBigNumState* pB, IppsBigNumState* pGCD))
IPPAPI(IppStatus, ippsModInv_BN,(IppsBigNumState* pA, IppsBigNumState* pM, IppsBigNumState* pInv))

IPPAPI(IppStatus, ippsSetOctString_BN,(const Ipp8u* pStr, int strLen, IppsBigNumState* pBN))
IPPAPI(IppStatus, ippsGetOctString_BN,(Ipp8u* pStr, int strLen, const IppsBigNumState* pBN))

/* Montgomery Operations */
IPPAPI(IppStatus, ippsMontGetSize,(IppsExpMethod method, int length, int* pSize))
IPPAPI(IppStatus, ippsMontInit,(IppsExpMethod method, int length, IppsMontState* pCtx))

IPPAPI(IppStatus, ippsMontSet,(const Ipp32u* pModulo, int size, IppsMontState* pCtx))
IPPAPI(IppStatus, ippsMontGet,(Ipp32u* pModulo, int* pSize, const IppsMontState* pCtx))

IPPAPI(IppStatus, ippsMontForm,(const IppsBigNumState* pA, IppsMontState* pCtx, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsMontMul, (const IppsBigNumState* pA, const IppsBigNumState* pB, IppsMontState* m, IppsBigNumState* pR))
IPPAPI(IppStatus, ippsMontExp, (const IppsBigNumState* pA, const IppsBigNumState* pE, IppsMontState* m, IppsBigNumState* pR))

/* Pseudo-Random Number Generation */
IPPAPI(IppStatus, ippsPRNGGetSize,(int* pSize))
IPPAPI(IppStatus, ippsPRNGInit,   (int seedBits, IppsPRNGState* pCtx))

IPPAPI(IppStatus, ippsPRNGSetModulus,(const IppsBigNumState* pMod, IppsPRNGState* pCtx))
IPPAPI(IppStatus, ippsPRNGSetH0,     (const IppsBigNumState* pH0,  IppsPRNGState* pCtx))
IPPAPI(IppStatus, ippsPRNGSetAugment,(const IppsBigNumState* pAug, IppsPRNGState* pCtx))
IPPAPI(IppStatus, ippsPRNGSetSeed,   (const IppsBigNumState* pSeed,IppsPRNGState* pCtx))
IPPAPI(IppStatus, ippsPRNGGetSeed,   (const IppsPRNGState* pCtx,IppsBigNumState* pSeed))

IPPAPI(IppStatus, ippsPRNGen,   (Ipp32u* pRand, int nBits, void* pCtx))
IPPAPI(IppStatus, ippsPRNGen_BN,(IppsBigNumState* pRand, int nBits, void* pCtx))

/* Probable Prime Number Generation */
IPPAPI(IppStatus, ippsPrimeGetSize,(int nMaxBits, int* pSize))
IPPAPI(IppStatus, ippsPrimeInit,   (int nMaxBits, IppsPrimeState* pCtx))

IPPAPI(IppStatus, ippsPrimeGen, (int nBits, int nTrials, IppsPrimeState* pCtx,
                                 IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsPrimeTest,(int nTrials, Ipp32u* pResult, IppsPrimeState* pCtx,
                                 IppBitSupplier rndFunc, void* pRndParam))

IPPAPI(IppStatus, ippsPrimeGet,   (Ipp32u* pPrime, int* pLen, const IppsPrimeState* pCtx))
IPPAPI(IppStatus, ippsPrimeGet_BN,(IppsBigNumState* pPrime, const IppsPrimeState* pCtx))

IPPAPI(IppStatus, ippsPrimeSet,   (const Ipp32u* pPrime, int nBits, IppsPrimeState* pCtx))
IPPAPI(IppStatus, ippsPrimeSet_BN,(const IppsBigNumState* pPrime, IppsPrimeState* pCtx))


/*
// RSA Cryptography
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpRSA IppsRSAState;
#endif /* _OWN_BLDPCS */

#if !defined( _OWN_BLDPCS )
/* key types */
typedef enum {
   IppRSApublic  = 0x20000000,
   IppRSAprivate = 0x40000000
} IppRSAKeyType;

/* key component's tag */
typedef enum {
   IppRSAkeyN    = 0x01,
   IppRSAkeyE    = 0x02,
   IppRSAkeyD    = 0x04,
   IppRSAkeyP    = 0x08,
   IppRSAkeyQ    = 0x10,
   IppRSAkeyDp   = 0x20,
   IppRSAkeyDq   = 0x40,
   IppRSAkeyQinv = 0x80
} IppRSAKeyTag;
#endif /* _OWN_BLDPCS */

#define MIN_RSA_SIZE (32)
#define MAX_RSA_SIZE (65536)

IPPAPI(IppStatus, ippsRSAGetSize,(int kn, int kp, IppRSAKeyType flag, int* pSize))
IPPAPI(IppStatus, ippsRSAInit,   (int kn, int kp, IppRSAKeyType flag, IppsRSAState* pCtx))

IPPAPI(IppStatus, ippsRSASetKey,(const IppsBigNumState* pKey, IppRSAKeyTag tag, IppsRSAState* pCtx))
IPPAPI(IppStatus, ippsRSAGetKey,(IppsBigNumState* pKey, IppRSAKeyTag tag, const IppsRSAState* pCtx))

IPPAPI(IppStatus, ippsRSAGenerate,(IppsBigNumState* pE,
                                   int kn, int kp, int nTrials, IppsRSAState* pCtx,
                                   IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsRSAValidate,(const IppsBigNumState* pE,
                                   int nTrials, Ipp32u* pResult, IppsRSAState* pCtx,
                                   IppBitSupplier rndFunc, void* pRndParam))

IPPAPI(IppStatus, ippsRSAEncrypt,(const IppsBigNumState* pX, IppsBigNumState* pY, IppsRSAState* pCtx))
IPPAPI(IppStatus, ippsRSADecrypt,(const IppsBigNumState* pX, IppsBigNumState* pY, IppsRSAState* pCtx))

/* RSA schemes: RSAES-OAEP */
IPPAPI(IppStatus, ippsRSAOAEPEncrypt,(const Ipp8u* pSrc, int srcLen,
                                      const Ipp8u* pLabel, int labLen,
                                      const Ipp8u* pSeed,
                                            Ipp8u* pDst, IppsRSAState* pRSA,
                                      IppHASH hashFun, int hashLen,
                                      IppMGF mgfFunc))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_SHA1,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_SHA224,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_SHA256,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_SHA384,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_SHA512,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPEncrypt_MD5,(const Ipp8u* pSrc, int srcLen,
                                       const Ipp8u* pLabel, int labLen,
                                       const Ipp8u* pSeed,
                                             Ipp8u* pDst, IppsRSAState* pRSA))

IPPAPI(IppStatus, ippsRSAOAEPDecrypt,(const Ipp8u* pSrc,
                                      const Ipp8u* pLabel, int labLen,
                                            Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA,
                                      IppHASH hashFun, int hashLen,
                                      IppMGF mgfFunc))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_SHA1,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_SHA224,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_SHA256,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_SHA384,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_SHA512,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSAOAEPDecrypt_MD5,(const Ipp8u* pSrc,
                                       const Ipp8u* pLabel, int labLen,
                                             Ipp8u* pDst, int* pDstLen, IppsRSAState* pRSA))

/* RSA schemes: RSASSA-PKCS_v1_5 */
IPPAPI(IppStatus, ippsRSAEncrypt_PKCSv15,(const Ipp8u* pSrc, Ipp32u srcLen,
                                          const Ipp8u* pRandPS,
                                                Ipp8u* pDst, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSADecrypt_PKCSv15,(const Ipp8u* pSrc,
                                                Ipp8u* pDst, Ipp32u* pDstLen, IppsRSAState* pRSA))

/* RSA schemes: RSASSA-PSS */
IPPAPI(IppStatus, ippsRSASSASign,(const Ipp8u* pHMsg, int hashLen,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA,
                                  IppHASH hashFun, IppMGF mgfFun))
IPPAPI(IppStatus, ippsRSASSASign_SHA1,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA224,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA256,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA384,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA512,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_MD5,(const Ipp8u* pHMsg,
                                  const Ipp8u* pSalt, int saltLen,
                                        Ipp8u* pSign, IppsRSAState* pRSA))

IPPAPI(IppStatus, ippsRSASSAVerify,(const Ipp8u* pHMsg, int hashLen,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA,
                                    IppHASH hashFun, IppMGF mgfFun))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA1,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA224,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA256,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA384,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA512,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_MD5,(const Ipp8u* pHMsg,
                                    const Ipp8u* pSign,
                                    IppBool* pIsValid, IppsRSAState* pRSA))

/* RSA schemes: RSASSA-PSS-PKCS-v1_5 */
IPPAPI(IppStatus, ippsRSASSASign_SHA1_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                               Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA224_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA256_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA384_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_SHA512_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 Ipp8u* pSign, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSASign_MD5_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                              Ipp8u* pSign, IppsRSAState* pRSA))

IPPAPI(IppStatus, ippsRSASSAVerify_SHA1_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                 const Ipp8u* pSign,
                                                 IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA224_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                   const Ipp8u* pSign,
                                                   IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA256_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                   const Ipp8u* pSign,
                                                   IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA384_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                   const Ipp8u* pSign,
                                                   IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_SHA512_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                   const Ipp8u* pSign,
                                                   IppBool* pIsValid, IppsRSAState* pRSA))
IPPAPI(IppStatus, ippsRSASSAVerify_MD5_PKCSv15,(const Ipp8u* pMsg, Ipp32u msgLen,
                                                const Ipp8u* pSign,
                                                IppBool* pIsValid, IppsRSAState* pRSA))

/*
// DL Cryptography
*/
#define MIN_DLP_BITSIZE      (512)
#define MIN_DLP_BITSIZER     (160)

#define MIN_DLPDH_BITSIZE    (512)
#define MIN_DLPDH_BITSIZER   (160)
#define DEF_DLPDH_BITSIZER   (160)

#define MIN_DLPDSA_BITSIZE   (512)
#define MAX_DLPDSA_BITSIZE  (1024)
#define MIN_DLPDSA_BITSIZER  (160)
#define DEF_DLPDSA_BITSIZER  (160)
#define MAX_DLPDSA_BITSIZER  (160)
#define MIN_DLPDSA_SEEDSIZE  (160)

#if !defined( _OWN_BLDPCS )
typedef struct _cpDLP IppsDLPState;

/* domain parameter tags */
typedef enum {
   IppDLPkeyP    = 0x01,
   IppDLPkeyR    = 0x02,
   IppDLPkeyG    = 0x04
} IppDLPKeyTag;

typedef enum {
   ippDLValid,                /* validation pass successfully  */

   ippDLBaseIsEven,           /* !(P is odd)                   */
   ippDLOrderIsEven,          /* !(R is odd)                   */
   ippDLInvalidBaseRange,     /* !(2^(L-1) < P < 2^L)          */
   ippDLInvalidOrderRange,    /* !(2^(M-1) < R < 2^M)          */
   ippDLCompositeBase,
   ippDLCompositeOrder,
   ippDLInvalidCofactor,      /* !( R|(P-1) )                  */
   ippDLInvalidGenerator,     /* !( G^R == 1 (mod P) )         */
                              /* !(1 < G < (P-1))              */
   ippDLInvalidPrivateKey,    /* !(1 < private < (R-1))        */
   ippDLInvalidPublicKey,     /* !(1 < public  <=(P-1))        */
   ippDLInvalidKeyPair,       /* !(G^private == public         */

   ippDLInvalidSignature       /* invalid signature             */
} IppDLResult;
#endif /* _OWN_BLDPCS */


IPPAPI( const char*, ippsDLGetResultString, (IppDLResult code))

/* Initialization */
IPPAPI(IppStatus, ippsDLPGetSize,(int bitSizeP, int bitSizeR, int* pSize))
IPPAPI(IppStatus, ippsDLPInit,   (int bitSizeP, int bitSizeR, IppsDLPState* pCtx))

/* Set Up and Retrieve Domain Parameters */
IPPAPI(IppStatus, ippsDLPSet,(const IppsBigNumState* pP,
                              const IppsBigNumState* pR,
                              const IppsBigNumState* pG,
                              IppsDLPState* pCtx))
IPPAPI(IppStatus, ippsDLPGet,(IppsBigNumState* pP,
                              IppsBigNumState* pR,
                              IppsBigNumState* pG,
                              IppsDLPState* pCtx))
IPPAPI(IppStatus, ippsDLPSetDP,(const IppsBigNumState* pDP, IppDLPKeyTag tag, IppsDLPState* pCtx))
IPPAPI(IppStatus, ippsDLPGetDP,(IppsBigNumState* pDP, IppDLPKeyTag tag, const IppsDLPState* pCtx))

/* Key Generation, Validation and Set Up */
IPPAPI(IppStatus, ippsDLPGenKeyPair,(IppsBigNumState* pPrvKey, IppsBigNumState* pPubKey,
                                     IppsDLPState* pCtx,
                                     IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsDLPPublicKey, (const IppsBigNumState* pPrvKey,
                                     IppsBigNumState* pPubKey,
                                     IppsDLPState* pCtx))
IPPAPI(IppStatus, ippsDLPValidateKeyPair,(const IppsBigNumState* pPrvKey,
                                     const IppsBigNumState* pPubKey,
                                     IppDLResult* pResult,
                                     IppsDLPState* pCtx))

IPPAPI(IppStatus, ippsDLPSetKeyPair,(const IppsBigNumState* pPrvKey,
                                     const IppsBigNumState* pPubKey,
                                     IppsDLPState* pCtx))

/* Singing/Verifying (DSA version) */
IPPAPI(IppStatus, ippsDLPSignDSA,  (const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pPrvKey,
                                    IppsBigNumState* pSignR, IppsBigNumState* pSignS,
                                    IppsDLPState* pCtx))
IPPAPI(IppStatus, ippsDLPVerifyDSA,(const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pSignR, const IppsBigNumState* pSignS,
                                    IppDLResult* pResult,
                                    IppsDLPState* pCtx))

/* Shared Secret Element (DH version) */
IPPAPI(IppStatus, ippsDLPSharedSecretDH,(const IppsBigNumState* pPrvKeyA,
                                         const IppsBigNumState* pPubKeyB,
                                         IppsBigNumState* pShare,
                                         IppsDLPState* pCtx))

/* DSA's parameter Generation and Validation */
IPPAPI(IppStatus, ippsDLPGenerateDSA,(const IppsBigNumState* pSeedIn,
                                      int nTrials, IppsDLPState* pCtx,
                                      IppsBigNumState* pSeedOut, int* pCounter,
                                      IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsDLPValidateDSA,(int nTrials, IppDLResult* pResult, IppsDLPState* pCtx,
                                      IppBitSupplier rndFunc, void* pRndParam))

/* DH parameter's Generation and Validation */
IPPAPI(IppStatus, ippsDLPGenerateDH,(const IppsBigNumState* pSeedIn,
                                     int nTrials, IppsDLPState* pCtx,
                                     IppsBigNumState* pSeedOut, int* pCounter,
                                     IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsDLPValidateDH,(int nTrials, IppDLResult* pResult, IppsDLPState* pCtx,
                                     IppBitSupplier rndFunc, void* pRndParam))


/*
// EC (prime) Cryptography
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpECCP      IppsECCPState;
typedef struct _cpECCB      IppsECCBState;
typedef struct _cpECCPPoint IppsECCPPointState;
typedef struct _cpECCBPoint IppsECCBPointState;

/* operation result */
typedef enum {
   ippECValid,             /* validation pass successfully     */

   ippECCompositeBase,     /* field based on composite         */
   ippECComplicatedBase,   /* number of non-zero terms in the polynomial (> PRIME_ARR_MAX) */
   ippECIsZeroDiscriminant,/* zero discriminant */
   ippECCompositeOrder,    /* composite order of base point    */
   ippECInvalidOrder,      /* invalid base point order         */
   ippECIsWeakMOV,         /* weak Meneze-Okamoto-Vanstone  reduction attack */
   ippECIsWeakSSSA,        /* weak Semaev-Smart,Satoh-Araki reduction attack */
   ippECIsSupersingular,   /* supersingular curve */

   ippECInvalidPrivateKey, /* !(0 < Private < order) */
   ippECInvalidPublicKey,  /* (order*PublicKey != Infinity)    */
   ippECInvalidKeyPair,    /* (Private*BasePoint != PublicKey) */

   ippECPointOutOfGroup,   /* out of group (order*P != Infinity)  */
   ippECPointIsAtInfinite, /* point (P=(Px,Py)) at Infinity  */
   ippECPointIsNotValid,   /* point (P=(Px,Py)) out-of EC    */

   ippECPointIsEqual,      /* compared points are equal     */
   ippECPointIsNotEqual,   /* compared points are different  */

   ippECInvalidSignature   /* invalid signature */
} IppECResult;

/* domain parameter set/get flags */
typedef enum {
   IppECCArbitrary = 0x00000,       /* arbitrary ECC */

   IppECCPStd      = 0x10000,       /* random (recommended) EC over FG(p): */
   IppECCPStd112r1 = IppECCPStd,    /* secp112r1 request */
   IppECCPStd112r2 = IppECCPStd+1,  /* secp112r2 request */
   IppECCPStd128r1 = IppECCPStd+2,  /* secp128r1 request */
   IppECCPStd128r2 = IppECCPStd+3,  /* secp128r2 request */
   IppECCPStd160r1 = IppECCPStd+4,  /* secp160r1 request */
   IppECCPStd160r2 = IppECCPStd+5,  /* secp160r2 request */
   IppECCPStd192r1 = IppECCPStd+6,  /* secp192r1 request */
   IppECCPStd224r1 = IppECCPStd+7,  /* secp224r1 request */
   IppECCPStd256r1 = IppECCPStd+8,  /* secp256r1 request */
   IppECCPStd384r1 = IppECCPStd+9,  /* secp384r1 request */
   IppECCPStd521r1 = IppECCPStd+10, /* secp521r1 request */

   IppECCBStd      = 0x20000,       /* random (recommended) EC over FG(2^m): */
   IppECCBStd113r1 = IppECCBStd,    /* sect113r1 request */
   IppECCBStd113r2 = IppECCBStd+1,  /* sect113r2 request */
   IppECCBStd131r1 = IppECCBStd+2,  /* sect131r1 request */
   IppECCBStd131r2 = IppECCBStd+3,  /* sect131r2 request */
   IppECCBStd163r1 = IppECCBStd+4,  /* sect163r1 request */
   IppECCBStd163r2 = IppECCBStd+5,  /* sect163r2 request */
   IppECCBStd193r1 = IppECCBStd+6,  /* sect193r1 request */
   IppECCBStd193r2 = IppECCBStd+7,  /* sect193r2 request */
   IppECCBStd233r1 = IppECCBStd+8,  /* sect233r1 request */
   IppECCBStd283r1 = IppECCBStd+9,  /* sect283r1 request */
   IppECCBStd409r1 = IppECCBStd+10, /* sect409r1 request */
   IppECCBStd571r1 = IppECCBStd+11, /* sect571r1 request */

   IppECCKStd      = 0x40000,       /* Koblitz (recommended) EC over FG(2^m): */
   IppECCBStd163k1 = IppECCKStd,    /* Koblitz 163 request */
   IppECCBStd233k1 = IppECCKStd+1,  /* Koblitz 233 request */
   IppECCBStd239k1 = IppECCKStd+2,  /* Koblitz 239 request */
   IppECCBStd283k1 = IppECCKStd+3,  /* Koblitz 283 request */
   IppECCBStd409k1 = IppECCKStd+4,  /* Koblitz 409 request */
   IppECCBStd571k1 = IppECCKStd+5   /* Koblitz 571 request */
} IppECCType;
#endif /* _OWN_BLDPCS */

IPPAPI( const char*, ippsECCGetResultString, (IppECResult code))

/* Initialization */
IPPAPI(IppStatus, ippsECCPGetSize,(int feBitSize, int* pSize))
IPPAPI(IppStatus, ippsECCBGetSize,(int feBitSize, int* pSize))

IPPAPI(IppStatus, ippsECCPInit,(int feBitSize, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBInit,(int feBitSize, IppsECCBState* pECC))

/*  Set Up and Retrieve Domain Parameters */
IPPAPI(IppStatus, ippsECCPSet,(const IppsBigNumState* pPrime,
                               const IppsBigNumState* pA, const IppsBigNumState* pB,
                               const IppsBigNumState* pGX,const IppsBigNumState* pGY,const IppsBigNumState* pOrder,
                               int cofactor,
                               IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSet,(const IppsBigNumState* pPrime,
                               const IppsBigNumState* pA, const IppsBigNumState* pB,
                               const IppsBigNumState* pGX,const IppsBigNumState* pGY,const IppsBigNumState* pOrder,
                               int cofactor,
                               IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPSetStd,(IppECCType flag, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSetStd,(IppECCType flag, IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPGet,(IppsBigNumState* pPrime,
                               IppsBigNumState* pA, IppsBigNumState* pB,
                               IppsBigNumState* pGX,IppsBigNumState* pGY,IppsBigNumState* pOrder,
                               int* cofactor,
                               IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBGet,(IppsBigNumState* pPrime,
                               IppsBigNumState* pA, IppsBigNumState* pB,
                               IppsBigNumState* pGX,IppsBigNumState* pGY,IppsBigNumState* pOrder,
                               int* cofactor,
                               IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPGetOrderBitSize,(int* pBitSize, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBGetOrderBitSize,(int* pBitSize, IppsECCBState* pECC))

/* ECC Validation */
IPPAPI(IppStatus, ippsECCPValidate,(int nTrials, IppECResult* pResult, IppsECCPState* pECC,
                                    IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsECCBValidate,(int nTrials, IppECResult* pResult, IppsECCBState* pECC,
                                    IppBitSupplier rndFunc, void* pRndParam))

/* EC Point Initialization */
IPPAPI(IppStatus, ippsECCPPointGetSize,(int feBitSize, int* pSize))
IPPAPI(IppStatus, ippsECCBPointGetSize,(int feBitSize, int* pSize))

IPPAPI(IppStatus, ippsECCPPointInit,(int feBitSize, IppsECCPPointState* pPoint))
IPPAPI(IppStatus, ippsECCBPointInit,(int feBitSize, IppsECCBPointState* pPoint))

/* Set Up and Retrieve EC Point */
IPPAPI(IppStatus, ippsECCPSetPoint,(const IppsBigNumState* pX, const IppsBigNumState* pY,
                                    IppsECCPPointState* pPoint, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSetPoint,(const IppsBigNumState* pX, const IppsBigNumState* pY,
                                    IppsECCBPointState* pPoint, IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPSetPointAtInfinity,(IppsECCPPointState* pPoint, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSetPointAtInfinity,(IppsECCBPointState* pPoint, IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPGetPoint,(IppsBigNumState* pX, IppsBigNumState* pY,
                                    const IppsECCPPointState* pPoint, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBGetPoint,(IppsBigNumState* pX, IppsBigNumState* pY,
                                    const IppsECCBPointState* pPoint, IppsECCBState* pECC))

/* Test EC Point (return result) */
IPPAPI(IppStatus, ippsECCPCheckPoint,(const IppsECCPPointState* pP,
                                      IppECResult* pResult, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBCheckPoint,(const IppsECCBPointState* pP,
                                      IppECResult* pResult, IppsECCBState* pECC))

/* EC Point Operations */
IPPAPI(IppStatus, ippsECCPComparePoint,(const IppsECCPPointState* pP, const IppsECCPPointState* pQ,
                                        IppECResult* pResult, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBComparePoint,(const IppsECCBPointState* pP, const IppsECCBPointState* pQ,
                                        IppECResult* pResult, const IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPNegativePoint,(const IppsECCPPointState* pP,
                                         IppsECCPPointState* pR, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBNegativePoint,(const IppsECCBPointState* pP,
                                         IppsECCBPointState* pR, IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPAddPoint,(const IppsECCPPointState* pP, const IppsECCPPointState* pQ,
                                    IppsECCPPointState* pR, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBAddPoint,(const IppsECCBPointState* pP, const IppsECCBPointState* pQ,
                                    IppsECCBPointState* pR, IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPMulPointScalar,(const IppsECCPPointState* pP, const IppsBigNumState* pK,
                                          IppsECCPPointState* pR, IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBMulPointScalar,(const IppsECCBPointState* pP, const IppsBigNumState* pK,
                                          IppsECCBPointState* pR, IppsECCBState* pECC))

/* Set Up, Validation and Key Generation */
IPPAPI(IppStatus, ippsECCPGenKeyPair,(IppsBigNumState* pPrivate, IppsECCPPointState* pPublic,
                                      IppsECCPState* pECC,
                                      IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsECCBGenKeyPair,(IppsBigNumState* pPrivate, IppsECCBPointState* pPublic,
                                      IppsECCBState* pECC,
                                      IppBitSupplier rndFunc, void* pRndParam))

IPPAPI(IppStatus, ippsECCPPublicKey,(const IppsBigNumState* pPrivate,
                                     IppsECCPPointState* pPublic,
                                     IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBPublicKey,(const IppsBigNumState* pPrivate,
                                     IppsECCBPointState* pPublic,
                                     IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPValidateKeyPair,(const IppsBigNumState* pPrivate, const IppsECCPPointState* pPublic,
                                           IppECResult* pResult,
                                           IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBValidateKeyPair,(const IppsBigNumState* pPrivate, const IppsECCBPointState* pPublic,
                                           IppECResult* pResult,
                                           IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPSetKeyPair,(const IppsBigNumState* pPrivate, const IppsECCPPointState* pPublic,
                                      IppBool regular,
                                      IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSetKeyPair,(const IppsBigNumState* pPrivate, const IppsECCBPointState* pPublic,
                                      IppBool regular,
                                      IppsECCBState* pECC))

/* Shared Secret Element (DH version) */
IPPAPI(IppStatus, ippsECCPSharedSecretDH,(const IppsBigNumState* pPrivateA,
                                          const IppsECCPPointState* pPublicB,
                                          IppsBigNumState* pShare,
                                          IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSharedSecretDH,(const IppsBigNumState* pPrivateA,
                                          const IppsECCBPointState* pPublicB,
                                          IppsBigNumState* pShare,
                                          IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPSharedSecretDHC,(const IppsBigNumState* pPrivateA,
                                           const IppsECCPPointState* pPublicB,
                                           IppsBigNumState* pShare,
                                           IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSharedSecretDHC,(const IppsBigNumState* pPrivateA,
                                           const IppsECCBPointState* pPublicB,
                                           IppsBigNumState* pShare,
                                           IppsECCBState* pECC))

/* Singing/Verifying (DSA version) */
IPPAPI(IppStatus, ippsECCPSignDSA,  (const IppsBigNumState* pMsgDigest,
                                     const IppsBigNumState* pPrivate,
                                     IppsBigNumState* pSignX, IppsBigNumState* pSignY,
                                     IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSignDSA,  (const IppsBigNumState* pMsgDigest,
                                     const IppsBigNumState* pPrivate,
                                     IppsBigNumState* pSignX, IppsBigNumState* pSignY,
                                     IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPVerifyDSA,(const IppsBigNumState* pMsgDigest,
                                     const IppsBigNumState* pSignX, const IppsBigNumState* pSignY,
                                     IppECResult* pResult,
                                     IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBVerifyDSA,(const IppsBigNumState* pMsgDigest,
                                     const IppsBigNumState* pSignX, const IppsBigNumState* pSignY,
                                     IppECResult* pResult,
                                     IppsECCBState* pECC))

/* Singing/Verifying (NR version) */
IPPAPI(IppStatus, ippsECCPSignNR,  (const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pPrivate,
                                    IppsBigNumState* pSignX, IppsBigNumState* pSignY,
                                    IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBSignNR,  (const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pPrivate,
                                    IppsBigNumState* pSignX, IppsBigNumState* pSignY,
                                    IppsECCBState* pECC))

IPPAPI(IppStatus, ippsECCPVerifyNR,(const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pSignX, const IppsBigNumState* pSignY,
                                    IppECResult* pResult,
                                    IppsECCPState* pECC))
IPPAPI(IppStatus, ippsECCBVerifyNR,(const IppsBigNumState* pMsgDigest,
                                    const IppsBigNumState* pSignX, const IppsBigNumState* pSignY,
                                    IppECResult* pResult,
                                    IppsECCBState* pECC))

////////////////////////////////////////////////////////////////////

/*
// Comparison result of GF elements
*/
#if !defined( _OWN_BLDPCS )
typedef enum {
    IppsElementEQ = 0,
    IppsElementNE = 1,
    IppsElementGT = 2,
    IppsElementLT = 3,
    IppsElementNA = 4
} IppsElementCmpResult;
#endif


/*
// GF(p), p>2, prime
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpGFp IppsGFPState;
typedef struct _cpGFpElement IppsGFPElement;
#endif

IPPAPI(IppStatus, ippsGFPGetSize,(Ipp32u bitSize, Ipp32u* pStateSizeInBytes))
IPPAPI(IppStatus, ippsGFPInit,   (IppsGFPState* pGFpCtx, const Ipp32u* pPime, Ipp32u bitSize))

IPPAPI(IppStatus, ippsGFPGet,       (const IppsGFPState* pGFpCtx, const Ipp32u** ppModulus, Ipp32u* pElementLen))

IPPAPI(IppStatus, ippsGFPElementGetSize,(const IppsGFPState* pGFpCtx, Ipp32u* pElementSize))
IPPAPI(IppStatus, ippsGFPElementInit,   (IppsGFPElement* pR, const Ipp32u* pA, Ipp32u lenA, IppsGFPState* pGFpCtx))

IPPAPI(IppStatus, ippsGFPSetElement,      (const Ipp32u* pA, Ipp32u lenA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPSetElementZero,  (IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPSetElementPower2,(Ipp32u power, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPSetElementRandom,(IppsGFPElement* pR, IppsGFPState* pGFpCtx, IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsGFPCmpElement,(const IppsGFPElement* pA, const IppsGFPElement* pB, IppsElementCmpResult* pResult, const IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPCpyElement,(const IppsGFPElement* pA, IppsGFPElement* pR, const IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPGetElement,(const IppsGFPElement* pA, Ipp32u* pDataA, Ipp32u dataLen, const IppsGFPState* pGFpCtx))

IPPAPI(IppStatus, ippsGFPNeg, (const IppsGFPElement* pA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPInv, (const IppsGFPElement* pA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPSqrt,(const IppsGFPElement* pA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPAdd, (const IppsGFPElement* pA, const IppsGFPElement* pB, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPSub, (const IppsGFPElement* pA, const IppsGFPElement* pB, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPMul, (const IppsGFPElement* pA, const IppsGFPElement* pB, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPExp, (const IppsGFPElement* pA, const IppsGFPElement* pE, IppsGFPElement* pR, IppsGFPState* pGFpCtx))

IPPAPI(IppStatus, ippsGFPMontEncode,(const IppsGFPElement* pA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPMontDecode,(const IppsGFPElement* pA, IppsGFPElement* pR, IppsGFPState* pGFpCtx))

/*
// EC over GF(p), p>2, prime
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpGFPEC      IppsGFPECState;
typedef struct _cpGFPECPoint IppsGFPECPoint;
#endif

IPPAPI(IppStatus, ippsGFPECGetSize,(const IppsGFPState* pGF, Ipp32u* pSizeInBytes))
IPPAPI(IppStatus, ippsGFPECInit,   (IppsGFPECState* pEC,
                     const IppsGFPElement* pA, const IppsGFPElement* pB,
                     const IppsGFPElement* pX, const IppsGFPElement* pY,
                     const Ipp32u* pOrder, Ipp32u opderLen,
                     Ipp32u cofactor,
                     IppsGFPState* pGF))
IPPAPI(IppStatus, ippsGFPECSet,(const IppsGFPElement* pA, const IppsGFPElement* pB,
                     const IppsGFPElement* pX, const IppsGFPElement* pY,
                     const Ipp32u* pOrder, Ipp32u orderLen,
                     Ipp32u cofactor,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECGet,(const IppsGFPECState* pEC,
                     const IppsGFPState** ppGF, Ipp32u* pElementLen,
                     IppsGFPElement* pA, IppsGFPElement* pB,
                     IppsGFPElement* pX, IppsGFPElement* pY,
                     const Ipp32u** ppOrder, Ipp32u* pOrderLen,
                     Ipp32u* pCofactor))
IPPAPI(IppStatus, ippsGFPECVerify,(IppsGFPECState* pEC, IppECResult* pResult))

IPPAPI(IppStatus, ippsGFPECPointGetSize,(const IppsGFPECState* pEC, Ipp32u* pSizeInBytes))
IPPAPI(IppStatus, ippsGFPECPointInit,   (IppsGFPECPoint* pPoint, const IppsGFPElement* pX, const IppsGFPElement* pY,
                     IppsGFPECState* pEC))

IPPAPI(IppStatus, ippsGFPECSetPointAtInfinity,(IppsGFPECPoint* pPoint,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECSetPoint,(const IppsGFPElement* pX, const IppsGFPElement* pY, IppsGFPECPoint* pPoint,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECSetPointRandom,(IppsGFPECPoint* pPoint, IppsGFPECState* pEC,
                     IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsGFPECCpyPoint,(const IppsGFPECPoint* pA, IppsGFPECPoint* pR,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECCmpPoint,(const IppsGFPECPoint* pP, const IppsGFPECPoint* pQ, IppsElementCmpResult* pResult,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECVerifyPoint,(const IppsGFPECPoint* pP, IppECResult* pResult,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECGetPoint,(const IppsGFPECPoint* pPoint, IppsGFPElement* pX, IppsGFPElement* pY,
                     IppsGFPECState* pEC))

IPPAPI(IppStatus, ippsGFPECNegPoint,(const IppsGFPECPoint* pP, IppsGFPECPoint* pR,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECAddPoint,(const IppsGFPECPoint* pP, const IppsGFPECPoint* pQ, IppsGFPECPoint* pR,
                     IppsGFPECState* pEC))
IPPAPI(IppStatus, ippsGFPECMulPointScalar,(const IppsGFPECPoint* pP, const Ipp32u* pN, Ipp32u nLen, IppsGFPECPoint* pR,
                     IppsGFPECState* pEC))


/*
// d-degree extension of GF(p), p>2, prime
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpGFpExtElement IppsGFPXElement;
typedef struct _cpGFpExt IppsGFPXState;
#endif

IPPAPI(IppStatus, ippsGFPXGetSize,(const IppsGFPState* pGroundGF, Ipp32u degree, Ipp32u* pStateSizeInBytes))
IPPAPI(IppStatus, ippsGFPXInit,   (IppsGFPXState* pGFPExtCtx, IppsGFPState* pGroundGF, Ipp32u degree, const Ipp32u* pModulus))

IPPAPI(IppStatus, ippsGFPXGet,       (const IppsGFPXState* pGFPExtCtx,
                                      const IppsGFPState** ppGroundField, Ipp32u* pModulus, Ipp32u* pModulusDegree, Ipp32u* pModulusLen,
                                      Ipp32u* pElementLen))

IPPAPI(IppStatus, ippsGFPXElementGetSize,(const IppsGFPXState* pGFPExtCtx, Ipp32u* pElementSize))
IPPAPI(IppStatus, ippsGFPXElementInit,   (IppsGFPXElement* pR, const Ipp32u* pA, Ipp32u lenA, IppsGFPXState* pGFPExtCtx))

IPPAPI(IppStatus, ippsGFPXSetElement,      (const Ipp32u* pA, Ipp32u lenA, IppsGFPXElement* pR, IppsGFPXState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPXSetElementZero,  (IppsGFPXElement* pR, const IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXSetElementPowerX,(Ipp32u power, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXSetElementRandom,(IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx, IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsGFPXCpyElement,      (const IppsGFPXElement* pA, IppsGFPXElement* pR, const IppsGFPXState* pGFpCtx))
IPPAPI(IppStatus, ippsGFPXCmpElement,      (const IppsGFPXElement* pA, const IppsGFPXElement* pB, IppsElementCmpResult* pResult, const IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXGetElement,      (const IppsGFPXElement* pA, Ipp32u* pDataA, Ipp32u dataLen, const IppsGFPXState* pGFPExtCtx))

IPPAPI(IppStatus, ippsGFPXAdd,    (const IppsGFPXElement* pA, const IppsGFPXElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXAdd_GFP,(const IppsGFPXElement* pA, const IppsGFPElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXSub,    (const IppsGFPXElement* pA, const IppsGFPXElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXSub_GFP,(const IppsGFPXElement* pA, const IppsGFPElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXMul,    (const IppsGFPXElement* pA, const IppsGFPXElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXMul_GFP,(const IppsGFPXElement* pA, const IppsGFPElement* pB, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXNeg,    (const IppsGFPXElement* pA, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXExp,    (const IppsGFPXElement* pA, const Ipp32u* pB, Ipp32u bSize, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXInv,    (const IppsGFPXElement* pA, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))
IPPAPI(IppStatus, ippsGFPXDiv,    (const IppsGFPXElement* pA, const IppsGFPXElement* pB, IppsGFPXElement* pQ, IppsGFPXElement* pR, IppsGFPXState* pGFPExtCtx))

/*
// EC over GF(p^d), p>2, prime
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpGFPXEC      IppsGFPXECState;
typedef struct _cpGFPXECPoint IppsGFPXECPoint;
#endif

IPPAPI(IppStatus, ippsGFPXECGetSize,(const IppsGFPXState* pGF, Ipp32u* pSizeInBytes))
IPPAPI(IppStatus, ippsGFPXECInit,   (IppsGFPXECState* pEC,
                     const IppsGFPXElement* pA, const IppsGFPXElement* pB,
                     const IppsGFPXElement* pX, const IppsGFPXElement* pY,
                     const Ipp32u* pOrder, Ipp32u opderLen,
                     Ipp32u cofactor,
                     IppsGFPXState* pGF))
IPPAPI(IppStatus, ippsGFPXECSet,(const IppsGFPXElement* pA, const IppsGFPXElement* pB,
                     const IppsGFPXElement* pX, const IppsGFPXElement* pY,
                     const Ipp32u* pOrder, Ipp32u orderLen,
                     Ipp32u cofactor,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECGet,(const IppsGFPXECState* pEC,
                     const IppsGFPXState** ppGF, Ipp32u* pElementLen,
                     IppsGFPXElement* pA, IppsGFPXElement* pB,
                     IppsGFPXElement* pX, IppsGFPXElement* pY,
                     const Ipp32u** ppOrder, Ipp32u* pOrderLen,
                     Ipp32u* pCofactor))
IPPAPI(IppStatus, ippsGFPXECVerify,(IppsGFPXECState* pEC, IppECResult* pResult))

IPPAPI(IppStatus, ippsGFPXECPointGetSize,(const IppsGFPXECState* pEC, Ipp32u* pSizeInBytes))
IPPAPI(IppStatus, ippsGFPXECPointInit,   (IppsGFPXECPoint* pPoint,
                                    const IppsGFPXElement* pX, const IppsGFPXElement* pY,
                                          IppsGFPXECState* pEC))

IPPAPI(IppStatus, ippsGFPXECSetPointAtInfinity,(IppsGFPXECPoint* pPoint,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECSetPoint,(const IppsGFPXElement* pX, const IppsGFPXElement* pY, IppsGFPXECPoint* pPoint,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECSetPointRandom,(IppsGFPXECPoint* pPoint, IppsGFPXECState* pEC,
                     IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsGFPXECCpyPoint,(const IppsGFPXECPoint* pA, IppsGFPXECPoint* pR,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECCmpPoint,(const IppsGFPXECPoint* pP, const IppsGFPXECPoint* pQ, IppsElementCmpResult* pResult,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECVerifyPoint,(const IppsGFPXECPoint* pP, IppECResult* pResult,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECGetPoint,(const IppsGFPXECPoint* pPoint, IppsGFPXElement* pX, IppsGFPXElement* pY,
                     IppsGFPXECState* pEC))

IPPAPI(IppStatus, ippsGFPXECNegPoint,(const IppsGFPXECPoint* pP, IppsGFPXECPoint* pR,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECAddPoint,(const IppsGFPXECPoint* pP, const IppsGFPXECPoint* pQ, IppsGFPXECPoint* pR,
                     IppsGFPXECState* pEC))
IPPAPI(IppStatus, ippsGFPXECMulPointScalar,(const IppsGFPXECPoint* pP, const Ipp32u* pN, Ipp32u nLen, IppsGFPXECPoint* pR,
                     IppsGFPXECState* pEC))

/*
// 2-degree extension of GF(p^d), p>2, prime
*/
#if !defined( _OWN_BLDPCS )
typedef struct _cpGFpQExtElement IppsGFPXQElement;
typedef struct _cpGFpQExt IppsGFPXQState;
#endif

IPPAPI(IppStatus, ippsGFPXQGetSize,(const IppsGFPXState* pGFPExtCtx, Ipp32u* pStateSizeInBytes))
IPPAPI(IppStatus, ippsGFPXQInit,   (IppsGFPXQState* pGFPQExtCtx, IppsGFPXState* pGroundField, const Ipp32u* pModulus))

IPPAPI(IppStatus, ippsGFPXQGet,    (const IppsGFPXQState* pGFPQExtCtx,
                                    const IppsGFPXState** ppGroundField, Ipp32u* pModulus, Ipp32u* pModulusDegree, Ipp32u* pModulusLen,
                                    Ipp32u* pElementLen))

IPPAPI(IppStatus, ippsGFPXQElementGetSize, (const IppsGFPXQState* pGFPQExtCtx, Ipp32u* pElementSize))
IPPAPI(IppStatus, ippsGFPXQElementInit,    (IppsGFPXQElement* pR, const Ipp32u* pA, Ipp32u lenA, IppsGFPXQState* pGFPQExtCtx))

IPPAPI(IppStatus, ippsGFPXQSetElement,      (const Ipp32u* pA, Ipp32u lenA, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQSetElementZero,  (IppsGFPXQElement* pR, const IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQSetElementRandom,(IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx, IppBitSupplier rndFunc, void* pRndParam))
IPPAPI(IppStatus, ippsGFPXQSetElementPowerX,(Ipp32u power, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQCpyElement,      (const IppsGFPXQElement* pA, IppsGFPXQElement* pR, const IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQCmpElement,      (const IppsGFPXQElement* pA, const IppsGFPXQElement* pB, IppsElementCmpResult* pResult, const IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQGetElement,      (const IppsGFPXQElement* pA, Ipp32u* pDataA, Ipp32u dataLen, const IppsGFPXQState* pGFPQExtCtx))

IPPAPI(IppStatus, ippsGFPXQAdd, (const IppsGFPXQElement* pA, const IppsGFPXQElement* pB, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQSub, (const IppsGFPXQElement* pA, const IppsGFPXQElement* pB, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQNeg, (const IppsGFPXQElement* pA, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQMul, (const IppsGFPXQElement* pA, const IppsGFPXQElement* pB, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQMul_GFP, (const IppsGFPXQElement* pA, const IppsGFPElement* pB, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQExp, (const IppsGFPXQElement* pA, const Ipp32u* pB, Ipp32u bSize, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))
IPPAPI(IppStatus, ippsGFPXQInv, (const IppsGFPXQElement* pA, IppsGFPXQElement* pR, IppsGFPXQState* pGFPQExtCtx))

/*
// Pairing
*/

#if !defined( _OWN_BLDPCS )
typedef struct _cpPairing  IppsTatePairingDE3State;
#endif

IPPAPI(IppStatus, ippsTatePairingDE3GetSize,(const IppsGFPECState* pG1,
                              const IppsGFPXECState* pG2,
                              Ipp32u* pSizeInBytes))
IPPAPI(IppStatus, ippsTatePairingDE3Init,(IppsTatePairingDE3State* pPairingCtx,
                              const IppsGFPECState* pG1,
                              const IppsGFPXECState* pG2,
                              const IppsGFPXQState* pGT))

IPPAPI(IppStatus, ippsTatePairingDE3Get,(const IppsTatePairingDE3State* pPairingCtx,
                              const IppsGFPECState** ppG1,
                              const IppsGFPXECState** ppG2,
                              const IppsGFPXQState** ppGT))

IPPAPI(IppStatus, ippsTatePairingDE3Apply,(const IppsGFPECPoint* pPointG1,
                              const IppsGFPXECPoint* pPointG2,
                              IppsGFPXQElement* pElementGT,
                              IppsTatePairingDE3State* pPairingCtx))


#ifdef  __cplusplus
}
#endif


#if defined (_IPP_STDCALL_CDECL)
  #undef  _IPP_STDCALL_CDECL
  #define __stdcall __cdecl
#endif


#endif /* __IPPCP_H__ */
