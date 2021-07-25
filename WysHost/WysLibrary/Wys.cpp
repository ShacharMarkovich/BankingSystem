/**
***
*** Copyright  (C) 2014-2015 Intel Corporation. All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation. and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
*** ----------------------------------------------------------------------------
**/

/*
Prior to Visual Studio 2015 Update 3, the delay load function hooks were non - const.
They were made const to improve security.
For backwards compatibility with previous Visual Studio versions, we define
the macro DELAYIMP_INSECURE_WRITABLE_HOOKS prior to including DelayImp header
(See details in the DelayImp header file before __pfnDliNotifyHook2 declaration).
*/
#define DELAYIMP_INSECURE_WRITABLE_HOOKS

#include "Wys.h"
#include "atlimage.h"
#include <fstream>

//Prototype for the delay load function
FARPROC WINAPI delayHook(unsigned dliNotify, PDelayLoadInfo pdli);
PfnDliHook __pfnDliNotifyHook2 = delayHook;

// Instance of utility class
WysUtil Wysutil;

WYSCONFIG	gWysConfig		= {0};
WYSCAPS		gWysCaps		= {0};

Wys::Wys()
{
	try
	{
		initialized=true;

		gWysConfig.globalConfig.maxAllowedClicks	= WYS_MAX_ALLOWED_CLICKS_ON_IMAGE;
		gWysConfig.globalConfig.maxCaptchaLength	= WYS_MAX_CAPTCHA_INPUT_LENGTH;
		gWysConfig.wysEventActions					= WYSEVENT_ACTION_NONE;

		//This is the path to the Intel DAL trusted application that was created in Eclipse.
		taPath=getenv("DALSDK");
		taPath.append("\\Samples\\WysSample\\WysTA\\bin\\WysTA.dalp");

		//This is the UUID of this trusted application.
		//This is the same value as the UUID field in the Intel DAL trusted application manifest.
		taId = "4c507bdd2853417b98ee630275d44822";

		//Initialize the JHI
		//Note: this is the first call to JHI function.
		//JHI.dll will now be delayed -load
		//The function delayHook will be called and will perform the load from a trusted path with signature verification
		JHI_RET ret = JHI_Initialize(&jhiHandle, NULL, 0);
		if(ret != JHI_SUCCESS)
		{
			initialized=false;
		}

		wstring ws(taPath.begin(),taPath.end());
		
		//Install the Intel DAL trusted application
		ret=JHI_Install2(jhiHandle, taId.c_str(),ws.c_str());	
		if(ret != JHI_SUCCESS)
		{
			initialized=false;
		}

		//Open session with the Intel DAL Trusted Application 
		ret= JHI_CreateSession(jhiHandle, taId.c_str(), 0, NULL, &jhiSessionHandle);
		if(ret != JHI_SUCCESS)
		{
			initialized=false;
		}

		//PAVP session does not exist
		sessionExists = false;
	}
	catch(std::exception e)
	{
		initialized=false;
	}
}

Wys* Wys::Session()
{
	static Wys instance;
	return &instance;
}

/* Display an encrypted image using WYS */
WYSRESULT Wys::doWysSequence(HWND windowHandle, unsigned char wysImageType)
{
	WYSRESULT wysRet = WYS_S_SUCCESS;

	wysWindowProps.windowBackgroundColor = COLOR_WHITE;
	wysWindowProps.WindowHandle = windowHandle;

	wysWindowProps.taInstanceId = jhiSessionHandle;
	
	//if there is an existing PAVP session - close it
    if (sessionExists)
    {
		ReleaseImageX();
        closePavpSession();
        sessionExists = false;
    }

	//initialize pavp with the window handle
	if(handler.EstablishSession(windowHandle, wysWindowProps.windowBackgroundColor))
	{
		sessionExists = true;
		pavpSessionHandle = handler.GetPAVPSlotHandle();
		//display the image 
		return doWysDisplay(&pavpSessionHandle, wysImageType);
	}
	return wysRet;
}

/* Reports a mouse-down event on the WYS image. */
WYSRESULT Wys::onMouseDown(HWND windowHandle, UINT16 x, UINT16 y)
{
	XY_PAIR_BYTES mouseDownXY;
	WYSRESULT retExt = WYS_E_INVALID_MOUSE_INPUT;
	WysImage *pWysImageObj;
		
	mouseDownXY.xw.value = x;
	mouseDownXY.yh.value = y;
	if(!sessionExists)
		retExt = WYS_E_INVALID_SESSION;
	else
	{
		if(!handler.IsPointInRect(&mouseDownXY))
			return retExt;
		pWysImageObj = getWysImageObject(imageId);
		if( pWysImageObj == NULL ) 
		{
			retExt = WYS_E_INVALID_IMAGE_ID;
		} 
		else if( pWysImageObj->m_bValid ) 
		{
			if( pWysImageObj->m_wysImageType != WysStandardImageCaptcha ) 
			do
			{
				if( pWysImageObj->m_clickRecords.size() > pWysImageObj->m_WysImgConfig.maxAllowedClicks ) 
				{
					retExt = WYS_E_CLICK_INPUT_COUNT_MAX;
					break;
				}

				if ((mouseDownXY.xw.value >= (pWysImageObj->m_wysImgRenderRect.left)) &&
					(mouseDownXY.xw.value <= (pWysImageObj->m_wysImgRenderRect.right)) &&
					(mouseDownXY.yh.value >= (pWysImageObj->m_wysImgRenderRect.top)) && 
					(mouseDownXY.yh.value <= (pWysImageObj->m_wysImgRenderRect.bottom)) )
				{
					if (pWysImageObj->m_nMetadataObjs == 0) 
					{
						pWysImageObj->m_WysButtonDown = mouseDownXY;
						pWysImageObj->m_nWysButtonDown = 1;
						retExt = WYS_E_VALID_MOUSE_INPUT;
						break;
					} 
				} 
				else 
				{
					break;
				} 
				pWysImageObj->m_nWysButtonDown = WYS_INVALID_WYS_BUTTON;
				for (int i=0; i < pWysImageObj->m_nMetadataObjs; i++) 
				{
					if ((mouseDownXY.xw.value >= (pWysImageObj->m_wysImgRenderRect.left+pWysImageObj->m_pMetadataObjs[i].objBounds.x)) &&
						(mouseDownXY.xw.value <= (pWysImageObj->m_wysImgRenderRect.left+pWysImageObj->m_pMetadataObjs[i].objBounds.x + pWysImageObj->m_pMetadataObjs[i].objBounds.w)) &&
						(mouseDownXY.yh.value >= (pWysImageObj->m_wysImgRenderRect.top+pWysImageObj->m_pMetadataObjs[i].objBounds.y)) && 
						(mouseDownXY.yh.value <= (pWysImageObj->m_wysImgRenderRect.top+pWysImageObj->m_pMetadataObjs[i].objBounds.y + pWysImageObj->m_pMetadataObjs[i].objBounds.h)) ) 
					{

						pWysImageObj->m_nWysButtonDown = i; // remember index of button over which mouse button was down

						if( pWysImageObj->m_wysObjActionRenderType == StandardWys || pWysImageObj->m_wysObjActionRenderType == CustomAlphaImage )
						{
							drawClickEffect(windowHandle, pWysImageObj->m_wysImgRenderRect.left+pWysImageObj->m_pMetadataObjs[i].objBounds.x, pWysImageObj->m_wysImgRenderRect.top+pWysImageObj->m_pMetadataObjs[i].objBounds.y,
								pWysImageObj->m_pMetadataObjs[i].objBounds.w, pWysImageObj->m_pMetadataObjs[i].objBounds.h, pWysImageObj->m_actionObjColor);
						} 
						retExt = WYS_E_VALID_MOUSE_INPUT;
						break;
					}
				}
			} 
			while(false);
		}
		else 
		{
			retExt = WYS_E_INVALID_IMAGE;
		}
	}
	return retExt;
}

/* Reports a mouse-up event on the WYS image. */
WYSRESULT Wys::onMouseUp(UINT16 x, UINT16 y)
{
	WYSRESULT retExt = WYS_E_INVALID_MOUSE_INPUT;
	bool bClickWithinImageBounds = false;

	XY_PAIR_BYTES mouseDownXY;
	mouseDownXY.xw.value = x;
	mouseDownXY.yh.value = y;

	if( !sessionExists ) 
		retExt = WYS_E_INVALID_SESSION; 
	else
	{
		WysImage *pWysImageObj = getWysImageObject( imageId );
		if( pWysImageObj == NULL )
		{
			retExt = WYS_E_INVALID_IMAGE_ID;
		} 
		else if( pWysImageObj->m_bValid )
		{
			if( pWysImageObj->m_wysImageType != WysStandardImageCaptcha ) 
				do 
				{
		
			if( pWysImageObj->m_clickRecords.size() > pWysImageObj->m_WysImgConfig.maxAllowedClicks ) 
			{
				retExt = WYS_E_CLICK_INPUT_COUNT_MAX;
				break;
			}
			if( pWysImageObj->m_nWysButtonDown == WYS_INVALID_WYS_BUTTON )
			{
				break;
			}
			// at this point we have a valid mouse down
			if ((mouseDownXY.xw.value >= (pWysImageObj->m_wysImgRenderRect.left)) &&
				(mouseDownXY.xw.value <= (pWysImageObj->m_wysImgRenderRect.right)) &&
				(mouseDownXY.yh.value >= (pWysImageObj->m_wysImgRenderRect.top)) && 
				(mouseDownXY.yh.value <= (pWysImageObj->m_wysImgRenderRect.bottom)) )
			{
				bClickWithinImageBounds = true;
				if( pWysImageObj->m_nMetadataObjs == 0 )
				{
					pWysImageObj->m_WysButtonClick = mouseDownXY;
					if( (pWysImageObj->m_WysButtonClick.xw.value == pWysImageObj->m_WysButtonDown.xw.value) && (pWysImageObj->m_WysButtonClick.yh.value == pWysImageObj->m_WysButtonDown.yh.value) )
					{ // down(x,y) == up(x,y) ?
						pWysImageObj->m_WysButtonClick.xw.value = (mouseDownXY.xw.value - (UINT16)pWysImageObj->m_wysImgRenderRect.left);
						pWysImageObj->m_WysButtonClick.yh.value = (mouseDownXY.yh.value - (UINT16)pWysImageObj->m_wysImgRenderRect.top);
						// keep storing the valid mouse up/down coordinates

						pWysImageObj->m_clickRecords.push_back( pWysImageObj->m_WysButtonClick.xwyh );
						++(pWysImageObj->m_stats.totalClicks);
						retExt = WYS_E_VALID_MOUSE_INPUT;
					}
					break;
				}// else m_nMetadataObjs > 0 and click is within wys image bounds
			} // else m_nMetadataObjs > 0 and click is outside wys image bounds retExt will be WYS_E_INVALID_MOUSE_INPUT
			// at this point we have a valid mouse button down so erase the click effect first before we next step

			int i;
			eraseClickEffect( pWysImageObj );
//			m_pCurrWysImage = NULL;

			if( retExt == WYS_E_INVALID_MOUSE_INPUT && !bClickWithinImageBounds ) 
			{ // mouse up is outside wys image bounds ?
				break;
			}
			for (i=0; i < pWysImageObj->m_nMetadataObjs; i++)// check for a valid mouse up
			{ 
				if ((mouseDownXY.xw.value >= (pWysImageObj->m_wysImgRenderRect.left+pWysImageObj->m_pMetadataObjs[i].objBounds.x)) &&
					(mouseDownXY.xw.value <= (pWysImageObj->m_wysImgRenderRect.left+pWysImageObj->m_pMetadataObjs[i].objBounds.x + pWysImageObj->m_pMetadataObjs[i].objBounds.w)) &&
					(mouseDownXY.yh.value >= (pWysImageObj->m_wysImgRenderRect.top+pWysImageObj->m_pMetadataObjs[i].objBounds.y)) && 
					(mouseDownXY.yh.value <= (pWysImageObj->m_wysImgRenderRect.top+pWysImageObj->m_pMetadataObjs[i].objBounds.y + pWysImageObj->m_pMetadataObjs[i].objBounds.h)) ) {

					if( pWysImageObj->m_nWysButtonDown == i )
					{
						pWysImageObj->m_WysButtonClick.xw.value = (mouseDownXY.xw.value - (UINT16)pWysImageObj->m_wysImgRenderRect.left);
						pWysImageObj->m_WysButtonClick.yh.value = (mouseDownXY.yh.value - (UINT16)pWysImageObj->m_wysImgRenderRect.top);
						pWysImageObj->m_clickRecords.push_back( pWysImageObj->m_WysButtonClick.xwyh );
						pWysImageObj->m_elementIDs.push_back( i );
						++(pWysImageObj->m_stats.totalClicks);
						if( pWysImageObj->m_pMetaClicked[i] == 0 )
						{
							pWysImageObj->m_bUniqueMetaClick = true;
						} else
						{
							pWysImageObj->m_bUniqueMetaClick = false;
						}
						++(pWysImageObj->m_pMetaClicked[i]);
						retExt = WYS_E_VALID_MOUSE_INPUT;
					} 
					break;
				}
			}

			pWysImageObj->m_nWysButtonDown = WYS_INVALID_WYS_BUTTON;

			} while( false );
		} 
		else
		{
			retExt = WYS_E_INVALID_IMAGE;
		}
	}
	return retExt;
}

/* Sends user input to the applet, and gets applet's response. */
UINT32 Wys::onClickSubmit(wchar_t* userInput, UINT16 inputLength)
{
	UINT32 wysRet = WYS_E_FAILURE;

	if(!sessionExists)
	{
		wysRet = WYS_E_INVALID_SESSION;
	}
	else
	{
		WysImage *pWysImageObj = getWysImageObject( imageId );
		if( pWysImageObj == NULL ) 
		{
			wysRet = WYS_E_INVALID_IMAGE_ID;
		} 
		else if( pWysImageObj->m_bValid ) 
		{
			do
			{
				if (pWysImageObj->m_wysImageType == WysStandardImagePinPad || pWysImageObj->m_wysImageType == WysStandardImageOK )
				{
					wysRet = submitClickInput(pWysImageObj->m_wysImageType, &pWysImageObj->m_clickRecords);
				}
				else if(pWysImageObj->m_wysImageType == WysStandardImageCaptcha)
				{
					if( inputLength > WYS_MAX_CAPTCHA_INPUT_LENGTH)
					{
						wysRet = WYS_E_CAPTCHA_INPUT_LENGTH_MAX;
						break;
					}
					wysRet = submitCaptchaString(inputLength, userInput);
				}
				else 
				{
					wysRet = WYS_E_INVALID_WYSSTDIMAGE_TYPE;
					break;
				}
				if( wysRet == WYS_S_SUCCESS ) 
				{
					pWysImageObj->m_bValid = false;
					pWysImageObj->ClearClickRecords();
				}
			} while( false );
		} 
		else
		{
			wysRet = WYS_E_INVALID_IMAGE;
		}
	}
	return wysRet;
}

/* Clears user input. */
WYSRESULT Wys::onClickClear()
{
	WYSRESULT retExt = WYS_S_SUCCESS;
	if(!sessionExists)
		retExt = WYS_E_INVALID_SESSION;
	else
	{
		WysImage *pWysImageObj;
		if( imageId == WYS_IMAGE_ID_ALL )
		{
			if( !m_wysImageObjs.empty() ) 
			{
				WYSIMAGEOBJS_HASH_MAP::iterator it;
				// iterate through all WysImage objs and delete them one by one
				for (it = m_wysImageObjs.begin(); it != m_wysImageObjs.end(); ++it)
				{
					pWysImageObj = it->second;
					if( pWysImageObj->m_bValid ) 
						pWysImageObj->ClearClickRecords();
				    else 
						retExt = WYS_E_INVALID_IMAGE;
				}
			}
		}
		else 
		{
			pWysImageObj = getWysImageObject(imageId);
			if( pWysImageObj == NULL ) 
				retExt = WYS_E_INVALID_IMAGE_ID;
			else if( pWysImageObj->m_bValid )
				pWysImageObj->ClearClickRecords();
			else 
				retExt = WYS_E_INVALID_IMAGE;
		}
	}
	return retExt;
}

/*if user input is OK, Create one time password*/
bool Wys::getOtp(void* outArr, int arrLength)
{
	JHI_RET ret;
	JVM_COMM_BUFFER commBuff;
	char rBuff[2048];
	commBuff.RxBuf->buffer = rBuff; 
	commBuff.RxBuf->length = 2048;
	commBuff.TxBuf->length = 0;
	
	INT32 resp = 0;
	if(jhiHandle!=NULL && jhiSessionHandle!=NULL)
	{
		ret = JHI_SendAndRecv2(jhiHandle, jhiSessionHandle, COMMAND_ID_GET_OTP, &commBuff, &resp);
		if(ret == JHI_SUCCESS)
		{
			if(commBuff.RxBuf->length != arrLength)
			{
				return false;
			}
			memcpy(outArr,commBuff.RxBuf->buffer,commBuff.RxBuf->length);
		}	
	}
	else resp=-1;

	return resp == RESP_CODE_APPLET_SUCCESS;
}

bool Wys::close()
{
	bool res = closePavpSession();
	if (jhiHandle != NULL)
	{
		JHI_RET ret;
		//Close Trusted Application session
		if (jhiSessionHandle != NULL)
		{
			ret = JHI_CloseSession(jhiHandle, &jhiSessionHandle);
			if (ret != JHI_SUCCESS)
			{
				res = false;
			}
		}

		//Uninstall the Trusted Application
		ret = JHI_Uninstall(jhiHandle, const_cast<char*>(taId.c_str()));
		if (ret != JHI_SUCCESS)
		{
			res = false;
		}

		//Deinit the JHI handle
		ret = JHI_Deinit(jhiHandle);
		if (ret != JHI_SUCCESS)
		{
			res = false;
		}
	}
	if (encryptedBitmap != NULL)
	{
		delete[] encryptedBitmap;
		encryptedBitmap = NULL;
	}

	initialized = false;

	return res;
}

bool Wys::closePavpSession()
{
	bool res = true;
	//close PAVP session
	if (!handler.ClosePavpSession())
		res = false;
	return res;
}

/*Displays the image using PAVP session handle*/
WYSRESULT Wys::doWysDisplay(UINT32* pavpSessionHandle, unsigned char wysImageType)
{
	WYSRESULT res;
	
	XY_PAIR_BYTES winPos, logoSize;
	COLORREF winColor, buttonColor, fontColor, frameColor, buttonBorderColor, frameBorderColor;
	UINT32 captchaFont = 0;
	UINT8 *pLogoSignature = NULL;
	UINT32 nLogoSignatureLen = 0;
	CImage imgLogo;
	UINT32 logoImageLen;

	CREATE_WYSIMAGE *wysImage;

	winPos.xw.value = (UINT16)0; 
	winPos.yh.value = (UINT16)0;
	winColor = COLOR_WHITE;
	buttonColor = COLOR_GREEN;
	buttonBorderColor = COLOR_BLACK;
	fontColor = COLOR_WHITE;
	frameColor = COLOR_BLUE;
	frameBorderColor = COLOR_GREEN;
	
	//For this example, no logo is used.
	logoImageLen = logoSize.xwyh = 0;

	UINT32 wysImgLen = sizeof(CREATE_WYSIMAGE) + logoImageLen;
	UINT32 wysImageId = WYS_USER_IMAGE_ID_RANGE_START+1; 	

	wysImage = (CREATE_WYSIMAGE *)new UINT8[wysImgLen];

	if( wysImage != NULL ) 
	{	
		wysImage->wysImageType = wysImageType;
		wysImage->appParam.value = wysImageId; 
		wysImage->wysImageSize.xw.value = (UINT16)256;
		wysImage->wysImageSize.yh.value = (UINT16)256;
		wysImage->buttonSize.xw.value = (UINT16)20;
		wysImage->buttonSize.yh.value = (UINT16)20;
		wysImage->fontType = 0;
		wysImage->textLength = 9;
		wysImage->buttonBorderWidth = 2;
		wysImage->frameBorderWidth = 3;
		wysImage->colors.buttonColor.value = buttonColor;
		wysImage->colors.fontColor.value = fontColor;
		wysImage->colors.wysImageColor.value = winColor;
		wysImage->colors.frameColor.value = frameColor;
		wysImage->colors.buttonBorderColor.value = buttonBorderColor;
		wysImage->colors.frameBorderColor.value = frameBorderColor;
		wysImage->logoSize = logoSize;

		res = createStdImage(*pavpSessionHandle, &winPos, wysImage, wysImgLen, NULL, 0);
		if(res == WYS_S_SUCCESS) 
		{
			imageId = wysImage->appParam.value;
			res = repaintImage(pavpSessionHandle, imageId); 
		}
	}
	else
	{
		res = WYS_E_MEMORY;
	}
	
	delete wysImage;
	wysImage = NULL;
	return res;
}

/* Paints the image */
WYSRESULT Wys::repaintImage(UINT32* pavpSessionHandle, UINT32 wysImageId)
{
	WYSRESULT retExt = WYS_S_SUCCESS;
	
	if( !sessionExists ) 
	{
		retExt = WYS_E_INVALID_SESSION;
	} else
	{
		WysImage *pWysImageObj;
		if( wysImageId == WYS_IMAGE_ID_ALL )
		{
			if( !m_wysImageObjs.empty() ) 
			{
				WYSIMAGEOBJS_HASH_MAP::iterator it;
				bool bFirst = true;
				for (it = m_wysImageObjs.begin(); it != m_wysImageObjs.end(); ++it)
				{
					pWysImageObj = it->second;
					if( pWysImageObj != NULL ) {
						if(!handler.DisplayVideo(pWysImageObj->m_pDecoderRenderTargets, &pWysImageObj->m_wysImgRenderRect, bFirst ) )
						{
							retExt = WYS_E_CANNOT_DISPLAY_IMAGE;
						}
						bFirst = false;
					}
				}
			} 
			else
			{
				handler.RefreshBackground(wysWindowProps.WindowHandle, NULL, wysWindowProps.windowBackgroundColor);
				retExt = WYS_E_INVALID_IMAGE_ID;
			}
		} 
		else 
		{
			pWysImageObj = getWysImageObject( wysImageId );
			if( pWysImageObj == NULL )
			{
				retExt = WYS_E_INVALID_IMAGE_ID;
			}
			else
			{                                          //contains value?
				if( handler.DisplayVideo(pWysImageObj->m_pDecoderRenderTargets, &pWysImageObj->m_wysImgRenderRect, true ))
				{
					retExt = WYS_E_CANNOT_DISPLAY_IMAGE;
				}
			}
		}
	}
	return retExt;
}

bool Wys::drawClickEffect(HWND windowHandle, int btnL, int btnT, int btnW, int btnH, DWORD color)
{
	bool bRet = false;
    HDC hdc = NULL;
	
    if ((hdc = ::GetDC(windowHandle)) != NULL) {
		do {
		HPEN hPen, hPenOrig, hPenOld;
		int btnR = btnL+btnW;
		int btnB = btnT+btnH;
		hPen = ::CreatePen( PS_SOLID, 1, RGB(0x80,0x80,0x80) ); // GRAY
		if( hPen == NULL ) {
			break;
		}
		hPenOrig = (HPEN)::SelectObject( hdc, hPen );
		if( hPenOrig == NULL ) {
			break;
		}

		::MoveToEx( hdc, btnL+LOFF, btnT+LOFF, NULL );
		::LineTo( hdc, btnR-LOFF, btnT+LOFF );
		::LineTo( hdc, btnR-LOFF, btnB-LOFF );
		::LineTo( hdc, btnL+LOFF, btnB-LOFF );
		::LineTo( hdc, btnL+LOFF, btnT+LOFF );
	
		hPen = ::CreatePen( PS_SOLID, 1, color ); // border color
		if( hPen == NULL ) {
			break;
		}
		hPenOld = (HPEN)::SelectObject( hdc, hPen );
		if( hPenOld == NULL ) {
			break;
		}
		::DeleteObject( hPenOld);
		::MoveToEx( hdc, btnL+LOFF2, btnT+LOFF2, NULL );
		::LineTo( hdc, btnR-LOFF2, btnT+LOFF2 );
		::LineTo( hdc, btnR-LOFF2, btnB-LOFF2 );
		::LineTo( hdc, btnL+LOFF2, btnB-LOFF2 );
		::LineTo( hdc, btnL+LOFF2, btnT+LOFF2 );

		hPen = ::CreatePen( PS_SOLID, 1, RGB(0xA0,0xA0,0xA0) ); // LITE GRAY
		if( hPen == NULL ) {
			break;
		}
		hPenOld = (HPEN)::SelectObject( hdc, hPen );
		if( hPenOld == NULL ) {
			break;
		}
		::DeleteObject( hPenOld);
		::MoveToEx( hdc, btnL+LOFF3, btnT+LOFF3, NULL );
		::LineTo( hdc, btnR-LOFF3, btnT+LOFF3 );
		::LineTo( hdc, btnR-LOFF3, btnB-LOFF3 );
		::LineTo( hdc, btnL+LOFF3, btnB-LOFF3 );
		::LineTo( hdc, btnL+LOFF3, btnT+LOFF3 );

		hPenOld = (HPEN)::SelectObject( hdc, hPenOrig );
		if( hPenOld == NULL ) {
			break;
		}
		::DeleteObject( hPenOld );

		bRet = true;
	
		} while( false );
	
		::ReleaseDC(windowHandle, hdc); 
	} else {

	}

	return bRet;
}

/* Creates the image to be display*/
WYSRESULT Wys::createStdImage(IN UINT32 pavpSessionHandle, IN XY_PAIR_BYTES *pwysImagePosition, IN CREATE_WYSIMAGE *pWysImageCreateParams,
					IN UINT32 wysImageCreateParamsLen, IN void *rsvdparam1,	IN UINT32 rsvparam2)
{
	WYSRESULT retExt = WYS_E_INVALID_SESSION_HANDLE;

	do {
		if( pwysImagePosition == NULL || pWysImageCreateParams == NULL ) 
		{
			retExt = WYS_E_INVALID_PARAMS;
		} 
		else if( wysImageCreateParamsLen != ((sizeof(CREATE_WYSIMAGE) + (pWysImageCreateParams->logoSize.xw.value * pWysImageCreateParams->logoSize.yh.value * WYS_LOGO_BYTESPERPIXEL))))
		{
			retExt = WYS_E_INVALID_PARAMS;
		} 
		else if( wysImageCreateParamsLen > WYS_MAX_CREATE_WYSIMAGE_SIZE )
		{
			retExt = WYS_E_LOGO_IMAGE_SIZE_MAX;
		} 
		else
		{
			// do some bounds checking
			unsigned int wysImageId = pWysImageCreateParams->appParam.value;
			if( wysImageId < WYS_USER_IMAGE_ID_RANGE_START || wysImageId > WYS_USER_IMAGE_ID_RANGE_END ) 
			{
				retExt = WYS_E_INVALID_IMAGE_ID; break;
			}
			unsigned int totalButtonH = pWysImageCreateParams->buttonBorderWidth * 2 + pWysImageCreateParams->buttonSize.yh.value;
			unsigned int totalButtonW = pWysImageCreateParams->buttonBorderWidth * 2 + pWysImageCreateParams->buttonSize.xw.value;
			unsigned int totalframeBorderW = pWysImageCreateParams->frameBorderWidth * 2;
			
			if (pWysImageCreateParams->wysImageType == WysStandardImagePinPad)
			{
				if( ((totalframeBorderW + pWysImageCreateParams->logoSize.xw.value + (totalButtonW * WYS_LOCAL_NUM_METADATA_OBJS_X) ) > pWysImageCreateParams->wysImageSize.xw.value) ||
					((totalframeBorderW + pWysImageCreateParams->logoSize.yh.value + (totalButtonH * WYS_LOCAL_NUM_METADATA_OBJS_Y) ) > pWysImageCreateParams->wysImageSize.yh.value) )
				{
					retExt = WYS_E_INVALID_INPUT; break;
				}
			} 
			else if(pWysImageCreateParams->wysImageType == WysStandardImageOK)
			{
				if( ((totalframeBorderW + pWysImageCreateParams->logoSize.xw.value + totalButtonW ) > pWysImageCreateParams->wysImageSize.xw.value) ||
					((totalframeBorderW + pWysImageCreateParams->logoSize.yh.value + totalButtonH ) > pWysImageCreateParams->wysImageSize.yh.value) ) 
				{
					retExt = WYS_E_INVALID_INPUT; break;
				}
			}
			else if(pWysImageCreateParams->wysImageType == WysStandardImageCaptcha)
			{
				if( ((totalframeBorderW + pWysImageCreateParams->logoSize.xw.value ) > pWysImageCreateParams->wysImageSize.xw.value) ||
					((totalframeBorderW + pWysImageCreateParams->logoSize.yh.value ) > pWysImageCreateParams->wysImageSize.yh.value) 
					) 
				{
					retExt = WYS_E_INVALID_INPUT; break;
				}
			} 
			else
			{
				retExt = WYS_E_INVALID_WYSSTDIMAGE_TYPE; break;
			}

			retExt = displayImage(pwysImagePosition, pWysImageCreateParams);
		}
	} while( false );

	return retExt;
}

WYSRESULT Wys::displayImage( XY_PAIR_BYTES *pWysImagePosition, CREATE_WYSIMAGE *pWysImage )
{
	WYSRESULT retExt = WYS_S_SUCCESS;
	do {
		UINT32 wysImageId = pWysImage->appParam.value;
		// is wysImageId already in use ?
		if( getWysImageObject( wysImageId ) != NULL ) {
			retExt = WYS_E_IMAGE_ID_IN_USE;
			break;
		}

		RECT wysWinClientRect;
		// check if wys image fits inside the window client region
		if( ::GetClientRect(wysWindowProps.WindowHandle, &wysWinClientRect ) ) {
			if( ((pWysImagePosition->xw.value + pWysImage->wysImageSize.xw.value) > wysWinClientRect.right) ||
				((pWysImagePosition->yh.value + pWysImage->wysImageSize.yh.value) > wysWinClientRect.bottom)
				) {
				retExt = WYS_E_IMAGE_WINDOW_BOUNDS;
				break;
			}
		} else {
			retExt = WYS_E_WINAPI_FAILURE;
			break;
		}

		pWysImage->reserved1.value = handler.GetPAVPSlotHandle();

		CREATE_WINDOW_RESPONSE_MSG *windowResp;
		UINT32 windowRespSize;
		METADATA_OBJECT *pMetadataObjs;
		int nMetadataObjs;

		windowRespSize = sizeof(CREATE_WINDOW_RESPONSE_MSG) + (pWysImage->wysImageType == WysStandardImagePinPad ? WYS_LOCAL_NUM_METADATA_OBJS*sizeof(WIDGET_MAPPING_BYTES) : 0);
		windowResp = (CREATE_WINDOW_RESPONSE_MSG *) new UINT8[windowRespSize];
		if( windowResp != NULL ) 
		{
			memset(windowResp, 0, windowRespSize);
			if ((retExt = sendWYSStdWindowRequest(pWysImage, windowResp, &windowRespSize)) != WYS_S_SUCCESS)
			{

			} 
			else if( windowResp->imageSize.value > 0 )
			{
				void *pDecoderRenderTargets;
				RECT wysImgRect;
		
				wysImgRect.left		= pWysImagePosition->xw.value;
				wysImgRect.top		= pWysImagePosition->yh.value;
				wysImgRect.right	= pWysImage->wysImageSize.xw.value;
				wysImgRect.bottom	= pWysImage->wysImageSize.yh.value;

				if( (retExt = getLocalWysImageFromFWAndDisplay( windowResp, &wysImgRect, &pDecoderRenderTargets )) == WYS_S_SUCCESS )
				{
					if( pWysImage->wysImageType == WysStandardImagePinPad ) 
					{
						nMetadataObjs = WYS_LOCAL_NUM_METADATA_OBJS;
						pMetadataObjs = new METADATA_OBJECT[nMetadataObjs];
						if( pMetadataObjs != NULL )
						{
							for (int i=0; i < nMetadataObjs; i++) 
							{
								pMetadataObjs[i].objBounds.x = Wysutil.bytesToShort(windowResp->map[i].location.xw);
								pMetadataObjs[i].objBounds.y = Wysutil.bytesToShort(windowResp->map[i].location.yh);
								pMetadataObjs[i].objBounds.w = Wysutil.bytesToShort(windowResp->map[i].size.xw);
								pMetadataObjs[i].objBounds.h = Wysutil.bytesToShort(windowResp->map[i].size.yh);

							}
							WysImage *pWysImageObj = new WysImage(wysImageId, wysImgRect, pDecoderRenderTargets, &gWysConfig.globalConfig, (WysImageType)pWysImage->wysImageType, StandardWys, 
								pWysImage->colors.buttonBorderColor.value );
							if( pWysImageObj != NULL )
							{
								if( pWysImageObj->init(nMetadataObjs, pMetadataObjs) )
								{
									m_wysImageObjs[wysImageId] = pWysImageObj;
								} 
								else
								{
									delete pWysImageObj;
									pWysImageObj = NULL;
									retExt = WYS_E_MEMORY;
								}
							}
							else
							{
								retExt = WYS_E_MEMORY;
							}
							delete[] pMetadataObjs;
							pMetadataObjs = NULL;
						} 
						else
						{
							retExt = WYS_E_MEMORY;
						}
					} 
					else // ok or captcha
					{ 
						WysImage *pWysImageObj = new WysImage(wysImageId, wysImgRect, pDecoderRenderTargets, &gWysConfig.globalConfig, (WysImageType)pWysImage->wysImageType);
						if( pWysImageObj != NULL ) 
						{
							if( pWysImageObj->init(0,NULL) )
							{
								m_wysImageObjs[wysImageId] = pWysImageObj;
							} 
							else 
							{
								delete pWysImageObj;
								pWysImageObj = NULL;
								retExt = WYS_E_MEMORY;
							}
						} 
						else 
						{
							retExt = WYS_E_MEMORY;
						}
					}
				}
			} 
			else
			{
				retExt = WYS_E_INVALID_INPUT;
			}
			delete[] windowResp;
			windowResp = NULL;
		}
		else
		{
			retExt = WYS_E_MEMORY;
		}
	} 
	while ( false );
	
	return retExt;
}

/* Gets the encrypted image from the applet and displays it by the GFX.*/
WYSRESULT Wys::getLocalWysImageFromFWAndDisplay( CREATE_WINDOW_RESPONSE_MSG *windowResp, RECT *pWysImageRect, void **pDecoderRenderTargets )
{
	WYSRESULT retExt = WYS_S_SUCCESS;
	UINT32 FinalImageSize;
	UINT8 *pImageBuff;
	UINT32 wysImageHandle;

	do 
	{
		wysImageHandle = Wysutil.bytesToInt(windowResp->jhiHandle);
		FinalImageSize = Wysutil.bytesToInt(windowResp->imageSize);
		pImageBuff = (UINT8 *)_aligned_malloc(FinalImageSize, sizeof(void *) );
		if( pImageBuff == NULL )
		{
			retExt = WYS_E_MEMORY;
			break;
		}
		if( (retExt = getImageFromFW(wysImageHandle, FinalImageSize, pImageBuff )) == WYS_S_SUCCESS )
		{ 
			HRESULT hr;
			hr = handler.SetNewKey((char *)&windowResp->S1Kb[0]);
			SecureZeroMemory( &windowResp->S1Kb[0], sizeof(windowResp->S1Kb) );
			if( FAILED(hr) )
			{
				retExt = WYS_E_FAILURE;
				break;
			}
			hr = handler.DoDecryptionBlt( pDecoderRenderTargets, pImageBuff, FinalImageSize, windowResp->CryptoCounter, pWysImageRect, WYS_SRCIMAGE_MEMORY, true  );		
		
			if( FAILED(hr) ) 
			{
				retExt = WYS_E_CANNOT_DISPLAY_IMAGE;
			}
		} // else release allocated resources and return
		_aligned_free( pImageBuff );
	} while( false );

	return retExt;
}

/* Gets the encrypted image from the applet.*/
WYSRESULT Wys::getImageFromFW(UINT32 meHandle, UINT32 ImageSize, UINT8 *pImageBuff)
{
	WYSRESULT retExt			= WYS_S_SUCCESS;
	void *pReq					= NULL;
	UINT8 *pResp				= NULL;
	void *inData				= NULL;
	GET_IMAGE_CHUNK_MSG *ImgMsg = NULL;
	UINT16 respBufLen;
	UINT16 reqBufLen;
	UINT32 dataLength;
	UINT32 NumOfChunks;
	UINT32 LastChunkSize;
	UINT32 ChunkIdx;
	UINT32 ChunkSize;
   
	do {
	NumOfChunks = ImageSize / READ_IMAGE_CHUNK_SIZE;    
	LastChunkSize = ImageSize % READ_IMAGE_CHUNK_SIZE;
	if ( LastChunkSize > 0 )
	{
		NumOfChunks += 1;
	}
	else
	{
		LastChunkSize = READ_IMAGE_CHUNK_SIZE;
	}

	ChunkSize = READ_IMAGE_CHUNK_SIZE;
	dataLength = sizeof(GET_IMAGE_CHUNK_MSG);
	reqBufLen = dataLength;
	pReq = new UINT8[reqBufLen];
	// Allocate buffer for request and keep reusing it for every Chunk request
	if (pReq == NULL)
	{
		retExt = WYS_E_MEMORY;
		break;
	}
	inData = pReq;
	ImgMsg = (GET_IMAGE_CHUNK_MSG *)inData;
	ImgMsg->subCommand = SUB_COMM_GET_IMAGE_CHUNK;
	WysUtil::intToBytes(ChunkSize, &ImgMsg->size);
	WysUtil::intToBytes(meHandle, &ImgMsg->handle);
	

	// Allocate buffer for response and keep reusing it for every Chunk response
	pResp = pImageBuff;

	for (ChunkIdx=0; ChunkIdx < NumOfChunks; ChunkIdx++)
	{        
		if ( (ChunkIdx+1)==NumOfChunks )
		{ // need to do some specific initializations for the last chunk
			// assuming last chunk size is != READ_IMAGE_CHUNK_SIZE
			ChunkSize = LastChunkSize;
			WysUtil::intToBytes(ChunkSize, &ImgMsg->size);
		}
		dataLength = ChunkSize;
		respBufLen = dataLength;
		
		// Get a Chunk
		if ((retExt = jhiSendRecv(reqBufLen, pReq, respBufLen, pResp, &dataLength, (UINT8 *)pImageBuff )) == WYS_S_SUCCESS)
		{
			pImageBuff += ChunkSize;
			pResp += ChunkSize;
		}
		else
		{
			retExt = WYS_E_FAILURE;
			break;
		}
	}
	} while ( false );

	if (pReq != NULL)
	{
		delete[] pReq;
		pReq = NULL;
	}
                             
	return retExt;
}

WYSRESULT Wys::jhiSendRecv(UINT32 reqBufLen, void *pSendReq, UINT32 recvBufLen, 
										void *pRecvBuf, UINT32 *pRespDataLen, UINT8 *pRespData )
{
	WYSRESULT					retExt	= WYS_E_INSUFFICIENT_BUFFER_SIZE;
	int							ret;
	APPLETSENDRECVDATAPARAMS	jhisrdp;

	jhisrdp.TxBuf->length = reqBufLen;
	jhisrdp.TxBuf->buffer = pSendReq;
	jhisrdp.RxBuf->length = recvBufLen;
	jhisrdp.RxBuf->buffer = pRecvBuf;

	if(jhiHandle!=NULL && jhiSessionHandle!=NULL)
	{
		ret = JHI_SendAndRecv2(jhiHandle, jhiSessionHandle, WYS_STANDARD_COMMAND_ID, &jhisrdp, NULL ); //0 = cmdId

		if (ret == JHI_SUCCESS) {
			if(((jhisrdp.RxBuf->buffer == NULL) || (jhisrdp.RxBuf->length == 0))) { //sanity check
				retExt = WYS_S_SUCCESS;
			} else {
				if( pRespDataLen != NULL && pRespData != NULL) 
					if( pRecvBuf != NULL && jhisrdp.RxBuf->length != 0 && jhisrdp.RxBuf->length <= *pRespDataLen)  // valid buffer to copy response into?
						memcpy_s(pRespData, *pRespDataLen, pRecvBuf, jhisrdp.RxBuf->length);

				retExt = WYS_S_SUCCESS; 
			}
		} else {
			retExt = WYS_E_FAILURE;
		}
	}
	else 
	{
		retExt = WYS_E_FAILURE;
	}

	return retExt;
}

WysImage* Wys::getWysImageObject( unsigned int wysImageId )
{
	WYSIMAGEOBJS_HASH_MAP::iterator it;

	it = m_wysImageObjs.find( wysImageId );

	if( it != m_wysImageObjs.end() ) {
		return it->second;
	}

	return NULL;
}

WYSRESULT Wys::sendWYSStdWindowRequest( CREATE_WYSIMAGE *pWysWindowParams, 
				CREATE_WINDOW_RESPONSE_MSG *pRespData, UINT32 *pRespDataLength )
{
	WYSRESULT	retExt	= WYS_E_INTERNAL_SERVICE_ERROR;
	void		*pReq	= NULL;
	void		*pResp	= NULL;
	void		*inData	= NULL;
	UINT32	dataLength;
	UINT16	reqBufLen;
	UINT16	respBufLen;


	do{

	dataLength = sizeof(CREATE_WYSIMAGE);

	UINT32 logoImgLen = (WYS_LOGO_BYTESPERPIXEL*pWysWindowParams->logoSize.xw.value*pWysWindowParams->logoSize.yh.value);
	if( logoImgLen > 0  ) {
		dataLength += logoImgLen;
	}

	reqBufLen = dataLength;
	pReq = new UINT8[reqBufLen];
	if (pReq == NULL) {

		retExt = WYS_E_MEMORY;
		break;
	}
	inData = pReq;

	CREATE_WYSIMAGE *WindowMsg;

	WindowMsg = (CREATE_WYSIMAGE *)inData;
	WindowMsg->reserved0 = SUB_COMM_BUILD_WINDOW;
	WindowMsg->wysImageType = pWysWindowParams->wysImageType;
	WindowMsg->buttonBorderWidth = pWysWindowParams->buttonBorderWidth;
	WindowMsg->frameBorderWidth = pWysWindowParams->frameBorderWidth;
	WindowMsg->fontType = pWysWindowParams->fontType;
	WindowMsg->textLength = pWysWindowParams->textLength;
	Wysutil.intToBytes(pWysWindowParams->appParam.value,&WindowMsg->appParam);
	Wysutil.intToBytes(pWysWindowParams->reserved1.value,&WindowMsg->reserved1);
	Wysutil.shortToBytes(pWysWindowParams->wysImageSize.xw.value,&WindowMsg->wysImageSize.xw);
	Wysutil.shortToBytes(pWysWindowParams->wysImageSize.yh.value,&WindowMsg->wysImageSize.yh);
	Wysutil.shortToBytes(pWysWindowParams->buttonSize.xw.value,&WindowMsg->buttonSize.xw);
	Wysutil.shortToBytes(pWysWindowParams->buttonSize.yh.value,&WindowMsg->buttonSize.yh);

	UINT32 wysImageColor = pWysWindowParams->colors.wysImageColor.value;
	UINT32 buttonColor = pWysWindowParams->colors.buttonColor.value;
	UINT32 buttonBorderColor = pWysWindowParams->colors.buttonBorderColor.value;
	UINT32 fontColor = pWysWindowParams->colors.fontColor.value;
	UINT32 frameColor = pWysWindowParams->colors.frameColor.value;
	UINT32 frameBorderColor = pWysWindowParams->colors.frameBorderColor.value;

	WindowMsg->colors.buttonColor.value = (buttonColor<<8) | (buttonColor>>24);
	WindowMsg->colors.fontColor.value = (fontColor<<8) | (fontColor>>24);
	WindowMsg->colors.wysImageColor.value = (wysImageColor<<8) | (wysImageColor>>24);
	WindowMsg->colors.frameColor.value = (frameColor<<8) | (frameColor>>24);
	WindowMsg->colors.buttonBorderColor.value = (buttonBorderColor<<8) | (buttonBorderColor>>24);
	WindowMsg->colors.frameBorderColor.value = (frameBorderColor<<8) | (frameBorderColor>>24);

	Wysutil.shortToBytes(pWysWindowParams->logoSize.xw.value,&WindowMsg->logoSize.xw);
	Wysutil.shortToBytes(pWysWindowParams->logoSize.yh.value,&WindowMsg->logoSize.yh);
	if (logoImgLen > 0) {
		memcpy_s(&WindowMsg->logoImage[0], logoImgLen, pWysWindowParams->logoImage, logoImgLen);
	}

	dataLength = *pRespDataLength;
	respBufLen = dataLength;
	pResp = new UINT8[respBufLen];

	if (pResp == NULL) {
		retExt = WYS_E_MEMORY;
		break;
	}

	retExt = jhiSendRecv(reqBufLen, pReq, respBufLen, pResp, pRespDataLength, (UINT8 *)pRespData );

	}while( 0 );

	if (pReq != NULL) 
	{
		delete[] pReq;
		pReq = NULL;
	}
	if (pResp  != NULL) 
	{
		delete[] pResp;
		pResp = NULL;
	}

	return retExt;
}

WYSRESULT Wys::submitClickInput(INT32 wysImageType, XYPAIR_VECTOR *Clicks )
{
	WYSRESULT retExt					= WYS_E_INTERNAL_SERVICE_ERROR;
	void *pReq							= NULL;
	void *pResp							= NULL;
	SUBMIT_INPUT_CLICKS_MSG *ClicksMsg	= NULL;
	UINT32 InputSize					= 0;
	UINT16 reqBufLen					= 0;
	UINT16 respBufLen					= 0;
	UINT32 clicksCount					= Clicks->size();

	do 
	{
		if( wysImageType == WysStandardImageOK && clicksCount > 1 )
		{ // for OK button send only the first click
			clicksCount =  1;
		}
		InputSize = sizeof(SUBMIT_INPUT_CLICKS_MSG) + sizeof(XY_PAIR_BYTES) * clicksCount;
		reqBufLen = InputSize;
		pReq = new UINT8[reqBufLen];
		if (pReq == NULL) {
			retExt = WYS_E_MEMORY;
			break;
		}

		//inData = pReq;
		ClicksMsg = (SUBMIT_INPUT_CLICKS_MSG*)pReq;
		memset(ClicksMsg, 0, InputSize);
		ClicksMsg->subCommand = SUB_COMM_SUBMIT_INPUT;
		ClicksMsg->clicksCount = clicksCount;
	
		XYPAIR_VECTOR::iterator it;
		XY_PAIR_BYTES pointXY;
		XY_PAIR_BYTES wysButtonClick;
		unsigned int i = 0;

		// PT changes for checking NULL value of "it"
		for( it = Clicks->begin(); i<clicksCount && it != Clicks->end(); ++it, ++i ) 
		{
			wysButtonClick.xwyh = *it;
			pointXY.xw.MSB = (unsigned char) (((unsigned short)wysButtonClick.xw.value & 0xFF00) >> 8);
			pointXY.xw.LSB = (unsigned char) ((unsigned short)wysButtonClick.xw.value & 0x00FF);
			pointXY.yh.MSB = (unsigned char) (((unsigned short)wysButtonClick.yh.value & 0xFF00) >> 8);
			pointXY.yh.LSB = (unsigned char) ((unsigned short)wysButtonClick.yh.value & 0x00FF);
			ClicksMsg->clicks[i] = pointXY;
		}

		retExt = jhiSendRecv(reqBufLen, pReq, respBufLen, pResp );
	} while( 0 );
    
	if (pReq != NULL) 
	{
		delete[] pReq;
		pReq = NULL;
	}
	if (pResp != NULL)
	{
		delete[] pResp;
		pResp = NULL;
	}
	return retExt;
}

/* Submits Captcha user input to the applet*/
WYSRESULT Wys::submitCaptchaString( UINT32 captchaStringLen, const wchar_t *sCaptchaString )
{
	WYSRESULT retExt				= WYS_E_INTERNAL_SERVICE_ERROR;
	void *pReq						= NULL;
	void *pResp						= NULL;
	void *inData					= NULL;
	SUBMIT_INPUT_TEXT_MSG *ptextMsg = NULL;
	UINT32 InputSize				= 0;
	UINT16 reqBufLen				= 0;
	UINT16 respBufLen				= 0;
	char *pCaptchaStr				= NULL;

	do {
	pCaptchaStr = new char[captchaStringLen+1];
	
	sprintf_s( pCaptchaStr, captchaStringLen+1, "%S", sCaptchaString );
	InputSize = sizeof(SUBMIT_INPUT_TEXT_MSG) + captchaStringLen;
	reqBufLen = InputSize;
	pReq = new UINT8[reqBufLen];
	if (pReq == NULL) {
		retExt = WYS_E_MEMORY;
		break;
	}

	inData = pReq;
	ptextMsg = (SUBMIT_INPUT_TEXT_MSG*)inData;
	memset(ptextMsg, 0, InputSize);
	memcpy_s(&(ptextMsg->text), captchaStringLen, pCaptchaStr, captchaStringLen);
	delete [] pCaptchaStr; // free the memory allocated for captcha
	pCaptchaStr = NULL;

	ptextMsg->subCommand = SUB_COMM_SUBMIT_INPUT;
	ptextMsg->textSize = captchaStringLen;

	retExt = jhiSendRecv(reqBufLen, pReq, respBufLen, pResp );
	} while( 0 );
    
	if (pReq != NULL) {
		delete[] pReq;
		pReq = NULL;
	}
	if (pResp != NULL) {
		delete[] pResp;
		pResp = NULL;
	}

	return retExt;
}

WYSRESULT Wys::ReleaseImageX(UINT32 wysImageId, BOOL bCancel)
{
	WYSRESULT retExt = WYS_S_SUCCESS;
	do {
	if( wysImageId == WYS_IMAGE_ID_ALL ) 
	{
		if( !m_wysImageObjs.empty() ) 
		{
			WYSIMAGEOBJS_HASH_MAP::iterator it;
			WysImage *pWysImageObj;
			// iterate through all WysImage objs and delete them one by one
			for (it = m_wysImageObjs.begin(); it != m_wysImageObjs.end(); ++it) 
			{
				pWysImageObj = it->second;
				if( pWysImageObj != NULL )
					wysCancel(wysImageId);
				DeleteWysImageObject(pWysImageObj, wysImageId);
			}
		}
		m_wysImageObjs.clear(); // clear after all objs have been deleted
	}
	else
	{
		WysImage *pWysImageObj = getWysImageObject( wysImageId );
		
		if( pWysImageObj == NULL )
		{
			retExt = WYS_E_INVALID_IMAGE_ID;
			break;
		}

		if( bCancel ) {
			retExt = wysCancel(wysImageId);
		}

		DeleteWysImageObject(pWysImageObj, wysImageId);
	}

	} while( false );

	return retExt;
}

void Wys::DeleteWysImageObject(WysImage *pWysImageObj, unsigned int wysImageId)
{
	if( wysImageId != WYS_IMAGE_ID_ALL ) 
	{
		m_wysImageObjs.erase(wysImageId); // for WYS_IMAGE_ID_ALL clear the map after all objects are deleted
	}
	handler.DeleteImage(&pWysImageObj->m_pDecoderRenderTargets);
	delete pWysImageObj;
}

WYSRESULT Wys::wysCancel(unsigned int appParam)
{
	WYSRESULT retExt		= WYS_E_INTERNAL_SERVICE_ERROR;
	void *pReq				= NULL;
	void *pResp				= NULL;
	CANCEL_MSG *pCancelMsg	= NULL;
	UINT32 InputSize		= 0;
	UINT16 reqBufLen		= 0;
	UINT16 respBufLen		= 0;

	do 
	{
		InputSize = sizeof(CANCEL_MSG);
		reqBufLen = InputSize;
		pReq = new UINT8[reqBufLen];
		if (pReq == NULL)
		{
			retExt = WYS_E_MEMORY;
			break;
		}

		pCancelMsg = (CANCEL_MSG*)pReq;
		pCancelMsg->subCommand = SUB_COMM_CANCEL;
	
		retExt = jhiSendRecv(reqBufLen, pReq, respBufLen, pResp );
	} while( 0 );
    
	if (pReq != NULL) 
	{
		delete[] pReq;
		pReq = NULL;
	}
	if (pResp != NULL)
	{
		delete[] pResp;
		pResp = NULL;
	}

	return retExt;
}

Wys::~Wys(void)
{	
}
