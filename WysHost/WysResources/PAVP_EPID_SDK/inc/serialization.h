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
  This file defines serialization interfaces to enable read/write to
  persistent storage and for data transfer
*/


#ifndef __EPID_SERIALIZATION_H__
#define __EPID_SERIALIZATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "epid_errors.h"
#include "epid_constants.h"

/*! 
  @defgroup epidSerializiation Serialization

  These components comprise an implementation of the serialization logic
  as defined in the spec.  In EPID, serialization serves 2 purposes.

  1) Define an interoperable format for transmitting data items between
  EPID entities. i.e. Issuers, Verifiers, Members

  2) Define an example methodology for persistent storage of data
  items. It is expected that differing implementations will have
  different requirements for this capability and that it will be
  replaced or modified

  @ingroup sampleUtils
  @{
*/

/*! 
  \brief Name of serialized object.

  This is currently a character string and is used to pass information
  to the serialization functions they can use to generate file names.
*/    
typedef char* EPIDSerializedObjectName;

/*! 
  \brief Read a specified data item from persistent storage as raw data.

  This function will retrieve the requested data item and return it to
  the caller.

  On input, dataBufferSize indicates how much memory is available to
  receive the data item On output, dataBufferSize indicates the length
  of the unserialized data item.  If insufficient memory is available,
  the required amount will be written to this value

  \param[in] serializedObjectType Identify which data item to be retrieved from storage
  \param[in] dataBuffer Points to memory to receive the unserialized data item
  \param[in,out] dataBufferSize Points to an integer containing buffer size
  \param[in] serializedObjectName Optional name of the object, can be used to generate filename

  \retval EPID_SUCCESS
  Data was successfully retrieved
  \retval EPID_FAILURE
  Data retrieval failed
  \retval EPID_INSUFFICIENT_MEMORY_ALLOCATED
  dataBuffer is not sufficiently large.  Required amount is written to
  dataBufferSize
  \retval EPID_BAD_ARGS
  Error was detected while validating parameters
  \retval EPID_FILE_ERROR
  Error was detected while trying to read specified file
*/
EPID_RESULT epidSerializeRead(EPIDSerializedObjectType serializedObjectType,
                              void* dataBuffer,
                              unsigned int* dataBufferSize,
                              EPIDSerializedObjectName serializedObjectName);


/*! 
  \brief Write a specified data item to persistent storage as raw data.

  This function will write the requested data item to persistent storage

  \param[in] serializedObjectType Identify which data item to be written
  to storage
  \param[in] dataBuffer Points to memory that contains the data item to
  be serialized
  \param[in] dataBufferSize Integer containing buffer size
  \param[in] serializedObjectName Optional name of the object, can be
  used to generate filename

  \retval EPID_SUCCESS
  Data was successfully written
  \retval EPID_FAILURE
  Data write failed
  \retval EPID_BAD_ARGS
  Error was detected while validating parameters
  \retval EPID_FILE_ERROR
  Error was detected while trying to write specified file
*/
EPID_RESULT epidSerializeWrite(EPIDSerializedObjectType serializedObjectType,
                               void* dataBuffer,
                               unsigned int dataBufferSize,
                               EPIDSerializedObjectName serializedObjectName);



/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __EPID_SERIALIZATION_H__
