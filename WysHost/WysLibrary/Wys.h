/**
***
*** Copyright  (C) 2014 Intel Corporation. All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation. and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
*** ----------------------------------------------------------------------------
**/
#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <jhi.h>
#include <DelayImp.h>
#include "PavpHandler.h"
#include "WysImage.h"

using namespace std;

/**********************************************************************************************//**
 * 	Values that represent WYSRESULT value. 
 **************************************************************************************************/
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_RED   0x0000FFFF
#define COLOR_GREEN 0x00FF0000
#define COLOR_BLUE  0xFF000000

#define COMMAND_ID_CHECK_INPUT_STATUS 1
#define COMMAND_ID_GET_OTP 2
#define RESP_CODE_APPLET_SUCCESS 0

#define LOFF	0
#define LOFF2	1
#define LOFF3	2

/**********************************************************************************************//**
 * Using WYS_STANDARD_COMMAND_ID will forward the command to the WYS standard commands processor. 
 * Applications using standard local and remote WYS features provided by Intel are expected to use 
 * WYS_STANDARD_COMMAND_ID while using WYSSendRecv() to communicate with the applet.
 * To communicate with a non-intel provided applet, the application and applet communication 
 * need to be configured with WYS_COMMSMODE_DIRECT enabled in the commsConfig parameter passed to WYSInit()
**************************************************************************************************/
#define WYS_STANDARD_COMMAND_ID  0xFFFF0001 // WYS standard command ID, the command ID will 
											// indicate that the command should be forwarded by 
											// applet to WYS standard commands processor

/**********************************************************************************************//**
 * 	Values that represent valid user defined WYS Image Ids.
 * 	User provided WYS Image Ids range is 0x80000001 - 0xFFFFFFFE
 **************************************************************************************************/
#define WYS_IMAGE_ID_ALL				0xFFFFFFFF	// used to specify all wys images
#define WYS_USER_IMAGE_ID_RANGE_START	0x80000001	// user provided image id range start
#define WYS_USER_IMAGE_ID_RANGE_END		0xFFFFFFFE	// user provided image id range end

#define WYS_MAX_ALLOWED_CLICKS_ON_IMAGE		127 
#define WYS_MAX_CAPTCHA_INPUT_LENGTH		127 
#define WYS_MAX_CREATE_WYSIMAGE_SIZE		65000

#pragma pack(1)

typedef JVM_COMM_BUFFER APPLETSENDRECVDATAPARAMS;

typedef struct _WYSWINDOW_PROPS {
	
	IN  HWND WindowHandle;			// Cannot be NULL. jhiHandle of the window in which to display the
  									// image. This is a platform specific Window jhiHandle.
  									// e.g. HWND on windows and X11 ID on Unix based systems.
	IN  unsigned int windowBackgroundColor; // XRGB/ARGB
	IN  void *taInstanceId;				// Applet instance id which can be used with other
											// jhiSessionHandles
}WYSWINDOW_PROPS, *PWYSWINDOW_PROPS;


/**********************************************************************************************//**
 * 	Wys Image component RGB colors. 
 **************************************************************************************************/
typedef struct _WYSIMAGE_COMPONENT_COLORS {
	INT_BYTES   wysImageColor;			// wys image background color (RGB)
	INT_BYTES   frameColor;				// internal frame color (RGB)
	INT_BYTES   frameBorderColor;		// internal frame border color (RGB)
	INT_BYTES   buttonColor;			// button color (RGB), relevant only for PIN pad and OK box windows
	INT_BYTES   buttonBorderColor;		// button border color (RGB), relevant only for PIN pad and OK box windows
	INT_BYTES   fontColor;				// font color (RGB)
}WYSIMAGE_COMPONENT_COLORS;

/**********************************************************************************************//**
 * 	Create wys image structure. 
 **************************************************************************************************/
typedef struct _CREATE_WYSIMAGE {

	unsigned char	reserved0;			// Reserved0
	unsigned char   wysImageType;		// WysStandardImagePinPad or WysStandardImageOK or WysStandardImageCaptcha
	INT_BYTES		reserved1;			// Reserved1
	INT_BYTES		appParam;			// Identifier for the image to be used subsequently to 
 										// identify the user input for this instance of the image.
 										// Cannot be WYS_IMAGE_ID_ALL, but can be in the user wys image id range.
	XY_PAIR_BYTES	wysImageSize;		// Size of the WYS Image in pixels. Note this is not the size of the
  										// parent Window in which the WYS image is displayed.
										// It can be equal to the client area of the parent Window in 
  										// which case Wys Image Position <x,y> should be <0,0>
	unsigned char	frameBorderWidth;   // thickness of frame border in pixels. 0 means no border.
	XY_PAIR_BYTES	buttonSize;			// button size in pixels
	unsigned char	buttonBorderWidth;  // thickness of button border in pixels. 0 means no border.
	unsigned char	fontType;			// type of font to use for button and captcha.
	unsigned char	textLength;			// length of the CAPTCHA text to be generated.
	WYSIMAGE_COMPONENT_COLORS colors;	// colors for all the various components on the wys image.
	XY_PAIR_BYTES	logoSize;			// size of the logo image in width and height. specify zero for no logo.
	unsigned char	logoImage[0];		// logo image data (contains width x height RGB [24bpp] values)
										// should follow logoSize.
}CREATE_WYSIMAGE, *PCREATE_WYSIMAGE;

//
// Request message for SUB_COMM_SUBMIT_INPUT for CAPTCHA window 
//
typedef struct _SUBMIT_INPUT_TEXT_MSG
{
	unsigned char  subCommand; // SUB_COMM_SUBMIT_INPUT
	unsigned char  textSize;   // the length of the submitted text
	unsigned char  text[0];    // the text entered by user in ASCII format, length is according to "textSize" field
} SUBMIT_INPUT_TEXT_MSG;

//
// Request message for SUB_COMM_SUBMIT_INPUT for PIN pad window and OK box window 
//
typedef struct _SUBMIT_INPUT_CLICKS_MSG
{
	unsigned char  subCommand;    // SUB_COMM_SUBMIT_INPUT
	unsigned char  clicksCount;   // the number of user's clicks on window
	XY_PAIR_BYTES  clicks[0];     // locations of the clicks, length is according to "clicksCount" field
} SUBMIT_INPUT_CLICKS_MSG;

//
// Response message to SUB_COMM_BUILD_WINDOW
//
typedef struct _CREATE_WINDOW_RESPONSE_MSG
{
	unsigned char        subCommand;             // SUB_COMM_BUILD_WINDOW
	unsigned char        windowType;             // WINDOW_TYPE_PINPAD, WINDOW_TYPE_OK or WINDOW_TYPE_CATCHA
	INT_BYTES            jhiHandle;                 // jhiHandle to the rendered image that should be used later for SUB_COMM_GET_IMAGE_CHUNK
	unsigned char        S1Kb[KEY_RECORD_SIZE];  // encrypted jhiSessionHandle key record
	unsigned char        CryptoCounter[CRYPTO_COUNTER_SIZE];  // Crypto counter value, generated in FW during the rendering
	INT_BYTES            imageSize;              // total rendered image size
	WIDGET_MAPPING_BYTES map[0];                 // the mapping of the buttons on the window (size = 10), only returned for WINDOW_TYPE_PINPAD
} CREATE_WINDOW_RESPONSE_MSG;

//
// Request message for SUB_COMM_CANCEL 
// 
typedef struct _CANCEL_MSG
{
	unsigned char  subCommand; // SUB_COMM_CANCEL
} CANCEL_MSG;


class Wys
{
private:
	//This is the path to the Intel DAL trusted application that was created in Eclipse.
	string taPath;
	
	/*This is the UUID of this trusted application.
	This is the same value as the UUID field in the Intel DAL trusted application manifest.*/
	string taId;

	UINT32 imageId;

	WYSIMAGEOBJS_HASH_MAP	m_wysImageObjs;

	//flag indicating whether the sample was initialized successfully or not
	bool initialized;

	//flag indicate whether PAVP jhiSessionHandle exists or not
	bool sessionExists;

	//JHI handlers
	JHI_HANDLE jhiHandle;
	JHI_SESSION_HANDLE jhiSessionHandle;

	//encrypted bitmap we display
	UINT8* encryptedBitmap;

	//Instance of PavpHandler
	PavpHandler handler;

	UINT32 pavpSessionHandle;

	WYSWINDOW_PROPS wysWindowProps; 

public:
	static Wys* Session();
	
	WYSRESULT doWysSequence(HWND windowHandle, unsigned char wysImageType);

	WYSRESULT onMouseDown(HWND windowHandle, UINT16 x, UINT16 y);
	WYSRESULT onMouseUp(UINT16 x, UINT16 y);
	
	UINT32 onClickSubmit(wchar_t* userInput, UINT16 inputLength);
	WYSRESULT onClickClear();

	bool getOtp(void* outArr, int arrLength);
	
	bool close();
	bool closePavpSession();

private:
	Wys(void);
	~Wys(void);

	WYSRESULT doWysDisplay(UINT32* pavpSessionHandle, unsigned char wysImageType);
	WYSRESULT createStdImage(UINT32 pavpSessionHandle, IN XY_PAIR_BYTES *pwysImagePosition, IN CREATE_WYSIMAGE *pWysImageCreateParams, IN UINT32 wysImageCreateParamsLen, IN void *rsvdparam1,	IN UINT32 rsvparam2);
	WYSRESULT displayImage(XY_PAIR_BYTES *pWysImagePosition, CREATE_WYSIMAGE *pWysImage );
	WYSRESULT sendWYSStdWindowRequest( CREATE_WYSIMAGE *pWysWindowParams, CREATE_WINDOW_RESPONSE_MSG *pRespData, UINT32 *pRespDataLength );
	WYSRESULT repaintImage(UINT32* pavpSessionHandle, UINT32 wysImageId);

	void deleteWysImageObject( WysImage *pWysImageObj, unsigned int wysImageId );
	WysImage* getWysImageObject( unsigned int wysImageId );
	WYSRESULT releaseImageX(UINT32 wysImageId = WYS_IMAGE_ID_ALL, BOOL bCancel = FALSE);
	WYSRESULT getLocalWysImageFromFWAndDisplay( CREATE_WINDOW_RESPONSE_MSG *windowResp, RECT *wysImageRect, void **pDecoderRenderTargets );
	WYSRESULT getImageFromFW(UINT32 mejhiHandle, UINT32 ImageSize, UINT8 *pImageBuff);
	WYSRESULT jhiSendRecv(UINT32 reqBufLen, void *pSendReq, UINT32 recvBufLen, void *pRecvBuf, UINT32 *pRespDataLen = NULL, UINT8 *pRespData = NULL);
	bool drawClickEffect(HWND windowHandle, int btnL, int btnT, int btnW, int btnH, DWORD color);
	
	WYSRESULT submitClickInput(INT32 wysImageType, XYPAIR_VECTOR *Clicks );
	WYSRESULT submitCaptchaString( UINT32 captchaStringLen, const wchar_t *sCaptchaString );
	WYSRESULT ReleaseImageX(UINT32 wysImageId = WYS_IMAGE_ID_ALL, BOOL bCancel = FALSE);
	WYSRESULT wysCancel(unsigned int appParam);
	void DeleteWysImageObject(WysImage *pWysImageObj, unsigned int wysImageId);
	inline bool eraseClickEffect(WysImage *pWysImageObj) { return FAILED(handler.DisplayVideo(pWysImageObj->m_pDecoderRenderTargets, &pWysImageObj->m_wysImgRenderRect, true )); }
};