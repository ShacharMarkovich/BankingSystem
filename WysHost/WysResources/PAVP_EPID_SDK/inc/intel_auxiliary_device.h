/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/

// A base class to define a generic Intel Auxiliary Device.
// Specific devices will inherit from this class.

#pragma once
#include <windows.h>
#include <initguid.h>
#include <d3d9.h>
#include <dxva2api.h>
#include <intel_auxiliary_api.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }
#endif

#define MIN_SURF_WIDTH	16
#define MIN_SURF_HEIGHT	16

class CAuxiliaryDevice
{
public:
	enum DEVICE_STATE
	{
		Uninitialized       = 0,
		Initialized         = 1,
		AccelServiceCreated = 2,
	};
	
	CAuxiliaryDevice();	
	
	HRESULT	InitializeDevice(IDirect3DDevice9 *pD3DDevice9);
	HRESULT UninitializeDevice();
	HRESULT CreateD3dSurface(
				UINT uiWidth,
				UINT uiHeight,
				D3DFORMAT fmt,
				DWORD dwDxvaType,
				IDirect3DSurface9 **ppSurface
				);

protected:
	
	// protected so child classes can access
	HRESULT QueryAccelGuids(
				GUID **ppAccelGuids, 
				UINT *puiAccelGuidCount
				);
    
	HRESULT QueryAccelRTFormats(
				const GUID *pAccelGuid, 
				D3DFORMAT **ppFormats, 
				UINT *puiCount
				);
    
	HRESULT QueryAccelFormats(
				const GUID *pAccelGuid, 
				D3DFORMAT **ppFormats, 
				UINT *puiCount
				);
    
	HRESULT QueryAccelCaps(
				const GUID *pAccelGuid, 
				void *pCaps, 
				UINT *puiCapSize
				);

	HRESULT ExecuteFunction(
				UINT uiFuncID,
				void *pDataIn,
				UINT uiSizeIn,
				void *pOutputData,
				UINT uiSizeOut
				);
    
	HRESULT CreateAccelService(
				const GUID *pAccelGuid, 
				void *pCreateParams, 
				UINT *puiCreateParamSize
				);

	HRESULT DestroyAccelService(
				const GUID *pAccelGuid,
				void *pDestroyParams
				);

	HRESULT GetDummySurface(
				IDirect3DSurface9 **ppDummySurf
				);

	UINT	SearchArrayForGuid(		
				const UINT length, 
				const GUID* const pArray, 
				const GUID& pTarget 
				);

	void	PrepareAuxFuncStruct(	
				DXVA2_DecodeExecuteParams * const pExecParams,
				DXVA2_DecodeExtensionData *pExtData,
				UINT funcId,
				void const * const pGuid,
				INT sizeIn,
				void const * const pDataOut,
				INT sizeOut  
				);

	DEVICE_STATE                    m_State;
	IDirectXVideoDecoderService*    m_pDecoderService;
	IDirectXVideoDecoder*           m_pAuxDevice;
	IDirect3DSurface9*              m_pDummySurf;	
};