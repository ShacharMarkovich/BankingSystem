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
 * This file defines the external interface used by "Members".
 */


#ifndef EPID_INC_EPID_MEMBER_H_
#define EPID_INC_EPID_MEMBER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "epid_constants.h"
#include "epid_errors.h"
#include "epid_types.h"
#include "epid_prng.h"

  /*!
    \defgroup memberAPI Member
    Interfaces that provide implementation of "Member" functionality.

    \ingroup coreEPIDComponents
    @{
  */

  /*!
    \brief a structure which contains all data items that a Member
    needs to perform its designated tasks
  */
  typedef struct EPIDMember EPIDMember;


  /*!
    \brief Create a structure that can be used for calling "Member"
    APIs of the SIK.

    This function must be called before calling any "Member" APIs.
    The function will allocate memory for the "ctx" and then populate
    the data.  The corresponding "Delete" function must be called to
    avoid memory leaks.

    It is assumed that the ECDSA signature on the parameters and group
    certificate has already been checked.

    There are three valid pairings of createPreComputationBlob and
    memberPreComputationBlob and one invalid pairing:

    TRUE, Ptr -- Use if the pre-computation blob does not exist and it
    needs to be returned for storage.

    FALSE, Ptr -- Use if the pre-computation blob exists and is being
    passed in to be used.

    TRUE, NULL -- Use if the pre-computation blob does not exist and
    it does NOT need to be returned for storage.

    FALSE, NULL -- INVALID

    \param[in] epidParameters
    Points to a structure containing the EPID parameter values to use
    \param[in] groupCertificate
    Points to a group certificate to be used for verifying messages
    from Members
    \param[in] createPreComputationBlob
    Boolean flag that specifies whether the precomputation blob is to
    be created
    \param[in,out] memberPreComputationBlob
    Points to memory that either contains pre-computation blob or will
    receive it if being created
    \param[in] memberPrivateKey
    Points to the uncompressed private key
    \param[in] pRandFunc
    Function pointer of prng to use
    \param[in] pUserPrngData
    Pointer to optional user data for prng calls
    \param[out] ctx
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
  EPID_RESULT epidMember_create(
      const EPIDParameterCertificateBlob* epidParameters,
      const EPIDGroupCertificateBlob* groupCertificate,
      int createPreComputationBlob,
      EPIDMemberPreComputationBlob* memberPreComputationBlob,
      const EPIDMemberPrivateKeyBlob* memberPrivateKey,
      EPIDPrngFunc pRandFunc,
      void* pUserPrngData,
      EPIDMember** ctx);

  /*!
    \brief Create a structure that can be used for calling "Member"
    APIs of the SIK

    This function must be called before calling any "Member" APIs.
    The function will allocate memory for the "ctx" and then populate
    the data.  The corresponding "Delete" function must be called to
    avoid memory leaks

    It is assumed that the ECDSA signature on the parameters and group
    certificate has already been checked

    If createPreComputationBlob is TRUE, then the pre-computation blob
    is created and returned to the caller for serialization

    If createPreComputationBlob is FALSE, then it is assumed that
    memberPreComputationBlob contains valid data to be used

    \param[in] epidParameters
    Points to a structure containing the EPID parameter values to use
    \param[in] groupCertificate
    Points to a group certificate to be used for verifying messages
    from Members
    \param[in] createPreComputationBlob
    Boolean flag that specifies whether the precomputation blob is to
    be created
    \param[in,out] memberPreComputationBlob
    Points to memory that either contains pre-computation blob or will
    receive it if being created
    \param[in] compressedMemberPrivateKey
    Points to the compressed private key
    \param[in] pRandFunc
    Function pointer of prng to use
    \param[in] pUserPrngData
    Pointer to optional user data for prng calls
    \param[out] ctx
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
  EPID_RESULT epidMember_createCompressed(
      const EPIDParameterCertificateBlob* epidParameters,
      const EPIDGroupCertificateBlob* groupCertificate,
      int createPreComputationBlob,
      EPIDMemberPreComputationBlob* memberPreComputationBlob,
      EPIDCompressedMemberPrivateKeyBlob* compressedMemberPrivateKey,
      EPIDPrngFunc  pRandFunc,
      void* pUserPrngData,
      EPIDMember** ctx);


  /*!
    \brief Destroy a "Member" context and clean up resources

    This function must be called when done calling "Member" APIs.

    \param[in] ctx
    The context to be deleted

    \retval EPID_SUCCESS
    Context successfully deleted
    \retval EPID_FAILURE
    Context deletion failed, possibly due to invalid ctx value
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
  */
  EPID_RESULT epidMember_delete(EPIDMember** ctx);

  /*!
    \brief register an allowed basename for the member

    \param[in] ctx
    The context to use
    \param[in] baseName
    Pointer to the basename value to be used for creating signature
    \param[in] baseNameLength
    Integer indicating the length in bytes of "baseName"

    \retval EPID_SUCCESS
    BaseName successfully registered
    \retval EPID_ALREADY_REGISTERED
    Specified BaseName was already registered
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidMember_registerBaseName(EPIDMember* ctx,
                                          const void* baseName,
                                          const size_t baseNameLength);

  /*!
    \brief Create a signature to be returned to a Verifier

    This function will create a signature that can be used by a
    Verifier to determine if the Member is a legitimate member of a
    particular group.

    If a pre-computed signature exists (indicated by non-NULL value
    for preSignature, it will automatically be used by this function.
    If not, then this function will use the standard signature
    algorithm.

    BaseName can be null since it is an optional parameter

    Caller must allocate memory for the signature and release it when
    the signature is no longer needed

    If BaseName is non-NULL, it will be compared to an internal policy
    database and an error will be returned if the BaseName value is
    not contained in that database

    If preSignature is NULL, then the full signature will be computed
    If preSignature is non-NULL, then it will be used to compute the
    signature and then the memory will be zero'ed out to ensure it is
    never used again

    \param[in] ctx
    The context to use
    \param[in] msg
    Pointer to the message to use for creating signature
    \param[in] msgSize
    Integer indicating the length in bytes of "message"
    \param[in] baseName
    Pointer to the basename value to be used for creating signature
    \param[in] baseNameSize
    Integer indicating the length in bytes of "baseName"
    \param[in] preSignature
    Points to a pre-signature.  Can be NULL
    \param[in] sigRL
    Points to the signature based revocation list.  Can be NULL.
    \param[out] signature
    Points to memory to receive the created signature
    \param[out] sigSize
    Size of signature

    \retval EPID_SUCCESS
    Signature successfully created and ready to be used
    \retval EPID_FAILURE
    Signature creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
    \retval EPID_INSUFFICIENT_MEMORY_ALLOCATED
    Buffer for signature is too small
    \retval EPID_BAD_ARGS
    Invalid arguments

  */
  EPID_RESULT epidMember_signMessage(EPIDMember* ctx,
                                     unsigned char* msg,
                                     unsigned int msgSize,
                                     unsigned char* baseName,
                                     unsigned int baseNameSize,
                                     EPIDPreSignatureBlob* preSignature,
                                     void* sigRL,
                                     void* signature,
                                     unsigned int* sigSize);


  /*!
    \brief
    Pre-compute a signature which can be used to compute an actual
    signature when requested by a Verifier

    This function will create a pre-computed signature which can be
    used to speed up the performance of computing an actual signature
    when requested by a Verifier.

    Caller must allocate memory for the pre-signature and release it
    when the signature is no longer needed

    Caller can create as many pre-computed signatures as desired but
    is responsible for proper memory management

    \param[in] ctx
    The context to use
    \param[out] preSignature
    Points to memory to receive a newly created pre-signature

    \retval EPID_SUCCESS
    Pre-Signature successfully created and ready to be used
    \retval EPID_FAILURE
    Pre-Signature creation failed
    \retval EPID_NULL_PTR
    Null pointer specified for required parameter
    \retval EPID_OUTOFMEMORY
    Failed to allocate memory
  */
  EPID_RESULT epidMember_computePreSignature(
      EPIDMember* ctx,
      EPIDPreSignatureBlob* preSignature);

  /*@}*/

#ifdef __cplusplus
}
#endif

#endif  // EPID_INC_EPID_MEMBER_H_






