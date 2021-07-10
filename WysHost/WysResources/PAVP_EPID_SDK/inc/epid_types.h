/*
  # ******************************************************************
  # INTEL CONFIDENTIAL
  #
  # Copyright (C) 2009-2012 Intel Corporation All Rights Reserved.
  #
  # The source code contained or described herein and all documents
  # related to the source code ("Material") are owned by Intel
  # Corporation or its suppliers or licensors. Title to the Material
  # remains with Intel Corporation or its suppliers and licensors. The
  # Material contains trade secrets and proprietary and confidential
  # information of Intel or its suppliers and licensors. The Material
  # is protected by worldwide copyright and trade secret laws and
  # treaty provisions. No part of the Material may be used, copied,
  # reproduced, modified, published, uploaded, posted, transmitted,
  # distributed, or disclosed in any way without Intel's prior express
  # written permission.
  #
  # No license under any patent, copyright, trade secret or other
  # intellectual property right is granted to or conferred upon you by
  # disclosure or delivery of the Materials, either expressly, by
  # implication, inducement, estoppel or otherwise. Any license under
  # such intellectual property rights must be express and approved by
  # Intel in writing.
  # ******************************************************************
*/

/*!
 * \file
 * \brief
 * This file defines structures for the data types used in the EPID
 * SIK
 */

#ifndef EPID_INC_EPID_TYPES_H_
#define EPID_INC_EPID_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "epid_constants.h"

  /*!
    @defgroup EPIDDataTypes EPID Data Types
    C Structure definitions that define data items used in EPID
    protocol specification.  At this time, these structure definitions
    are also used for the internal data representations in the core
    SIK library.  It is not guaranteed that this will be true in
    future versions of the SIK.

    \ingroup commonHeaders

    @{
  */

  // Make sure the C structures align correctly per the blob
  // definitions in the specification
#pragma pack(1)

  /// \brief a structure which contains an element of G1
  typedef struct EPIDG1Element {
    unsigned char x[EPID_NUMBER_SIZE];      ///< prime number
    unsigned char y[EPID_NUMBER_SIZE];      ///< prime number
  } EPIDG1Element;

  /// \brief a structure which contains an element of G2
  typedef struct EPIDG2Element {
    unsigned char x0[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x1[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x2[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char y0[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char y1[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char y2[EPID_NUMBER_SIZE];     ///< prime number
  } EPIDG2Element;

  /// \brief a structure which contains an element of G3
  typedef struct EPIDG3Element {
    unsigned char x[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char y[EPID_NUMBER_SIZE];     ///< prime number
  } EPIDG3Element;

  /// \brief a structure which contains an element of GT
  typedef struct EPIDGTElement {
    unsigned char x0[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x1[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x2[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x3[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x4[EPID_NUMBER_SIZE];     ///< prime number
    unsigned char x5[EPID_NUMBER_SIZE];     ///< prime number
  } EPIDGTElement;

  /// \brief a structure which contains a Non revoked proof
  typedef struct EPIDNRProof {
    EPIDG3Element T;                         ///< element of G3.
    unsigned char c[EPID_NUMBER_SIZE];     ///< 256 bit number
    unsigned char smu[EPID_NUMBER_SIZE];     ///< integer in [0, p'-1].
    unsigned char snu[EPID_NUMBER_SIZE];     ///< integer in [0, p'-1].
  } EPIDNRProof;

  /// \brief a structure which contains all the EPID parameters
  typedef struct EPIDParameterCertificateBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  Parameter Cert
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// prime number for G1 and G2.
    unsigned char p12[EPID_NUMBER_SIZE];
    /// prime number for G1 and G2.
    unsigned char q12[EPID_NUMBER_SIZE];
    /// small integer for G1, also denoted as cofactor.
    unsigned char h1[EPID_SMALL_INT_SIZE];
    /// integer in [0, q-1] for G1 and G2.
    unsigned char a12[EPID_NUMBER_SIZE];
    /// integer in [0, q-1] for G1 and G2.
    unsigned char b12[EPID_NUMBER_SIZE];
    /// Coefficient of an irreducible polynomial, an integer in [0, q-1].
    unsigned char coeff0[EPID_NUMBER_SIZE];
    /// Coefficient of an irreducible polynomial, an integer in [0, q-1].
    unsigned char coeff1[EPID_NUMBER_SIZE];
    /// Coefficient of an irreducible polynomial, an integer in [0, q-1].
    unsigned char coeff2[EPID_NUMBER_SIZE];
    /// quadratic non-residue, an integer in [0, q-1].
    unsigned char qnr[EPID_NUMBER_SIZE];
    /// OrderG2 value
    unsigned char orderg2[EPID_ORDERG2_SIZE];
    /// prime number for G3.
    unsigned char p3[EPID_NUMBER_SIZE];
    /// prime number for G3.
    unsigned char q3[EPID_NUMBER_SIZE];
    /// small integer for G3, also denoted as cofactor.
    unsigned char h3[EPID_SMALL_INT_SIZE];
    /// integer in [0, q-1] for G3.
    unsigned char a3[EPID_NUMBER_SIZE];
    /// integer in [0, q-1] for G3.
    unsigned char b3[EPID_NUMBER_SIZE];
    /// generator of G1.
    EPIDG1Element g1;
    /// generator of G2.
    EPIDG2Element g2;
    /// generator of G3.
    EPIDG3Element g3;
    /// EC-DSA signature.
    unsigned char sig[ECDSA_SIGNATURE_SIZE];
  } EPIDParameterCertificateBlob;

  /// \brief a structure which contains a EPID group certificate
  typedef struct EPIDGroupCertificateBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  group certificate
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Group ID.
    unsigned char gid[EPID_GROUP_ID_SIZE];
    /// G1.exp(g1, u1).
    EPIDG1Element h1;
    /// G1.exp(g1, u2).
    EPIDG1Element h2;
    /// G2.exp(g2, gamma).
    EPIDG2Element  w;
    /// EC-DSA signature.
    unsigned char sig[ECDSA_SIGNATURE_SIZE];
  } EPIDGroupCertificateBlob;

  /// \brief a structure which contains a group private key
  typedef struct EPIDGroupPrivateKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  group private key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Group ID.
    unsigned char gid[EPID_GROUP_ID_SIZE];
    /// random number
    unsigned char u1[EPID_NUMBER_SIZE];
    /// random number
    unsigned char u2[EPID_NUMBER_SIZE];
    /// random number
    unsigned char gamma[EPID_NUMBER_SIZE];
  } EPIDGroupPrivateKeyBlob;

  /// \brief a structure which contains a EPID member private key
  typedef struct EPIDMemberPrivateKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  member private key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Group ID.
    unsigned char gid[EPID_GROUP_ID_SIZE];
    /// G1.exp(g1, t1)
    EPIDG1Element A;
    /// random number
    unsigned char x[EPID_NUMBER_SIZE];
    /// random number
    unsigned char y[EPID_NUMBER_SIZE];
    /// random number
    unsigned char f[EPID_NUMBER_SIZE];
  } EPIDMemberPrivateKeyBlob;

  /// \brief a structure which contains a EPID compressed member private key
  typedef struct EPIDCompressedMemberPrivateKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  member private key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Group ID.
    unsigned char gid[EPID_GROUP_ID_SIZE];
    /// A.x
    unsigned char Ax[EPID_NUMBER_SIZE];
    /// seed
    unsigned char seed[EPID_SEED_SIZE];
  } EPIDCompressedMemberPrivateKeyBlob;

  /// \brief a structure which contains a partial EPID member private key
  typedef struct EPIDPartialMemberPrivateKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  member private key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Group ID.
    unsigned char gid[EPID_GROUP_ID_SIZE];
    /// G1.exp(g1, t1)
    EPIDG1Element A;
    /// random number
    unsigned char x[EPID_NUMBER_SIZE];
    /// random number
    unsigned char y[EPID_NUMBER_SIZE];
  } EPIDPartialMemberPrivateKeyBlob;

  /// \brief a structure which contains a member pre-computation blob
  typedef struct EPIDMemberPreComputationBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  member pre-computation blob
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Part of the Member pre-computation blob
    EPIDGTElement e12;
    /// Part of the Member pre-computation blob
    EPIDGTElement e22;
    /// Part of the Member pre-computation blob
    EPIDGTElement e2w;
    /// Part of the Member pre-computation blob
    EPIDGTElement ea2;
  } EPIDMemberPreComputationBlob;

  /// \brief a structure which contains a verifier pre-computation blob
  typedef struct EPIDVerifierPreComputationBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  verifier pre-computation blob
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// Part of the Verifier pre-computation blob
    EPIDGTElement e12;
    /// Part of the Verifier pre-computation blob
    EPIDGTElement e22;
    /// Part of the Verifier pre-computation blob
    EPIDGTElement e2w;
  } EPIDVerifierPreComputationBlob;

  /// \brief a structure which contains a verification key blob
  typedef struct EPIDVerificationKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  verification key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// element of G3.
    EPIDG3Element c;
    /// prime number for G3.
    unsigned char q3[EPID_NUMBER_SIZE];
    /// integer in [0, q-1] for G3.
    unsigned char a3[EPID_NUMBER_SIZE];
    /// integer in [0, q-1] for G3.
    unsigned char b3[EPID_NUMBER_SIZE];
    /// generator of G3.
    EPIDG3Element g3;
    /// prime number for G3.
    unsigned char p3[EPID_NUMBER_SIZE];
    /// EC-DSA signature.
    unsigned char sig[ECDSA_SIGNATURE_SIZE];
  } EPIDVerificationKeyBlob;

  /// \brief a structure which contains a signing key blob
  typedef struct EPIDSigningKeyBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  signing key
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// 256 bit integer.
    unsigned char d[EPID_NUMBER_SIZE];
  } EPIDSigningKeyBlob;

  /// \brief a structure which contains a EPID signature
  typedef struct EPIDBasicSignatureBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  epid signature
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// element of G3.
    EPIDG3Element B;
    /// element of G3.
    EPIDG3Element K;
    /// element of G1.
    EPIDG1Element T1;
    /// element of G1.
    EPIDG1Element T2;
    /// number
    unsigned char c[EPID_NUMBER_SIZE];
    /// 80 bit number
    unsigned char nd[EPID_ND_SIZE];
    /// number
    unsigned char sx[EPID_NUMBER_SIZE];
    /// number
    unsigned char sy[EPID_NUMBER_SIZE];
    /// 593 bit number (rounded up to 600 bits)
    unsigned char sf[EPID_SF_SIZE];
    /// number
    unsigned char sa[EPID_NUMBER_SIZE];
    /// number
    unsigned char sb[EPID_NUMBER_SIZE];
    /// number
    unsigned char salpha[EPID_NUMBER_SIZE];
    /// number
    unsigned char sbeta[EPID_NUMBER_SIZE];
  } EPIDBasicSignatureBlob;

  /// \brief a structure which contains a pre-computed EPID signature
  typedef struct EPIDPreSignatureBlob {
    /// version number.  First version contains "1".
    unsigned char sver[EPID_SVER_SIZE];
    /// identifier.  epid pre-signature
    unsigned char blobid[EPID_BLOBID_SIZE];
    /// element of G3.
    EPIDG3Element B;
    /// element of G3.
    EPIDG3Element K;
    /// element of G1.
    EPIDG1Element T1;
    /// element of G1.
    EPIDG1Element T2;
    /// number
    unsigned char a[EPID_NUMBER_SIZE];
    /// number
    unsigned char b[EPID_NUMBER_SIZE];
    /// number
    unsigned char rx[EPID_NUMBER_SIZE];
    /// number
    unsigned char ry[EPID_NUMBER_SIZE];
    /// 592 bit number
    unsigned char rf[EPID_RF_SIZE];
    /// number
    unsigned char ra[EPID_NUMBER_SIZE];
    /// number
    unsigned char rb[EPID_NUMBER_SIZE];
    /// number
    unsigned char ralpha[EPID_NUMBER_SIZE];
    /// number
    unsigned char rbeta[EPID_NUMBER_SIZE];
    /// element of G1.
    EPIDG1Element R1;
    /// element of G1.
    EPIDG1Element R2;
    /// element of G3.
    EPIDG3Element R3;
    /// element of GT.
    EPIDGTElement R4;
  } EPIDPreSignatureBlob;

  /*!
    \brief type for seed which is used in performance rekey
  */
  typedef  EPIDG1Element EPIDPrivateKeySeed;

#pragma pack()


  /*@}*/

#ifdef __cplusplus
}
#endif

#endif  // EPID_INC_EPID_TYPES_H_
