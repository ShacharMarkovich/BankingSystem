/*
  #*************************************************************************
  # INTEL CONFIDENTIAL
  #
  # Copyright (C) 2008-2011 Intel Corporation All Rights Reserved.
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
  #
  # ************************************************************************
*/

/*!\file
  \brief This file defines interface to EC-DSA sign/verify functionality that is required in EPID protocol
*/


#ifndef __ECDSA_CRYPTO_H__
#define __ECDSA_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "epid_constants.h"
#include "epid_errors.h"
#include "epid_types.h"
#include "epid_macros.h"

  /*! 
    @defgroup ECDSASignVerify ECDSA Sign and Verify

    This component provides an implementation of standard ECDSA signature
    and verification functionality as is required by the EPID spec.

    This implementation provides generic sign/verify functionality and 
    is separated from the rest of the EPID implementation primarily because 
    it is standard industry cryptography and could easily be replaced by
    a different implementation.

    The actual ECDSA parameters and pub/private keys are embedded inside
    this component and can easily be replaced if necessary

    @ingroup sampleUtils
    @{
  */

  /*! 
    \brief
    a structure which contains all data items that a EcDsa needs to
    perform its designated tasks
  */
  typedef struct EPIDEcDsa EPIDEcDsa;

  /*! 
    \brief
    Create a structure that can be used for calling "EcDsa" APIs of the
    SIK

    This function must be called before calling any "EcDsa" APIs.  The
    function will allocate memory for the "ctx" and then populate the
    data.  The corresponding "Delete" function must be called to avoid
    memory leaks

    It is assumed that the parameters for the ECDSA curve are the same
    as those used for EPID

    \param[in] epidParameters Points to a structure containing the EPID
    parameter values to use
    \param[out] ctx Returns a pointer to a context value to be used on subsequent API calls

    \retval EPID_SUCCESS
    Context successfully created and ready to be used
    \retval EPID_FAILURE
    Context creation failed
  */
  EPID_RESULT epidEcDsa_create(
      const EPIDParameterCertificateBlob* epidParameters,
      EPIDEcDsa** ctx);

  /*
    \brief Apply an EC-DSA signature to a data buffer of arbitrary size

    Will first create a SHA256 hash of the buffer and then sign the
    hash value

    \param[in] ctx The context to use
    \param[in] signKey Signing key blob to use
    \param[in] buffer Pointer to the data to be signed
    \param[in] bufferLengthBytes Integer specifying the number of
    bytes to sign
    \param[out] signature Pointer to memory which will receive the
    newly created signature

    \retval EPID_SUCCESS
    Signature successfully created and ready to be used
    \retval EPID_FAILURE
    Signature creation failed
  */
  EPID_RESULT epidEcDsa_sign(EPIDEcDsa* ctx,
                             const EPIDSigningKeyBlob* signKey,
                             const unsigned char* buffer,
                             unsigned int bufferLengthBytes,
                             unsigned char* signature);

  /*
    \brief Verify an EC-DSA signature on a data buffer of arbitrary size

    Will first create a SHA256 hash of the buffer and then verify the
    signature on the hash value

    \param[in] ctx The context to use
    \param[in] verifyKey Verification key blob to use
    \param[in] buffer Pointer to the data that was signed
    \param[in] bufferLengthBytes Integer specifying the number of
    bytes that was signed
    \param[in] signature Pointer to memory which contains the
    signature to be verified
    \param[out] isValid Verification result.

    \retval EPID_SUCCESS
    Signature successfully verified
    \retval EPID_FAILURE
    Was unable to complete the verification process
  */
  EPID_RESULT epidEcDsa_verify(EPIDEcDsa* ctx,
                               const EPIDVerificationKeyBlob* verifyKey,
                               const unsigned char* buffer,
                               unsigned int bufferLengthBytes,
                               const unsigned char* signature,
                               int* isValid);


  /*! 
    \brief Destroy a "EcDsa" context and clean up resources

    This function must be called when done calling "EcDsa" APIs.

    \param[in] ctx The context to be deleted

    \retval EPID_SUCCESS
    Context successfully deleted
    \retval EPID_FAILURE
    Context deletion failed, possibly due to invalid ctx value
  */
  EPID_RESULT epidEcDsa_delete(EPIDEcDsa** ctx);

  /*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __ECDSA_CRYPTO_H__
