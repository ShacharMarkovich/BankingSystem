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

/*!  \file
  \brief
  This file defines an interface to a set of misc "helper" functions
  used by other components of the SIK
*/

#ifndef __EPID_HELPER_H__
#define __EPID_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "epid_types.h"
#include "epid_errors.h"

/*!
@defgroup helperFunctions Helper Functions

This component provides an implementation of several helper functions
that can be used in all parts of the SIK

@ingroup sampleUtils
@{
*/

/*!
\brief validate C structure sizes

This function will do checks to make sure the compiler has correctly
aligned the C structures with the expected sizes of the blobs as
defined in the specification
*/
EPID_RESULT ValidateStructureSizes();

/*!
\brief display the contents of a memory buffer to STDOUT

\param[in] pBuf pointer to memory buffer to be displayed
\param[in] bufSize length of buffer
*/
void PrintBuffer(unsigned char* pBuf, unsigned int bufSize);

/*!
\brief display the contents of a group certificate to STDOUT

\param[in] groupCert pointer to group certificate
*/
void DisplayGroupCertificate(EPIDGroupCertificateBlob* groupCert);

/*!
\brief display the contents of a group private key to STDOUT

\param[in] groupPrivateKey pointer to group private key
*/
void DisplayGroupPrivateKey(EPIDGroupPrivateKeyBlob* groupPrivateKey);

/*!
\brief display the contents of a member private key to STDOUT

\param[in] memberPrivateKey pointer to member private key
*/
void DisplayMemberPrivateKey(EPIDMemberPrivateKeyBlob* memberPrivateKey);

/*!
\brief
display the contents of a compressed member private key to STDOUT

\param[in] compressedMemberPrivateKey pointer to a compressed member
                                      private key
*/
void DisplayCompressedMemberPrivateKey(
    EPIDCompressedMemberPrivateKeyBlob* compressedMemberPrivateKey);

/*!
\brief display the contents of a signature to STDOUT

\param[in] signature pointer to the signature
*/
void DisplayEPIDSignature(EPIDBasicSignatureBlob* signature);

/*!
\brief display a set of EPID parameters to STDOUT

\param[in] paramsCert pointer to the parameters
*/
void DisplayEPIDParameters(EPIDParameterCertificateBlob* paramsCert);

/*!
\brief display the contents of a revocation list to STDOUT

\param[in] revocationList pointer to the revocation list
*/
void DisplayRevocationList(void* revocationList);

/*!
\brief load the contents of a data file into memory

The data will be loaded to memory allocated by this function.  It is
the responsibility of the caller to free the memory when it is no
longer needed

\param[in] filename name of file to be loaded
\param[in,out] size pointer to integer which on input contains the
                    size of allocated memory and on output contains
                    the size of the file that was loaded.  -1 means
                    load the file no matter how large it may be.

\return 
pointer to loaded file data.  NULL means an error occurred and the
file data was not retrieved
*/
unsigned char* LoadFile(const char* filename, unsigned int* size);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __EPID_HELPER_H__
