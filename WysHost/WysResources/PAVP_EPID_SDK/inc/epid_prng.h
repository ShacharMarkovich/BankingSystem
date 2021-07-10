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
 * This file defines the interface the EPID SIK uses to retrieve
 * random data.
 */

#ifndef EPID_INC_EPID_PRNG_H_
#define EPID_INC_EPID_PRNG_H_

#ifdef __cplusplus
extern "C" {
#endif

  /*!
    \brief
    Defines the interface the EPID SIK uses to retrieve random data
    from an external bit supplier.

    It is the responsibility of the caller of the EPID interfaces to
    implement a function of this prototype and to then pass a pointer
    to this function into the SIK.  Please refer to the _create
    functions for Issuer, Member, Verifier for more info on how to
    pass in this pointer

    IMPORTANT.  This function must match the prototype of
    IppBitSupplier in ippcp.h.  Including the __STDCALL macro
  */
#if defined( _WIN32 ) || defined ( _WIN64 )
#define __STDCALL  __stdcall
#else
#define __STDCALL
#endif

  typedef int (__STDCALL *EPIDPrngFunc)(unsigned int* pRandData,
                                        int nBits,
                                        void* pUserData);

#ifdef __cplusplus
}
#endif

#endif  // EPID_INC_EPID_PRNG_H_
