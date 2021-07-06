/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2009-2017 Intel Corporation 
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
#include "PavpHandler.h"
#include "crypt_data_gen.h"

#include <cstdio>
#include <comdef.h>
#include <fstream>

using namespace std;

#define SAFE_RELEASEX(x)	if (x != NULL) { ((IDirect3DSurface9 *)(x))->Release(); x = NULL; }

UINT32 PavpHandler::m_hPAVPSlot = 0xB05550FF;

PavpHandler::PavpHandler() 
{ 
#ifndef AMULET
	m_EncryptionMode.eEncryptionType = PAVP_ENCRYPTION_AES128_CTR;
	m_EncryptionMode.eCounterMode = PAVP_COUNTER_TYPE_A;
	m_pCryptoType = D3DCRYPTOTYPE_AES128_CTR;
	m_pDecodeProfile = DXVA2_ModeMPEG2_VLD;

	m_pD3d9Device = NULL;
	m_pD3d9 = NULL;
	m_pDecoderRenderTargets = NULL;
	m_pD3D9VideoDevice=NULL;
	m_pCryptoSession = NULL;
	m_hiGfxMonitor		= NULL;
	m_hMsgFont		= NULL;
	m_pAuthChannel		= NULL;
	m_hAuthChannel      = NULL;
#endif
}

PavpHandler::~PavpHandler()
{
#ifndef AMULET
	ClosePavpSession();

	SAFE_DELETE(m_PavpDevice);

#endif
}

HRESULT PavpHandler::ClosePavpSession()
{
	HRESULT ret = PAVP_STATUS_SUCCESS;
#ifndef AMULET
	if (m_PavpDevice != NULL)
	{
		m_PavpDevice->CleanUp();
		PavpEpidStatus PavpStatus = m_PavpDevice->UninitializeDevice();
		ret = PAVP_EPID_SUCCESS(PavpStatus) ? S_OK : E_FAIL;
	}

	SAFE_RELEASE(m_pCryptoSession);
	SAFE_RELEASE(m_pAuthChannel);
	SAFE_RELEASE(m_pDecoderRenderTargets);
	SAFE_RELEASE(m_pDirect3DEx);
	SAFE_RELEASE(m_pD3d9Device);
	SAFE_RELEASE(m_pD3D9VideoDevice);
	SAFE_RELEASE(m_pD3d9);

#endif
	return ret;
}

HRESULT PavpHandler::DeleteImage(void **m_pDecoderRenderTargets)
{
#ifndef AMULET
	SAFE_RELEASEX(*m_pDecoderRenderTargets);
#else
	if( m_pDecoderRenderTargets != NULL ) {
		::DeleteObject( (HBITMAP)m_pDecoderRenderTargets );
		*m_pDecoderRenderTargets = NULL;
	}
#endif
		return S_OK;
}

bool PavpHandler::EstablishSession(HWND winHandle, unsigned int winBackgroundColor)
{	
#ifndef AMULET
	WYSRESULT retExt = WYS_E_INTERNAL_SERVICE_ERROR;
	HRESULT hr = S_OK;

	WYSCAPS wysCaps;
	
	//Initiate GFX service for displaying the image
	hr = InitGFXService(&wysCaps, true);

	//check if WYS can be display on the current HW and GFX driver.
	hr = CanWYSImageBeDisplayedInWindow( &wysCaps, hr, m_hiGfxMonitor, &m_rciGfxMonitor, winHandle);

	do {
		if( FAILED(hr) || IS_WYSCAPSERROR(wysCaps) ) {
			retExt = WYS_E_WINAPI_FAILURE;
			break;
		} else if( !(IS_WYSPRESENT(wysCaps) ) ) {
			retExt = WYS_E_UNAVAILABLE;
			break;
		} else if(  !(IS_WYSCAPABLE(wysCaps)) ) {  
			retExt = WYS_E_NO_DISPLAY_CONNECTED_TO_IGFX;
			break;
		} else if ( IS_WYSIMGWININDISPLAY(wysCaps) ) {
			if( !IS_WYSIMGFITDISPLAY(wysCaps) ) {
				retExt = WYS_E_CANNOT_DISPLAY_IMAGE;
				break;
			} else if( IS_WYSIMGWINPARTLY(wysCaps) ) {
				retExt = WYS_E_IMAGE_SPANNING_DISPLAYS;
			} else if( !IS_WYSIMGWINOK(wysCaps) ) {
				retExt = WYS_E_WINDOW_NOT_IN_IGFX_DISPLAY;
				break;
			} else if( !( IS_WYSIMGOK(wysCaps)) ) {
				retExt = WYS_E_CANNOT_DISPLAY_IMAGE;
				break;
			}
		} else {
			retExt = WYS_E_WINDOW_NOT_IN_IGFX_DISPLAY;
			break;
		}

		//Copies the coordinates of the client area into m_wysWinRect
		if( !::GetClientRect(winHandle, &m_wysWinRect) ) {
			retExt = WYS_E_WINAPI_FAILURE;
			break;
		}

		//Create D3D9 device using the windows handle
		hr = CreateD3d9DeviceEx((HWND)winHandle);
		if( FAILED(hr) ) {
			break;
		}

		//Create Crypto Session using AES128 crypto type
		hr = InitializeCryptoSession();
		if( FAILED(hr) )  // PAVP_SESSION_TYPE_DECODE
			break;

		//Configure the Crypto session
		hr = ConfigureCryptoSession(NULL); 
		if( FAILED (hr) ) 
			break;

		//Create and initiate PAVP device
		hr = InitializePavpDevice();
		if( FAILED(hr) ) {
			break;
		}
		
		//Create PAVP Auxiliary service
 		if( (m_PavpLastError = m_PavpDevice->CreatePavpService(PAVP_KEY_EXCHANGE_DAA)) != PAVP_STATUS_SUCCESS ) {
			break;
		}

		//Using heavy mode session for secured session.
		m_PavpLastError = m_PavpDevice->ForceHeavyModeEarly(PAVP_EPID_API_VERSION); //API version = 2 << 16
		if(m_PavpLastError != PAVP_STATUS_SUCCESS) {
			break;
		}
		
		//Get PAVP handle
		if ( (m_PavpLastError = m_PavpDevice->GetPavpHandle(&m_hPAVPSlot)) != PAVP_STATUS_SUCCESS ) {
			break;
		}

		BOOL bStillAlive = FALSE;
		//Check if PAVP session is still alive
		hr = GetPavpSessionStatus(bStillAlive);
		if( FAILED(hr) ) 
			break;
		if( !bStillAlive )
		{
			hr = E_FAIL;
			break;
		}

		// paint window background color
		hr = RefreshBackground(winHandle, NULL, winBackgroundColor );
		if( FAILED(hr) ) {
			break;
		}

		if( m_bCustomMsg ) {
			LOGFONT lf;
			
			memset(&lf,0,sizeof(lf));
			wsprintf( lf.lfFaceName, L"%s", L"Neo Sans Intel" );
			lf.lfWeight = FW_SEMIBOLD;
			m_hMsgFont = ::CreateFontIndirect( &lf );
		}

		if( retExt != WYS_E_IMAGE_SPANNING_DISPLAYS ) {
			retExt = WYS_S_SUCCESS;
		}
	} while ( false );

	if( FAILED(hr) ) {
		retExt = WYS_E_INTERNAL_SERVICE_ERROR;
	}

	m_DxLastError = hr;
	return retExt == WYS_S_SUCCESS;
#else
	windowHandle = winHandle;
	windowBackgroundColor = winBackgroundColor;
	if( ::GetClientRect(winHandle, &m_wysWinRect) ) 
	{
		m_hPAVPSlot++; // return a unique dummy handle.
		RefreshBackground(winHandle, NULL,windowBackgroundColor);
		return true;
	}

	return false;
#endif
}

bool PavpHandler::ColorBackgroundGDI(HWND windowHandle, const RECT *pRect, unsigned int color)
{
	BOOL bRet = FALSE;
	HDC hDC = ::GetDC( windowHandle );

	if( hDC != NULL ) {
		HBRUSH hbrOld = (HBRUSH)::SelectObject(hDC, CreateSolidBrush(color));
		bRet = ::PatBlt(hDC, 0, 0, m_wysWinRect.right, m_wysWinRect.bottom, PATCOPY); 
		::DeleteObject( ::SelectObject(hDC, hbrOld));
		::ReleaseDC( windowHandle, hDC );
	}

	return bRet;
}

UINT32 PavpHandler::GetPAVPSlotHandle()
{
	return m_hPAVPSlot;
}

HRESULT PavpHandler::SetNewKey(char* NewKey)
{
#ifndef AMULET
	//PAVP_SET_KEY_WYSIWYS: update Sn_d then enable heavy mode
	PavpEpidStatus stat=m_PavpDevice->SetNewKey(PAVP_SET_KEY_WYSIWYS,(StreamKey *)NewKey);
#else
	S1Kb = new UINT8[KEY_RECORD_SIZE];
	memcpy(S1Kb, NewKey, KEY_RECORD_SIZE);
#endif
	return S_OK;
}

HRESULT PavpHandler::DoDecryptionBlt(void **pDecoderRenderTargets, UINT8 *pSrcFileMemory, UINT srcSize, UINT8 *pIVCounter, RECT *pImgSize, UINT srcImageType, bool bSrcEncrypted, bool bAllocSrc )
{
#ifndef AMULET

    HRESULT                 hr = E_FAIL;
    IDirect3DSurface9       *pEncryptedSrc = NULL;
    IDirect3DSurface9       *pDecryptedDst = NULL;
    BYTE                    *pSysMemEncrypted = NULL;
    D3DAES_CTR_IV           iv;
    UINT                    SysMemSize;
	UINT					Height, Width;


    do
    {
		//
		// Create Destination surface, this surface will finally hold the fw encrypted buffer
		// 
		if( srcImageType == WYS_SRCIMAGE_FILE ) {
			hr = GetBitmapInfo((wchar_t*)pSrcFileMemory, Height, Width);
			if (FAILED(hr)) {
				break;
			}
		} else if( pImgSize == NULL ) {
			break;
		} else {
			Width = pImgSize->right;
			Height = pImgSize->bottom;
		}
        //Now create a regular surface so we can get the surface pitch and we will decrypt to it
        hr = m_pD3d9Device->CreateOffscreenPlainSurfaceEx(  Width, Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pDecryptedDst, NULL, D3DUSAGE_RESTRICTED_CONTENT);

        if( FAILED( hr ) )
            break;
  
        SysMemSize = srcSize; //surface format dependent
		UINT nRemainBytes = SysMemSize%m_ContentProtectionCaps.BlockAlignmentSize;
		if( nRemainBytes > 0 ) 
			SysMemSize = SysMemSize + ( m_ContentProtectionCaps.BlockAlignmentSize - nRemainBytes );


		pSysMemEncrypted = (BYTE *)_aligned_malloc(SysMemSize, m_ContentProtectionCaps.BufferAlignmentStart);
		memcpy_s( pSysMemEncrypted, srcSize, pSrcFileMemory, srcSize );

		hr = m_pD3d9Device->CreateOffscreenPlainSurfaceEx(  Width, Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM,  //This memory allocation consumes system RAM
                                                            &pEncryptedSrc, (HANDLE*)&pSysMemEncrypted, //This parameter can be used in Direct3D 9 to share resources.
                                                            0);
        if (FAILED(hr)) {
            break;
        }

		if( pIVCounter == NULL ) {
            hr = E_FAIL;
			break;
		} else {
			FastMemCopy64( (UINT64*)&(iv.IV), (UINT64*)pIVCounter, sizeof(iv)/8 );
		}
		// Update destination surface with encrypted image 
		hr = m_pCryptoSession->DecryptionBlt(pEncryptedSrc, pDecryptedDst, SysMemSize, NULL, NULL, &iv);
		
        *pDecoderRenderTargets = pDecryptedDst;        

    } while( false );

	SAFE_RELEASE(pEncryptedSrc);

	if (pSysMemEncrypted != NULL) {
		_aligned_free(pSysMemEncrypted);
	}

    return hr;
#else
	//In AMULET there is no real encryption - so no need to decrypt the image, just to show:
    HRESULT hr = E_FAIL;

	HBITMAP hBmp = NULL;
	do {
		hBmp = ::CreateBitmap( pImgSize->right, pImgSize->bottom, 1, 32, pSrcFileMemory );
		if( hBmp == NULL ) {
			break;
		}
		hr = S_OK;
	} while ( false );

	*pDecoderRenderTargets = hBmp;

    return hr;
#endif
}

HRESULT PavpHandler::DisplayVideo(void *pDecoderRenderTargets, RECT *pImgSize, bool bDrawBackground, DWORD flags )
{
	HRESULT             hr = S_OK;

#ifndef AMULET
	
	IDirect3DSurface9   *pD3dBackBuffer = NULL;

    do {	    
        if( NULL == m_pD3d9Device ) {
            hr = E_UNEXPECTED;
            break;
        }
	    hr = GetBackBuffer(&pD3dBackBuffer);
		if( FAILED(hr) ) {
			break;
		}
		if( bDrawBackground ) {
			UINT8 r = (windowBackgroundColor&0xff);
			UINT8 g = ((windowBackgroundColor>>8)&0xff);
			UINT8 b = ((windowBackgroundColor>>16)&0xff);
			D3DCOLOR c = D3DCOLOR_XRGB(r, g, b);

			hr = FillColor( pD3dBackBuffer, NULL, c, 0 );
			if( FAILED(hr) ) {
				break;
			}
		}
		RECT r;
		r.left = r.top = 0;
		r.right = pImgSize->right - pImgSize->left;
		r.bottom = pImgSize->bottom - pImgSize->top;
		hr = m_pD3d9Device->StretchRect((IDirect3DSurface9 *)pDecoderRenderTargets, &r, pD3dBackBuffer, pImgSize, D3DTEXF_NONE);
		flags |= D3DPRESENT_UPDATECOLORKEY;

		if( FAILED(hr) ) {   
            break;
        }

		hr = m_pD3d9Device->PresentEx(NULL, NULL, NULL, NULL, flags);

	} while( false );

    SAFE_RELEASE(pD3dBackBuffer);


    return (m_DxLastError = hr);

#else

	HDC hDC, memDC;
	HBITMAP hBmp;
	BOOL bRet = FALSE;

	do {
		if( bDrawBackground ) 
			RefreshBackground(windowHandle, pImgSize, windowBackgroundColor );
		hBmp = (HBITMAP)pDecoderRenderTargets;
	
		if( NULL == (hDC = ::GetDC( windowHandle )) ) break;

		if( NULL == (memDC = ::CreateCompatibleDC ( hDC )) ) break;
	
		if( NULL == ::SelectObject( memDC, hBmp ) ) break;
		LONG w = pImgSize->right-pImgSize->left;
		LONG h = pImgSize->bottom-pImgSize->top;
		if( FALSE == (bRet = ::BitBlt( hDC, pImgSize->left, pImgSize->top, w, h, memDC, 0, 0, SRCCOPY )) ) break;
	
	} while ( false );

	if( memDC != NULL ) ::DeleteDC( memDC );
	if( hDC != NULL) ::ReleaseDC( windowHandle, hDC );
	if( !bRet ) hr = E_FAIL;

    return hr;

#endif
}

bool PavpHandler::IsPointInRect(XY_PAIR_BYTES *pMouseDimensions)
{
#ifndef AMULET
	RECT rciGfxMonitor;
	POINT ptMouseDown;
	GetiGfxMonRect( &rciGfxMonitor );

	ptMouseDown.x = pMouseDimensions->xw.value;
	ptMouseDown.y = pMouseDimensions->yh.value;

	ClientToScreen( windowHandle, &ptMouseDown );
	return PtInRect( &rciGfxMonitor, ptMouseDown );
#else
	return true;
#endif
}

#ifndef AMULET
IDirect3D9Ex* PavpHandler::m_pDirect3DEx = NULL;
UINT32 PavpHandler::m_nIntelGfxAdapter = -1;

HRESULT PavpHandler::InitGFXService( WYSCAPS *pWysCaps, bool bUseD3d9Overlay )
{
	HRESULT	hr = S_OK;

	do {// Create the D3DEx object.
		if( m_pDirect3DEx == NULL ) {
			hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pDirect3DEx);
		}
		if( !FAILED(hr) && bUseD3d9Overlay ) {
			D3DCAPS9 d3d9caps;
			HMONITOR hMon = NULL;
			hr = CanWYSImageBeDisplayedInWindow(pWysCaps, hr, hMon, NULL );
			if (FAILED(hr) || (m_nIntelGfxAdapter == (DWORD)-1) ) {
				break;
			}

			ZeroMemory(&d3d9caps, sizeof(d3d9caps));
			hr = m_pDirect3DEx->GetDeviceCaps(m_nIntelGfxAdapter, D3DDEVTYPE_HAL, &d3d9caps);
			if (FAILED(hr)) {
				break;
			}
			if( !(d3d9caps.Caps & D3DCAPS_OVERLAY)) { // supports overlay?
				pWysCaps->WYSHW_IS_PRESENT = 0; // maybe old gfx or server gfx ?
				hr = E_FAIL;
				break;
			}
		}
	} while( false );

	
	return hr;
}

bool isIntersect( RECT *r1, RECT *r2, bool *bCanFit )
{
	LONG w1, h1, w2, h2, a1, a2;
	RECT *t1, *t2;

	w1 = r1->right - r1->left;
	h1 = r1->bottom - r1->top;
	w2 = r2->right - r2->left;
	h2 = r2->bottom - r2->top;

	a1 = w1 * h1;
	a2 = w2 * h2;

	if( a1 > a2 ) {
		t1 = r1;
		t2 = r2;
		*bCanFit = false;
	} else {
		t1 = r2;
		t2 = r1;
		*bCanFit = true;
	}
	// t1 always points to rectangle with larger area
	//check top-left point
	if( t2->top >= t1->top && t2->top <= t1->bottom ) {
		if( t2->left >= t1->left && t2->left <= t1->right ) {
			return true;
		}
	}

	//check top-right point
	if( t2->top >= t1->top && t2->top <= t1->bottom ) {
		if( t2->right >= t1->left && t2->right <= t1->right ) {
			return true;
		}
	}

	//check bottom-left point
	if( t2->bottom >= t1->top && t2->bottom <= t1->bottom ) {
		if( t2->left >= t1->left && t2->left <= t1->right ) {
			return true;
		}
	}

	//check bottom-right point
	if( t2->bottom >= t1->top && t2->bottom <= t1->bottom ) {
		if( t2->right >= t1->left && t2->right <= t1->right ) {
			return true;
		}
	}

	return false;
}

HRESULT PavpHandler::GetBitmapInfo(wchar_t *PathName, UINT &Height, UINT &Width)
{
    HBITMAP                 hBitmap;
    BITMAP                  Bitmap;

    //Load the bitmap first using the GDI to get info about it
    hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    if( hBitmap == NULL )
    {
        return E_FAIL;
    }

    //Get info about the object
    GetObjectA( hBitmap, sizeof( BITMAP ), &Bitmap );
    //Unload the bitmap from memory
    DeleteObject( hBitmap );

    Height = Bitmap.bmHeight;
    Width = Bitmap.bmWidth;
    return S_OK;
}

HRESULT PavpHandler::InitializePavpDevice()
{
    HRESULT hr = E_OUTOFMEMORY;

	do { // create PAVP device
		m_PavpLastError = PAVP_STATUS_SUCCESS;
		m_PavpDevice = new CPavpDevice;
		if( m_PavpDevice == NULL ) {
			break;
		}
		
		m_PavpLastError = m_PavpDevice->InitializeDeviceWYS(m_pD3d9Device, m_pCryptoSession, e_GpuCp);

		hr = PAVP_EPID_SUCCESS(m_PavpLastError) ? S_OK : E_FAIL;
	} while( false );

    return hr;
}

HRESULT PavpHandler::CreateD3d9Device()
{
	HRESULT					hr = S_OK;
	D3DPRESENT_PARAMETERS	d3dPresParams;

	do
	{
        ZeroMemory(&d3dPresParams, sizeof(d3dPresParams));

		if( NULL != m_pD3d9Device )
		{
			SAFE_RELEASE(m_pD3d9Device);
		}

        if( NULL == m_pD3d9 )
        {
		    hr = Direct3DCreate9Ex(D3D_SDK_VERSION,&m_pD3d9);
		    if(NULL == m_pD3d9)
		    {
			    hr = E_FAIL;
			    break;
		    }
			D3DCAPS9 d3d9caps;
			hr=m_pD3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);
				if(!(d3d9caps.Caps & D3DCAPS_OVERLAY))
				{
					hr = E_FAIL;
			    break;
				}
        }

		D3DPRESENT_PARAMETERS d3dppex = {0};
		DWORD behaviorSwFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;

		d3dppex.BackBufferWidth = DisplayWidth;
		d3dppex.BackBufferHeight = DisplayHeight;
		d3dppex.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dppex.SwapEffect = D3DSWAPEFFECT_OVERLAY;
		d3dppex.hDeviceWindow = image.targetControl;
		d3dppex.Windowed = TRUE;
		d3dppex.Flags = D3DPRESENTFLAG_VIDEO;
		d3dppex.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dppex.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;
   
		// Create the D3DDevice
		hr = m_pD3d9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
										   NULL, behaviorSwFlags,
										   &d3dppex, NULL,
										   &m_pD3d9Device);
	} while( FALSE );

	return hr;
}

HRESULT PavpHandler::InitializeCryptoSession()
{
	HRESULT hr;
	hr = m_pD3D9VideoDevice->CreateCryptoSession(&m_pCryptoType,
                                                &m_pDecodeProfile,//pGuids+i
                                                &m_pCryptoSession,
                                                &m_hCryptoSession);   
    return hr;
}

HRESULT PavpHandler::GetPavpSessionStatus(BOOL &bStillAlive)
{
    HRESULT hr = E_FAIL;

    do {
        if ( m_pAuthChannel == NULL ) {
            break;
        }

		D3DAUTHENTICATEDCHANNELQUERY_PAVP_STATUS_OUTPUT PavpStatusOut = {0};
		D3DAUTHENTICATEDCHANNEL_QUERY_INPUT             PavpStatusIn  = {0};
        PavpStatusIn.hChannel = m_hAuthChannel;
        PavpStatusIn.QueryType = D3DAUTHENTICATEDQUERY_PAVP_SESSION_STATUS;
        PavpStatusIn.SequenceNumber = m_uiQuerySequence++;
        hr = m_pAuthChannel->Query(sizeof(PavpStatusIn), &PavpStatusIn, sizeof(PavpStatusOut), &PavpStatusOut);
        if (FAILED(hr)) {
            break;
        }
		
        hr = m_pAuthHelper->VerifySignature((const BYTE *)&(PavpStatusOut.Output.QueryType),
                                sizeof(PavpStatusOut) - sizeof(D3D_OMAC),
                                (const BYTE *)&(PavpStatusOut.Output.omac), 
                                sizeof(D3D_OMAC));

        if (FAILED(hr)) {
            hr = S_OK;
        }  
		
        bStillAlive = PavpStatusOut.bSessionAlive;

    } while( false );

    return hr;
}

HRESULT PavpHandler::ConfigureCryptoSession(HANDLE hDxvaDriver)
{
    HRESULT hr = S_OK;
    BYTE buffer[256];
    D3DAUTHENTICATEDCHANNEL_QUERY_INPUT input;
    D3DAUTHENTICATEDCHANNEL_QUERYDEVICEHANDLE_OUTPUT* pQueryDevice;
    D3DAUTHENTICATEDCHANNEL_CONFIGURECRYPTOSESSION ConfigCryptoSession;
    D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT output;

    do
    {
        if (m_hCryptoSession == NULL)
        {
            hr = E_FAIL;
            break;
        }

        if (m_hAuthChannel == NULL)
        {
            hr = E_FAIL;
            break;
        }

     // Query for the device handle
        ZeroMemory(buffer, sizeof(buffer));
        input.hChannel = m_hAuthChannel;
        input.QueryType = D3DAUTHENTICATEDQUERY_DEVICEHANDLE;
        input.SequenceNumber = m_uiQuerySequence++;
        hr = m_pAuthChannel->Query( sizeof(input),
                                    &input,
                                    sizeof(D3DAUTHENTICATEDCHANNEL_QUERYDEVICEHANDLE_OUTPUT),
                                    buffer);
        if (FAILED(hr))
        {
            break;
        }

        hr = m_pAuthHelper->VerifySignature(buffer + sizeof(D3D_OMAC), 
            sizeof(D3DAUTHENTICATEDCHANNEL_QUERYDEVICEHANDLE_OUTPUT) - sizeof(D3D_OMAC),
            buffer,
            sizeof(D3D_OMAC));
        if (FAILED(hr))
        {
            break;
        }

        pQueryDevice = (D3DAUTHENTICATEDCHANNEL_QUERYDEVICEHANDLE_OUTPUT*) buffer;
        m_hDevice = pQueryDevice->DeviceHandle;

        if (m_hDevice == NULL)
        {
            hr = E_FAIL;
            break;
        }

        // Call configure to set the crypto session
        ZeroMemory(&ConfigCryptoSession, sizeof(ConfigCryptoSession));
        ConfigCryptoSession.CryptoSessionHandle = m_hCryptoSession;
        ConfigCryptoSession.DXVA2DecodeHandle = hDxvaDriver;
        ConfigCryptoSession.DeviceHandle = m_hDevice;
        ConfigCryptoSession.Parameters.ConfigureType = D3DAUTHENTICATEDCONFIGURE_CRYPTOSESSION;
        ConfigCryptoSession.Parameters.hChannel = m_hAuthChannel;
        ConfigCryptoSession.Parameters.SequenceNumber = m_uiConfigSequence++;
        hr = m_pAuthHelper->Sign((BYTE*) &(ConfigCryptoSession.Parameters.ConfigureType),
                                 sizeof(ConfigCryptoSession) - sizeof(D3D_OMAC),
                                 (BYTE*) &ConfigCryptoSession,
                                 sizeof(D3D_OMAC));
        if (FAILED(hr))
        {
            break;
        }
        hr = m_pAuthChannel->Configure(sizeof(ConfigCryptoSession), &ConfigCryptoSession, &output);
        if (FAILED(hr))
        {
            break;
        }

    } while( FALSE );

    return hr;

}

HRESULT PavpHandler::CreateD3d9DeviceEx(HWND hWysImgWindow, bool bUseD3d9Overlay)
{
	HRESULT	hr		= S_OK;
	
	do {
	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dppex = {0};
	DWORD behaviorSwFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;

	d3dppex.BackBufferWidth = m_wysWinRect.right;
	d3dppex.BackBufferHeight = m_wysWinRect.bottom;
	d3dppex.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dppex.SwapEffect = bUseD3d9Overlay ? D3DSWAPEFFECT_OVERLAY : D3DSWAPEFFECT_DISCARD; 
	d3dppex.hDeviceWindow = hWysImgWindow;
	d3dppex.Windowed = TRUE;
	d3dppex.Flags = D3DPRESENTFLAG_VIDEO;
	d3dppex.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dppex.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;
   
	// Create the D3DDevice
	hr = m_pDirect3DEx->CreateDeviceEx(
										m_nIntelGfxAdapter,
										D3DDEVTYPE_HAL,
										NULL, behaviorSwFlags,
										&d3dppex, NULL,
										&m_pD3d9Device);
  
    if (FAILED(hr)){
		break;
    } else {
		hr = m_pD3d9Device->QueryInterface(IID_IDirect3DDevice9Video,(void **)&m_pD3D9VideoDevice);
		if(FAILED(hr)) {
		} else 
		{
			ZeroMemory(&m_ContentProtectionCaps, sizeof(m_ContentProtectionCaps));
			hr = GetContentProtectionCaps();
			if(!FAILED(hr))
				hr = InitAuthSession();
		}
		if( FAILED(hr) ) {
			DestroyD3d9();
		}
	}
	} while( false );

	return hr;
}

void PavpHandler::DestroyD3d9()
{
    SAFE_RELEASE(m_pD3D9VideoDevice);
	SAFE_RELEASE(m_pD3d9Device);
}

HRESULT PavpHandler::InitAuthSession()
{
    HRESULT        hr;
    DXVA2_VideoDesc VideoDesc = {0};
    D3DAUTHENTICATEDCHANNELTYPE dact = (D3DAUTHENTICATEDCHANNELTYPE)0;
    UINT        CertLen = 0;
    BYTE       *Cert;
    D3DAUTHENTICATEDCHANNEL_CONFIGUREINITIALIZE         config_init;
    D3DAUTHENTICATEDCHANNEL_CONFIGURE_OUTPUT            output;


	if ((m_ContentProtectionCaps.Caps & (D3DCPCAPS_HARDWARE | D3DCPCAPS_SOFTWARE))) {
        if (m_ContentProtectionCaps.Caps & D3DCPCAPS_HARDWARE) {
            dact = D3DAUTHENTICATEDCHANNELTYPE(D3DAUTHENTICATEDCHANNEL_DRIVER_HARDWARE);
        } else if (m_ContentProtectionCaps.Caps & D3DCPCAPS_SOFTWARE) {
            dact = D3DAUTHENTICATEDCHANNELTYPE(D3DAUTHENTICATEDCHANNEL_DRIVER_SOFTWARE);
        }

        hr = S_OK;

        if (FAILED (hr = m_pD3D9VideoDevice->CreateAuthenticatedChannel(dact, &m_pAuthChannel, &m_hAuthChannel))) {
            return E_FAIL;
        }
        if (0 == m_pAuthChannel) {
            hr = E_UNEXPECTED;
            return E_FAIL;
        } else {
		}
        if(dact != D3DAUTHENTICATEDCHANNEL_D3D9) {
            //  key exchange
            if (FAILED(hr = m_pAuthChannel->GetCertificateSize(&CertLen))) {
                return E_FAIL;
            }
            //    LOG_MESSAGE(" GetCertificateSize %d", CertLen);
            Cert = new BYTE[CertLen];
            if (Cert == NULL) {
                hr = E_OUTOFMEMORY;
                return E_FAIL;
            }
            //Get Certificate
            if (FAILED(hr = m_pAuthChannel->GetCertificate(CertLen, Cert))) {
                delete [] Cert;
                return E_FAIL;
            }
            
            if (m_pAuthHelper != NULL) {
                delete m_pAuthHelper;
                m_pAuthHelper = NULL;
            }

            m_pAuthHelper = new COPPAuthHelper;
			if( m_pAuthHelper == NULL ) {

	            delete [] Cert;
				return E_FAIL;
			}
            hr = m_pAuthHelper->Initialize(Cert, CertLen);
            delete [] Cert;
            Cert = NULL;
            if (FAILED(hr)) {
                return E_FAIL;
            }
            //Check for revokation later
            BYTE Signature[256];
            //  start session
            if (FAILED(hr = m_pAuthHelper->PrepareAuthSignature((BYTE*)&Signature[0], sizeof(Signature)))) {
                return E_FAIL;
            }
            if (FAILED(hr = m_pAuthChannel->NegotiateKeyExchange(sizeof(Signature), Signature))) {
                return E_FAIL;
            } else {
			}

            // Initialize sequence numbers;
            m_uiConfigSequence = rand() | (rand() << 16);
            m_uiQuerySequence  = rand() | (rand() << 16);

            // Call configure to initialize the authenticated channel
            ZeroMemory(&config_init, sizeof(config_init));
            config_init.StartSequenceConfigure = m_uiConfigSequence;
            config_init.StartSequenceQuery = m_uiQuerySequence;
            config_init.Parameters.ConfigureType = D3DAUTHENTICATEDCONFIGURE_INITIALIZE;
            config_init.Parameters.hChannel = m_hAuthChannel;
            config_init.Parameters.SequenceNumber = m_uiConfigSequence++;
            hr = m_pAuthHelper->Sign((BYTE*)&(config_init.Parameters.ConfigureType),
                                     sizeof(config_init) - sizeof(D3D_OMAC),
                                     (BYTE*)&config_init,
                                     sizeof(D3D_OMAC));
            if (FAILED(hr)) {
                return E_FAIL;
            }
            hr = m_pAuthChannel->Configure(sizeof(config_init), &config_init, &output);
            if (FAILED(hr)) {
                return E_FAIL;
            }
        } else {
		}
    } else {
        return E_FAIL;
    }

    return S_OK;
}

int PavpHandler::GetWysHardwareId(D3DADAPTER_IDENTIFIER9 *adapterInfo, HWND hWnd, WYSCAPS *pWysCapsFill, RECT *rcWindow, HRESULT &hr, HMONITOR &hiGfxMon, RECT *rciGfxMon )
{
	UINT32 adapterCount;
	int adapter = -1;
	WYSCAPS wysCaps, *pWysCaps = pWysCapsFill/*&wysCaps*/;
	D3DADAPTER_IDENTIFIER9 adapterInfo1;

	pWysCapsFill->flags = pWysCaps->flags = 0;
	if( (adapterCount = m_pDirect3DEx->GetAdapterCount()) > 0 ) do {

		string driverDescription;
		size_t found;
		HMONITOR hMonFromWnd;
		RECT winRectInVirtualScrCoords;
		UINT32 index;
	
		if( hWnd != NULL ) {
			hMonFromWnd = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONULL);
		} else {
			hMonFromWnd = NULL;
		}
		for(index = 0; index < adapterCount; index++) {
			hr = m_pDirect3DEx->GetAdapterIdentifier(index, 0, &adapterInfo1); // INDEXID_CONTAINER
			if(SUCCEEDED(hr)) {
				driverDescription = adapterInfo1.Description;
				found = driverDescription.find(INTEL_SEARCH_STRING);
				//check if adapter is Intel & the window is somewhat or fully on the display driven by this adapter.
				// also in case of multiple displays/adapters find the adapter to which the display that has
				// maximum of the window area is connected.
				if(adapterInfo1.VendorId == INTEL_ID && found != string::npos) {
					pWysCaps->WYSHW_IS_PRESENT = 1;
					// an intel adapter was found, now see if it has a display connected to it
					HMONITOR hMonFromAdaptor = m_pDirect3DEx->GetAdapterMonitor(index);
					if( hMonFromAdaptor != NULL ) {
						pWysCaps->WYSHW_CONNECTED_TO_DISPLAY = 1;
					} else {
						continue; // check next adapter
					}
					MONITORINFOEX  MonitorInfo;
					MonitorInfo.cbSize = sizeof(MonitorInfo);
					if( GetMonitorInfo(hMonFromAdaptor, &MonitorInfo) ) { // intel Gfx adapter is primary ?
						if( (MonitorInfo.dwFlags & MONITORINFOF_PRIMARY) ) {
							pWysCaps->WYSHW_IS_PRIMARY = 1;
						}
					} else {
						pWysCaps->WYSCAPS_ERROR = 1;
						break;
					}

					adapter = index;
					if( hWnd != NULL && hMonFromWnd != NULL ) { // do it only for a valid window handle and if Wys window is in some monitor
						pWysCaps->WYSIMGWIN_IN_SOME_DISPLAY = 1;
						if( ::GetWindowRect( hWnd, &winRectInVirtualScrCoords ) == FALSE ) {
							pWysCaps->WYSCAPS_ERROR = 1;
							break;
						}
						*rcWindow = winRectInVirtualScrCoords;
						// using rectangle of the window, find out if it intersects the display
						bool bCanFit;
						RECT rcMonitor = MonitorInfo.rcWork;
						pWysCaps->WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY = 1; // assume can fit in wys window
						if( isIntersect( &winRectInVirtualScrCoords, &rcMonitor, &bCanFit ) ) {
							pWysCaps->WYSIMGWIN_PARTLY_IN_WYSDISPLAY = 1; // it is intersecting, assume for now its partly intersecting
							if( !bCanFit ) {
								pWysCaps->WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY = 0;
							}
							pWysCapsFill->flags = pWysCaps->flags;
							memcpy_s( adapterInfo, sizeof(adapterInfo1), &adapterInfo1, sizeof(adapterInfo1) );
							hiGfxMon = hMonFromAdaptor;
							*rciGfxMon = rcMonitor;
							if( hMonFromWnd == hMonFromAdaptor ) {
								pWysCapsFill->WYSIMGWIN_MAX_IN_WYSDISPLAY = 1;
								if( !((winRectInVirtualScrCoords.bottom > rcMonitor.bottom) || (winRectInVirtualScrCoords.right > rcMonitor.right) || 
									(winRectInVirtualScrCoords.top < rcMonitor.top) || (winRectInVirtualScrCoords.left < rcMonitor.left)) ) {
									pWysCapsFill->WYSIMGWIN_FULLY_IN_WYSDISPLAY = 1;
									pWysCapsFill->WYSIMGWIN_PARTLY_IN_WYSDISPLAY = 0;
								} 
								break;
							}
							pWysCaps->flags = 0;
						}
					} else {
						break; //return ordinal number of 1st found intel gfx adapter
					}
				} // else not an intel gfx adapter
			} else { // adapter was removed (hot-pluggable) ?
				pWysCaps->WYSCAPS_ERROR = 1;
				break; // stop and return whatever was found so far
			}
		}
	} while( false );

	if( pWysCapsFill->WYSIMGWIN_FULLY_IN_WYSDISPLAY != 1 && pWysCapsFill->WYSIMGWIN_PARTLY_IN_WYSDISPLAY != 1 ) {
		pWysCapsFill->flags = pWysCaps->flags;
	}
	return adapter;
}

HRESULT PavpHandler::CanWYSImageBeDisplayedInWindow( WYSCAPS *pWysCaps, HRESULT &hr, HMONITOR &hiGfxMon, RECT *rciGfxMon, HWND hWnd )
{
	D3DADAPTER_IDENTIFIER9 adapterInfo = {0};
	RECT rcWindow;

	do {
	pWysCaps->flags = 0;
	m_nIntelGfxAdapter = GetWysHardwareId(&adapterInfo, hWnd, pWysCaps, &rcWindow, hr, hiGfxMon, rciGfxMon );

	if( m_nIntelGfxAdapter != (UINT32)-1 ) {
		if( hWnd == NULL ) { // just return HW capabilities
			break;
		}
		if( pWysCaps->WYSIMGWIN_PARTLY_IN_WYSDISPLAY == 1 || pWysCaps->WYSIMGWIN_FULLY_IN_WYSDISPLAY == 1 ) {
			UINT	rcWidth		= abs(rcWindow.right-rcWindow.left),
					rcHeight	= abs(rcWindow.bottom-rcWindow.top);

			// get current display mode and rotation to determine if wys image window can be displayed and 
			// fits in the display connected to the WYS hardware
			D3DDISPLAYMODEEX d3dDisplayMode;
			D3DDISPLAYROTATION  d3dDisplayRotation;
			d3dDisplayMode.Size = sizeof(d3dDisplayMode);
			hr = m_pDirect3DEx->GetAdapterDisplayModeEx( m_nIntelGfxAdapter, &d3dDisplayMode, &d3dDisplayRotation );
			if (FAILED(hr)) {
				pWysCaps->WYSCAPS_ERROR = 1;
				break;
			}

			if( rcHeight > d3dDisplayMode.Height ||  rcWidth > d3dDisplayMode.Width ) {
				pWysCaps->WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY = 0;
				break; // CheckDeviceOverlayType will surely fail if Wys image cannot fit inside the display
			} else {
				pWysCaps->WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY = 1;
			}

			D3DOVERLAYCAPS          d3doverlaycaps = {0};
			IDirect3D9ExOverlayExtension *d3d9overlay;
			hr = m_pDirect3DEx->QueryInterface(IID_PPV_ARGS(&d3d9overlay));
			if (FAILED(hr)) {
				pWysCaps->WYSCAPS_ERROR = 1;
				break;
			}
			hr = d3d9overlay->CheckDeviceOverlayType(m_nIntelGfxAdapter, D3DDEVTYPE_HAL,
														rcWidth, rcHeight,
														D3DFMT_X8R8G8B8, NULL,
														d3dDisplayRotation,
														&d3doverlaycaps);
			

			if( SUCCEEDED(hr) ) {
				pWysCaps->WYSIMG_SUPPORTED_IN_CURR_MODE = 1;
			} else {
				pWysCaps->WYSCAPS_ERROR = 1;
			}

			SAFE_RELEASE(d3d9overlay);
		}
	} // else m_DxLastError contains error
	} while( false );

	return hr;
}
#endif