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
  \brief
  This file defines an interface for a set of functions that produce
  PRNG data.
*/

#ifndef __EPID_PRNG_H__
#define __EPID_PRNG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ippcp.h"
#include "epid_prng.h"

/*! 
@defgroup prngFunctions Pseudo Random Number Generator

This component provides an API that supports retrieve PRNG data as
required by Epid components

@ingroup sampleUtils
@{
*/

/*! 
\brief
Initialize the sample PRNG library and load PRNG state from filesystem
if it exists.

If the PRNG state has been saved to filesystem, then each time it is
loaded, the PRNG data that is generated will be based on that start
rather than the default initial seed value.  This ensures that new
PRNG data is always used except when the PRNG state file does not
exist.  If the file does not exist, the same seed value will always be
used.
*/
void getPrngState(void);

/*! 
\brief Save PRNG state data to filesystem.

Saving the PRNG state data to filesystem prevents sample applications
from always using the same PRNG data
*/
void savePrngState(void);

/*! 
\brief provides requested amount of PRNG data

\param[out] pRandData Points to memory which will receive the
                      requested amount of PRNG data
\param[in] nBits Number of PRNG bits to generate
\param[in] pUserData Unused at this time

IMPORTANT.  This function must match the prototype of IppBitSupplier in ippcp.h.  
Including the __STDCALL macro
*/
// IppStatus __STDCALL SamplePrngFunc(Ipp32u* pRandData, int nBits, void* pUserData);

int __STDCALL SamplePrngFunc(unsigned int* pRandData, int nBits, void* pUserData);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __EPID_PRNG_H__
