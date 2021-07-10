/*
  # ******************************************************************
  # INTEL CONFIDENTIAL
  #
  # Copyright (C) 2009-2011 Intel Corporation All Rights Reserved.
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
 * This file defines the external interface used by "Issuers"
 */

#ifndef EPID_INC_EPID_ISSUER_H_
#define EPID_INC_EPID_ISSUER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "epid_constants.h"
#include "epid_types.h"
#include "epid_errors.h"
#include "epid_prng.h"

  /*!
    @defgroup issuerAPI Issuer
    Interfaces that provide implementation of "Issuer" functionality

    @ingroup coreEPIDComponents
    @{
  */

  /*!
    \brief
    a structure which contains all data items that an Issuer needs to
    perform its designated tasks
  */
  typedef struct EPIDIssuer EPIDIssuer;

  /*!
    \brief
    Create a structure that can be used for calling "Issuer" APIs of
    the SIK

    This function must be called before calling any "Issuer" APIs.
    The function will allocate memory for the "ctx" and then populate
    the data.  The corresponding "Delete" function must be called to
    avoid memory leaks

    It is assumed that the ECDSA signature on the parameters has
    already been checked

    \param[in] epidParameters
    Points to a structure containing the EPID parameter values to use
    \param[in] randFunc
    Function pointer of prng to use
    \param[in] pUserPrngData
    Pointer to optional user data for prng calls
    \param[out] pCtx
    Returns a pointer to a context value to be used on subsequent API
    calls

    \retval EPID_SUCCESS
    Context successfully created and ready to be used
    \retval EPID_FAILURE
    Context creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_BAD_ARGS
    Invalid arguments
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidIssuer_create(EPIDParameterCertificateBlob* epidParameters,
                                EPIDPrngFunc randFunc,
                                void* pUserPrngData,
                                EPIDIssuer** pCtx);

  /*!
    \brief
    Destroy an "Issuer" context and clean up resources

    This function must be called when done calling "Issuer" APIs.

    \param[in] ctx
    The context to be deleted

    \retval EPID_SUCCESS
    Context successfully deleted
    \retval EPID_FAILURE
    Context deletion failed, possibly due to invalid ctx value
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
  */
  EPID_RESULT epidIssuer_delete(EPIDIssuer** ctx);


  /*!
    \brief
    Create a Group Certificate

    This function will create the group key pair and the certificate

    The ECDSA signature for the certificate is NOT created in this
    function.  It is assumed the caller will use other APIs to apply
    that signature

    Caller must allocate memory for group cert and release it when the
    group is no longer needed

    It is assumed that the caller has already confirmed that gid is
    unique

    \param[in] ctx
    The context to use
    \param[in] gid
    Group ID value to use for the new group
    \param[out] groupCert
    Pointer to memory that will receive the newly created group
    certificate
    \param[out] groupPrivateKey
    pointer to memory that will receive the newly created group
    private key

    \retval EPID_SUCCESS
    Group successfully created and ready to be used
    \retval EPID_FAILURE
    Group creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidIssuer_createGroup(EPIDIssuer* ctx,
                                     unsigned char gid[EPID_GROUP_ID_SIZE],
                                     EPIDGroupCertificateBlob* groupCert,
                                     EPIDGroupPrivateKeyBlob* groupPrivateKey);

  /*!
    \brief
    Create one or more private keys

    This function will create one or more private keys Caller must
    allocate memory for the private keys and release it when the keys
    are no longer needed

    It is assumed that memberPrivateKey points to sufficient memory to
    receive all the requested keys

    \param[in] ctx
    The context to use
    \param[in] groupCert
    pointer to a group certificate which contains the gid value to use
    \param[in] groupPrivateKey
    pointer to the group private key that corresponds to the group
    certificate
    \param[in] keyCount
    counter of how many keys to create
    \param[out] memberPrivateKey
    Pointer to memory to receive the private keys
    \param[out] rekeySeed
    Pointer to memory to receive private key seeds, can be null

    \retval EPID_SUCCESS
    Keys successfully created and ready to be used
    \retval EPID_FAILURE
    Keys creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_UNLUCKY
    unable to find suitible random values, pick new random data and
    try again
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidIssuer_createMemberPrivateKeys(
      EPIDIssuer* ctx,
      EPIDGroupCertificateBlob* groupCert,
      EPIDGroupPrivateKeyBlob* groupPrivateKey,
      unsigned int keyCount,
      EPIDMemberPrivateKeyBlob* memberPrivateKey,
      EPIDPrivateKeySeed* rekeySeed);

  /*!
    \brief
    Create one or more compressed private keys

    This function will create one or more compressed private keys
    Caller must allocate memory for the private keys and release it
    when the keys are no longer needed

    It is assumed that compressedMemberPrivateKey points to sufficient
    memory to receive all the requested keys

    \param[in] ctx
    The context to use
    \param[in] groupCert
    pointer to a group certificate which contains the gid value to use
    \param[in] groupPrivateKey
    pointer to the group private key that corresponds to the group
    certificate
    \param[in] seed
    Pointer to array of seed values to use for compressed keys
    \param[in] keyCount
    counter of how many keys to create
    \param[out] compressedMemberPrivateKey
    Pointer to memory to receive the compressed key
    \param[out] rekeySeed
    Pointer to memory to receive rekeySeeds, can be null

    \retval EPID_SUCCESS
    Keys successfully created and ready to be used
    \retval EPID_FAILURE
    Keys creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_UNLUCKY
    unable to find suitable random values, pick new random data and
    try again
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidIssuer_createCompressedMemberPrivateKeys(
      EPIDIssuer* ctx,
      EPIDGroupCertificateBlob* groupCert,
      EPIDGroupPrivateKeyBlob* groupPrivateKey,
      unsigned char* seed,
      unsigned int keyCount,
      EPIDCompressedMemberPrivateKeyBlob* compressedMemberPrivateKey,
      EPIDPrivateKeySeed* rekeySeed);


  /*!
    \brief
    Verify Member Private Key is valid

    This function will verify that a member private key is valid for
    the given parameters and group id.  This is accomplished by
    creating a random message, signing it with the specified key, and
    then using the "VerifySignature" function.

    Internally, the CreateRevocationList function calls this function.
    This function is exposed so an Issuer can call it to determine
    which member key is invalid if CreateRevocationList returns an
    error.  Note that if a key that is in the PrivRL has sigRL entries
    that correspond to it, it will modify the sigRL and remove those
    signatures!

    \param[in] ctx
    The context to use
    \param[in] groupCert
    pointer to a group certificate which contains the gid value to use
    \param[in] privRL
    Points to a private key based revocation list
    \param[in] sigRL
    Points to a signature based revocation list
    \param[in] grpRL
    Points to a group revocation list
    \param[in] randomMessage
    Points to a block of data which is used as a "message" for doing
    EPID signature creation/verification
    \param[in]   keyToValidate
    Pointer to "blob" which contains either a compressed or
    uncompressed key
    \param[out]  fValue
    Points to memory which will receive the "f" value from the member
    key which is being validated

    \retval EPID_SUCCESS
    Key is valid
    \retval EPID_INVALID_MEMBER_KEY
    Key is not valid
    \retval EPID_MEMBER_KEY_REVOKED
    Key is already revoked
    \retval EPID_MEMBER_SIG_REVOKED
    Key is already revoked in SigRL
    \retval EPID_INVALID_SIGNATURE
    Signature was found to be invalid
    \retval EPID_FAILURE
    Key validation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_BAD_ARGS
    Invalid arguments
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
    \retval EPID_UNLUCKY
    Unable to find root before watchdog count exceeded
  */
  EPID_RESULT epidIssuer_validateMemberPrivateKey(
      EPIDIssuer* ctx,
      EPIDGroupCertificateBlob* groupCert,
      void* privRL,
      void* sigRL,
      void* grpRL,
      unsigned char randomMessage[EPID_NUMBER_SIZE],
      void* keyToValidate,
      unsigned char fValue[EPID_NUMBER_SIZE]);

  /*!
    \brief
    Performance rekey

    This function will perform rekey.

    \param[in] ctx
    The context to use
    \param[in] groupPrivateKey
    pointer to the group private key that corresponds to the group
    certificate
    \param[in] seed
    seed to create new partial private key
    \param[out] memberPrivateKey
    Pointer to memory to receive the partial private key

    \retval EPID_SUCCESS
    Keys successfully created and ready to be used
    \retval EPID_FAILURE
    Keys creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
  */
  EPID_RESULT epidIssuer_createPartialPrivateKey(
      EPIDIssuer* ctx,
      EPIDGroupPrivateKeyBlob groupPrivateKey,
      EPIDPrivateKeySeed seed,
      EPIDPartialMemberPrivateKeyBlob* memberPrivateKey);


  /*!
    \brief
    Verify Member Private Key is valid without checking revocation

    This function will verify that a member private key is valid EPID
    key for the group but will not check if it is on any revocation
    lists.

    \param[in] ctx
    The context to use
    \param[in] groupCert
    pointer to a group certificate which contains the gid value to use
    \param[in] keyToValidate
    Pointer to "blob" which contains either a compressed or
    uncompressed key

    \retval EPID_SUCCESS
    Key is valid
    \retval EPID_INVALID_MEMBER_KEY
    Key is not valid
    \retval EPID_FAILURE
    Key validation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_BAD_ARGS
    Invalid arguments
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidIssuer_quickVerifyMemberPrivateKey(
      EPIDIssuer* ctx,
      EPIDGroupCertificateBlob* groupCert,
      void* keyToValidate);

  /*@}*/

#ifdef __cplusplus
}
#endif

#endif  // EPID_INC_EPID_ISSUER_H_
