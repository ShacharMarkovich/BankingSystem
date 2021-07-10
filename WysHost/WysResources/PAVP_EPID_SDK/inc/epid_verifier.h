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
  # *******************************************************************
*/

/*!\file
  \brief This file defines the external interface used by "Verifiers"
*/

#ifndef __EPID_API_VERIFIER_H__
#define __EPID_API_VERIFIER_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "epid_constants.h"
#include "epid_errors.h"
#include "epid_types.h"

  /*!
    @defgroup verifierAPI Verifier
    Interfaces that provide implementation of "Verifier" functionality

    @ingroup coreEPIDComponents
    @{
  */

  /*!
     \brief a structure which contains all data items that an Verifier needs
     to perform its designated tasks
  */
  typedef struct EPIDVerifier EPIDVerifier;

  /*!
    \brief
    Create a structure that can be used for calling "Verifier" APIs of
    the SIK.

    This function must be called before calling any "Verifier" APIs.
    The function will allocate memory for the "ctx" and then populate
    the data.  The corresponding "Delete" function must be called to
    avoid memory leaks

    It is assumed that the ECDSA signature on the parameters,
    certificate, and revocation list has already been checked

    There are three valid pairings of createPreComputationBlob and
    verifierPreComputationBlob and one invalid pairing:
    
    TRUE, Ptr -- Use if the pre-computation blob does not exist and it
                 needs to be returned for storage

    FALSE, Ptr -- Use if the pre-computation blob exists and is being
                  passed in to be used

    TRUE, NULL -- Use if the pre-computation blob does not exist and
                  it does NOT need to be returned for storage

    FALSE, NULL -- INVALID

    \param[in] epidParameters
    Points to a structure containing the EPID parameter values to use
    \param[in] groupCertificate
    Points to a group certificate to be used for verifying messages
    from Members
    \param[in] privRL
    Points to a private key based revocation list
    \param[in] sigRL
    Points to a signature based revocation list
    \param[in] grpRL
    Points to a group revocation list
    \param[in] createPreComputationBlob
    Boolean flag that specifies whether the precomputation blob is to
    be created
    \param[in,out] verifierPreComputationBlob
    Points to memory that either contains pre-computation blob or will
    receive it if being created
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
    \retval EPID_GROUP_REVOKED
    Group specified in group certificate has been revoked.
  */
  EPID_RESULT epidVerifier_create(
      EPIDParameterCertificateBlob* epidParameters,
      EPIDGroupCertificateBlob* groupCertificate,
      void* privRL,
      void* sigRL,
      void* grpRL,
      int createPreComputationBlob,
      EPIDVerifierPreComputationBlob* verifierPreComputationBlob,
      EPIDVerifier** pCtx);


  /*!
     \brief Destroy a "Verifier" context and clean up resources

     This function must be called when done calling "Verifier" APIs.

     \param[in] ctx The context to be deleted

     \retval EPID_SUCCESS
     Context successfully deleted
     \retval EPID_FAILURE
     Context deletion failed, possibly due to invalid ctx value
     \retval EPID_NULL_PTR
     Null pointer specified for required parameter

  */
  EPID_RESULT epidVerifier_delete(EPIDVerifier ** ctx);

  /*!
     \brief Verify a members signature and check for revocation status

     This function will verify that a signature returned from a member
     is valid.  The revocation list will also be checked to see if the
     member has been revoked.  baseName can be null since it is an
     optional parameter.  Note that if a basic signature is provided
     (as determined by sigSize) but epidVerifier_create was called
     with a non-null SIG-RL, the member will be considered to have
     failed to prove it wasn't revoked.

     \param[in] ctx The context to use
     \param[in] message Pointer to the message that was used for
                        creating signature
     \param[in] messageLength Integer indicating the length in bytes
                              of "message"
     \param[in] baseName Pointer to the basename value used for
                         creating signature
     \param[in] baseNameLength Integer indicating the length in bytes
                               of "baseName"
     \param[in] signature Pointer to epid signature returned from a
                          member.  
     \param[in] sigSize Size of signature returned from member

     \retval EPID_SUCCESS
     Signature successfully verified
     \retval EPID_FAILURE
     Was unable to complete the verification process
     \retval EPID_INVALID_SIGNATURE
     Signature was found to be invalid
     \retval EPID_MEMBER_KEY_REVOKED
     The member key was found on the current revocation list
     \retval EPID_MEMBER_SIG_REVOKED
     The member failed to prove it was not revoked (Signature based
     revocation)
     \retval EPID_NULL_PTR
     Null pointer specified for required parameter
     \retval EPID_BAD_ARGS
     Invalid arguments
     \retval EPID_OUTOFMEMORY
     Failed to allocate memory
     \retval EPID_UNLUCKY
     unable to find root before watchdog count exceeded
  */
  EPID_RESULT epidVerifier_verifyMemberSignature(EPIDVerifier* ctx,
                                                 unsigned char* message,
                                                 unsigned int messageLength,
                                                 unsigned char* baseName,
                                                 unsigned int baseNameLength,
                                                 void* signature,
                                                 unsigned int sigSize);

  /*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __EPID_API_VERIFIER_H__
