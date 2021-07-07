/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2009-2010 Intel Corporation 
********************************************************************************
**                                                                            
**    INTEL CONFIDENTIAL                                                      
**    This file, software, or program is supplied under the terms of a        
**    license agreement and/or nondisclosure agreement with Intel Corporation 
**    and may not be copied or disclosed except in accordance with the        
**    terms of that agreement.  This file, software, or program contains      
**    copyrighted material and/or trade secret information of Intel           
**    Corporation, and must be treated as such.  Intel reserves all rights    
**    in this material, except as the license agreement or nondisclosure      
**    agreement specifically indicate.                                        
**                                                                            
**    All rights reserved.  No part of this program or publication            
**    may be reproduced, transmitted, transcribed, stored in a                
**    retrieval system, or translated into any language or computer           
**    language, in any form or by any means, electronic, mechanical,          
**    magnetic, optical, chemical, manual, or otherwise, without              
**    the prior written permission of Intel Corporation.                      
**                                                                            
**    Intel makes no warranty of any kind regarding this code.  This code     
**    is provided on an "As Is" basis and Intel will not provide any support, 
**    assistance, installation, training or other services.  Intel does not   
**    provide any updates, enhancements or extensions.  Intel specifically    
**    disclaims any warranty of merchantability, noninfringement, fitness     
**    for any particular purpose, or any other warranty.                      
**                                                                            
**    Intel disclaims all liability, including liability for infringement     
**    of any proprietary rights, relating to use of the code.  No license,    
**    express or implied, by estoppel or otherwise, to any intellectual       
**    property rights is granted herein.                                      
**/
#ifndef _PAVP_HANDLER_H
#define _PAVP_HANDLER_H

#include <Windows.h>
#include <d3d9.h>
#include <dxva.h>
#include "intel_pavp_device.h"
#include "intel_pavp_api.h"
#include "d3d9types.h"
#include "WYSUtil.h"

#ifndef WYSAMULET
#include "AuthHelper.h"
#endif

const D3DCOLOR BLACK = D3DCOLOR_XRGB(0x00,0x00,0x00);

//
// PAVP key size
//
#define KEY_RECORD_SIZE      16

//
// The size of the IV (counter) used for encrypting the image
//
#define CRYPTO_COUNTER_SIZE   16

#define WYS_SRCIMAGE_MEMORY				1
#define WYS_SRCIMAGE_FILE				3

#define INTEL_ID 32902
#define INTEL_SEARCH_STRING "Intel"

typedef struct _ImageData
{
	UINT8* BmpBuffer;
	UINT32 BmpBufferSize;
	HWND targetControl;
} ImageData;

class PavpHandler
{
	private:
	static UINT32	m_hPAVPSlot;
	RECT			m_wysWinRect;
	HWND windowHandle;
	unsigned int windowBackgroundColor;

	UINT8			*S1Kb;
	CPavpDevice			*m_PavpDevice;
	ImageData image;

#ifndef AMULET
public:
	static IDirect3D9Ex				*m_pDirect3DEx;
private:
	static UINT32					m_nIntelGfxAdapter;
	HRESULT							m_DxLastError;

	LONG DisplayHeight;
	LONG DisplayWidth;

	IDirect3DDevice9Ex	*m_pD3d9Device;
	IDirect3D9Ex			*m_pD3d9;
	E_PAVPAPI           m_ePavpApi;

	IDirect3DCryptoSession9 *m_pCryptoSession;
	HANDLE                  m_hCryptoSession;
	IDirect3DDevice9Video   *m_pD3D9VideoDevice;
	D3DCONTENTPROTECTIONCAPS	m_ContentProtectionCaps;
	PAVP_ENCRYPTION_MODE        m_EncryptionMode;
	IDirect3DAuthenticatedChannel9  *m_pAuthChannel;
	HANDLE                          m_hAuthChannel;
	HANDLE							m_hDevice;
	UINT                            m_uiQuerySequence;
	UINT							m_uiConfigSequence;
	COPPAuthHelper	                *m_pAuthHelper;

	HFONT							m_hMsgFont;
	HMONITOR						m_hiGfxMonitor;
	RECT							m_rciGfxMonitor;
	GUID							m_pCryptoType;
    GUID							m_pDecodeProfile;
	int								m_PavpLastError;
	
	IDirect3DSurface9           *m_pDecoderRenderTargets;

	bool							m_bCustomMsg;
#endif

public:
	PavpHandler();
	
	HRESULT DoDecryptionBlt(void **pDecoderRenderTargets, UINT8 *pSrcFileMemory, UINT srcSize = 0, UINT8 *pIVCounter = NULL, RECT *pImgSize = NULL,	UINT srcImageType = WYS_SRCIMAGE_FILE, bool bSrcEncrypted = true, bool bAllocSrc = true );
	bool EstablishSession(HWND windowHandle, unsigned int windowBackgroundColor/*, UINT32 *hPAVPSlot, WYSCAPS *pWysCaps*/);
	UINT32 GetPAVPSlotHandle();
	HRESULT SetNewKey(char* NewKey);
	HRESULT DisplayVideo(void *pDecoderRenderTargets, RECT *pImgSize, bool bDrawBackground = false, DWORD flags = 0 );
	HRESULT ClosePavpSession();
	bool RefreshBackground(HWND windowHandle, const RECT *pRect = NULL, unsigned int color = 0) { return (TRUE == ColorBackgroundGDI(windowHandle, pRect, color)); }
	bool IsPointInRect(XY_PAIR_BYTES *pMouseDimensions);
	HRESULT DeleteImage(void **m_pDecoderRenderTargets);
	~PavpHandler();
private:
	bool ColorBackgroundGDI(HWND windowHandle, const RECT *pRect, unsigned int color);

#ifndef AMULET
	HRESULT InitializePavpDevice();
	HRESULT InitializeCryptoSession();
	HRESULT CreateD3d9Device();
	
	HRESULT GetBitmapInfo(wchar_t *PathName, UINT &Height, UINT &Width);

	static int GetWysHardwareId		(D3DADAPTER_IDENTIFIER9 *adapterInfo, HWND hWnd, WYSCAPS *pWysCapsFill, RECT *rcWindow, HRESULT &hr, HMONITOR &hiGfxMon, RECT *rciGfxMon);
	static HRESULT InitGFXService	( WYSCAPS *pWysCaps, bool bUseD3d9Overlay = true );
	HRESULT CreateD3d9DeviceEx(HWND hWysImgWindow, bool bUseD3d9Overlay = true);
	static HRESULT CanWYSImageBeDisplayedInWindow( WYSCAPS *pWysCaps, HRESULT &hr, HMONITOR &hiGfxMon, RECT *rciGfxMon, HWND hWnd = NULL);
	HRESULT ConfigureCryptoSession(HANDLE hDxvaDriver);
	HRESULT GetPavpSessionStatus(BOOL &bStillAlive);
	HRESULT InitAuthSession();
	void DestroyD3d9();

	inline void	   GetiGfxMonRect			(RECT *rc)	{ *rc = m_rciGfxMonitor;								}
	inline HRESULT GetContentProtectionCaps(void){ return m_pD3D9VideoDevice->GetContentProtectionCaps(&m_pCryptoType, &m_pDecodeProfile, &m_ContentProtectionCaps); }
	inline void FastMemCopy64( UINT64 *pD, UINT64 *pS, UINT32 count )	{ while( count-- ) *pD++ = *pS++; }
	inline HRESULT GetBackBuffer( IDirect3DSurface9   **pD3dBackBuffer ){ return m_pD3d9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, pD3dBackBuffer); }
	inline HRESULT FillColor(IDirect3DSurface9 *pDecoderRenderTargets, const RECT *pRects = NULL, D3DCOLOR color = 0xffffffff, DWORD rectCount = 1)
	{
		return m_pD3d9Device->ColorFill( pDecoderRenderTargets, pRects, color );
	}
#endif
};
#endif