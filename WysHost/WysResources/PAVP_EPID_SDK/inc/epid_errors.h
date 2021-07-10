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
 * This file defines error codes that are used in the EPID SIK
 */

#ifndef EPID_INC_EPID_ERRORS_H_
#define EPID_INC_EPID_ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

  /*!
    \defgroup EPIDReturnValues EPID Return Values
    This module defines the available return values and the return
    type to be used for all SIK interfaces

    \ingroup commonHeaders

    @{
  */

  /// Zero Eum value for success
#define EPID_SUCCESS_BASE (0)
  /// Minimum error value
#define EPID_ERROR_BASE  (-1000)

  /// \brief Simple macro to be used to determine if status value
  ///        indicates success which by convention is zero or any
  ///        positive number
#define EPID_IS_SUCCESS(status) (EPID_SUCCESS_BASE <= status)
  /// \brief Simple macro to be used to determine if status value
  ///        indicates failure which by convention is any negative
  ///        number
#define EPID_IS_FAILURE(status) (EPID_SUCCESS_BASE >  status)

  /*!
    \brief data type to be used for return type for all external SIK
    interfaces

    Convention for status values is as follows:
    
    Zero indicates "success"

    Any positive number indicates "success with status"

    Any negative number indicates "failure"

    This means that any "success with status" enum values must be
    inserted between "EPID_SUCCESS" and "EPID_FAILURE" and all
    "failure" enum values must be inserted after "EPID_FAILURE" and
    before "EPID_INTERNAL_MAX_NEGATIVE"
  */
  typedef enum {
    /// Function succeeded
    EPID_SUCCESS =      EPID_SUCCESS_BASE,
    /// Function failed
    EPID_FAILURE =        EPID_ERROR_BASE,
    /// Function is not yet implemented
    EPID_NOT_IMPLEMENTED,
    /// Invalid EPID or ECDSA signature was detected
    EPID_INVALID_SIGNATURE,
    /// Member that produced the signature has been revoked
    EPID_MEMBER_KEY_REVOKED,
    /// Incoming buffer size was too small
    EPID_INSUFFICIENT_MEMORY_ALLOCATED,
    /// Function arguments invalid
    EPID_BAD_ARGS,
    /// Error handling file
    EPID_FILE_ERROR,
    /// Pointer is NULL
    EPID_NULL_PTR,
    /// Out of memory
    EPID_OUTOFMEMORY,
    /// Insufficient PRNG data available for requested action
    EPID_OUTOFPRNGDATA,
    /// Loop exited before good value based on random number could be found
    EPID_UNLUCKY,
    /// The member key being submitted for revocation is an invalid key
    EPID_INVALID_MEMBER_KEY,
    /// The specified basename has already been registered
    EPID_ALREADY_REGISTERED,
    /// Group was revoked
    EPID_GROUP_REVOKED,
    /// Member is revoked in signature based revocation list
    EPID_MEMBER_SIG_REVOKED,
    /// \brief THIS MUST BE NEGATIVE AND MUST BE LAST ENTRY IN ENUM,
    ///        used to sanity check that we haven't wrapped enum
    ///        around
    EPID_INTERNAL_MAX_NEGATIVE
  } EPID_RESULT;
  /*@}*/




  extern const char * g_ErrorStrings[];

#define DISPLAY_ERROR_CODE(msg, status)                         \
  if (0 > status && EPID_ERROR_BASE <= status) {                \
    printf("ERROR: %s (%s)\n", msg,                             \
           g_ErrorStrings[0 - (EPID_ERROR_BASE - status)]);}

  // IMPORTANT Don't use this macro unless you are certain that the
  //           following statement is true.  Otherwise results are
  //           unpredictable and will likely include a memory access
  //           violation!!!
  // STATEMENT: if (0 > status && EPID_ERROR_BASE <= status)
#define GET_ERROR_CODE(status) g_ErrorStrings[0 - (EPID_ERROR_BASE - status)]


#ifdef __cplusplus
}
#endif

#endif  // EPID_INC_EPID_ERRORS_H_
