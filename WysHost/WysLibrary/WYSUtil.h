/*
********************************************************************************
**    Intel Architecture Group 
**    Copyright (C) 2010-2011 Intel Corporation 
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
/***************************************************************************************//**
**
**    @file WysUtil.h
**
**    @brief  Declarations for the WYS Util class and functions
**
**    @author Pradeep Sebestian
**
********************************************************************************************/ 
#ifndef _WYS_UTIL_H_
#define _WYS_UTIL_H_
#include <vector>

using namespace std;

typedef vector<unsigned int> XYPAIR_VECTOR; // x/width - LSW y/height - MSW 
typedef vector<unsigned int> ELEMENTID_VECTOR; // stores element IDs of images

extern "C" {  // only need to export C interface if used by C++ source code

#ifndef WIN32
#ifdef CopyMemory
#undef CopyMemory
#define CopyMemory memcpy
#endif

#ifdef ZeroMemory
#undef ZeroMemory
#define ZeroMemory(p,n) memset(p,0,n)
#endif
#define memcpy_s(a,b,c,d) memcpy(a,c,d)
#else
#ifdef ZeroMemory
#undef ZeroMemory
#define ZeroMemory(p,n) SecureZeroMemory(p,n)
#endif
#define memset(a,b,c) SecureZeroMemory(a,c)
#endif // WIN32


/**********************************************************************************************//**
 * 	Values that represent actions to be taken on wys events. 
 **************************************************************************************************/
#define WYSEVENT_ACTION_NONE						0x0 // do not take any action

#define WYSIWYS_SESSION_INVALID		-1
#define WYSIWYS_SESSION_LOCAL		0

#define WYS_LOCAL_NUM_METADATA_OBJS_Y		4
#define WYS_LOCAL_NUM_METADATA_OBJS_X		3
#define WYS_LOCAL_NUM_METADATA_OBJS			10


/**********************************************************************************************//**
 * 	structure used to represent a word
 **************************************************************************************************/
typedef struct _SHORT_BYTES {
	union {
		struct {
			unsigned char MSB; // (short & 0xFF00) >> 8
			unsigned char LSB; // (short & 0x00FF)
		};
		unsigned short value;
	};
} SHORT_BYTES;

/**********************************************************************************************//**
 * 	structure used to represent a dword
 **************************************************************************************************/
typedef struct _INT_BYTES {
	union {
		struct {
			unsigned char MSWMSB; // (int & 0xFF000000) >> 24
			unsigned char MSWLSB; // (int & 0x00FF0000) >> 16
			unsigned char LSWMSB; // (int & 0x0000FF00) >> 8
			unsigned char LSWLSB; // (int & 0x000000FF)
		};
		struct {
			SHORT_BYTES MSW;
			SHORT_BYTES LSW;
		};
		unsigned int value;
	};
} INT_BYTES;

/**********************************************************************************************//**
 * 	Xy dimensions used to represent x,y or width, height. 
 **************************************************************************************************/
typedef struct _XY_PAIR_BYTES 
{
	union {
		struct {
			SHORT_BYTES xw;
			SHORT_BYTES yh;
		};
		unsigned int xwyh;
	};
} XY_PAIR_BYTES, *PXY_PAIR_BYTES;


typedef struct _WIDGET_MAPPING_BYTES
{
	XY_PAIR_BYTES  location;
	XY_PAIR_BYTES  size;	
} WIDGET_MAPPING_BYTES;

/**********************************************************************************************//**
 * 	Wys image configuration. 
 **************************************************************************************************/
typedef struct _WYSIMAGECONFIG {
	unsigned int maxAllowedClicks; // maximum number of clicks allowed on the image
	unsigned int maxCaptchaLength; // maximum captcha length allowed
}WYSIMAGECONFIG, *PWYSIMAGECONFIG;


/**********************************************************************************************//**
 * 	Wys image stats. 
 **************************************************************************************************/
typedef struct _WYSIMAGESTATS {
	unsigned short totalClicks;		// total number of clicks on the image
	unsigned short numMetaClicks;	// number of unique clicks on the image based on the image metadata
}WYSIMAGESTATS, *PWYSIMAGESTATS;


/**********************************************************************************************//**
 * 	Wys capabilities. 
 **************************************************************************************************/
typedef union _WYSCAPS {
	struct {
		unsigned int WYSHW_IS_PRESENT				:1; // WYS is supported in hardware
		unsigned int WYSHW_CONNECTED_TO_DISPLAY	:1; // WYS hardware is connected to a display
		unsigned int WYSHW_IS_PRIMARY				:1; // WYS HW is configured as primary hw
		unsigned int WYSSW_IS_PRESENT				:1; // WYS software is installed on the platform
		unsigned int WYSSW_CAN_BE_USED			:1; // WYS software installed on the platform is usable
		unsigned int WYSDISPLAY_IS_ACTIVE			:1; // WYS display is currently active
		unsigned int WYSIMGWIN_IN_SOME_DISPLAY	:1; // WYS image window is inside or intersects some display
		unsigned int WYSIMGWIN_FULLY_IN_WYSDISPLAY	:1; // Whole of WYS image window is inside a WYS display
		unsigned int WYSIMGWIN_PARTLY_IN_WYSDISPLAY	:1; // WYS image window partially intersects WYS display
		unsigned int WYSIMGWIN_MAX_IN_WYSDISPLAY		:1; // WYS image window intersects max area of WYS display
		unsigned int WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY	:1; // WYS image window can fit inside the WYS display
		unsigned int WYSIMG_SUPPORTED_IN_CURR_MODE	:1; // Current gfx mode does supports the request
		unsigned int WYSCAPS_ERROR					:1; // Error encountered while collecting capabilities
	};
	unsigned int flags;	// contains all WYSCAPS flag bits
}WYSCAPS, *PWYSCAPS;

/**********************************************************************************************//**
 * 	Wys config. 
 **************************************************************************************************/
typedef struct _WYSCONFIG {
	WYSIMAGECONFIG	globalConfig; // global configuration (can be overrided for individual images)
	unsigned int			wysEventActions;
}WYSCONFIG, *PWYSCONFIG;

enum WYSRESULT{
	WYS_S_SUCCESS						= 0,
	WYS_E_FAILURE						= 1,
	WYS_E_INVALID_MSGUNIT_HEADER		= 2,
	WYS_E_EXPECTED_TLVS					= 3,
	WYS_E_INVALID_SESSION_HANDLE		= 4,
	WYS_E_VALID_MOUSE_INPUT				= 5,
	WYS_E_INVALID_MOUSE_INPUT			= 6,
	WYS_E_APP_API_NOT_IMPLEMENTED		= 7,
	WYS_E_MAX_APP_SESSIONS_REACHED		= 8,
	WYS_E_INVALID_WYSSTDIMAGE_TYPE		= 9,
	WYS_E_INVALID_APP_NAME				= 10,
	WYS_E_INTERNAL_SERVICE_ERROR		= 11,
	WYS_E_MEMORY						= 12,
	WYS_E_INVALID_INPUT					= 13,
	WYS_E_INSUFFICIENT_BUFFER_SIZE		= 14,
	WYS_E_INVALID_TLV_TYPE				= 15,
	WYS_E_IMAGE_ID_IN_USE				= 16,
	WYS_E_CANNOT_DISPLAY_IMAGE			= 17,
	WYS_E_INVALID_IMAGE_ID				= 18,
	WYS_E_INIT_FAILED					= 19,
	WYS_E_ALREADY_INITIALIZED			= 20,
	WYS_E_UNREGISTER_CALLBACK_FIRST		= 21,
	WYS_E_INVALID_IMAGE					= 22,
	WYS_E_INVALID_APP_INSTANCE_ID		= 23,
	WYS_E_APP_SESSION_ABORTED			= 24,
	WYS_E_CREATED_NON_SHARED_SESSION	= 25,
	WYS_E_SHARED_SESSION_NOT_SUPPORTED	= 26,
	WYS_E_LOGO_IMAGE_SIZE_MAX			= 27,
	WYS_E_CAPTCHA_INPUT_LENGTH_MAX		= 28,
	WYS_E_IMAGE_WINDOW_BOUNDS			= 29,
	WYS_E_REGISTER_EVENT_FAIL			= 30,
	WYS_E_VALID_UNIQUE_MOUSE_INPUT		= 31,
	WYS_E_APP_SESSION_NOT_ALIVE			= 32,
	WYS_E_INVALID_PARAMS				= 33,
	WYS_E_API_UNSUPPORTED				= 34,
	WYS_E_UNAVAILABLE					= 35,
	WYS_E_SESSION_STATUS_QUERY_FAILED	= 36,
	WYS_E_NO_EVENTS_REGISTERED			= 37,
	WYS_E_INVALID_APP_INPUT				= 38,
	WYS_E_CLICK_INPUT_COUNT_MAX			= 39,
	WYS_E_INVALID_SESSION				= 40,
	WYS_E_MAX_WYS_SESSIONS_REACHED		= 41,
	WYS_E_IMAGE_SPANNING_DISPLAYS		= 42,
	WYS_E_INVALID_WINDOW_HANDLE			= 43,
	WYS_E_NO_DISPLAY_CONNECTED_TO_IGFX	= 44,
	WYS_E_WINDOW_NOT_IN_IGFX_DISPLAY	= 45,
	WYS_E_WINDOW_HANDLE_OWNER			= 46,
	WYS_E_INVALID_TLV_LENGTH			= 47,
	WYS_E_FRAME_TLV_NOT_FOUND			= 48,
	WYS_E_WINAPI_FAILURE				= 49,
	WYS_E_REGISTRY_READ_ERROR			= 50,
	WYS_E_INTERNAL_SVC_API_UNAVAILABLE	= 51,
	WYS_E_ALREADY_DEINITIALIZED			= 52,
	WYS_E_ELEMENT_IDS_MISMATCH			= 53,

	WYS_E_INVALID_STATUS				= 54 // add above this line
};

/*
 *	WYS_ELEMENT_IDS_CLEAR_FIRST_N	clears the first N element IDs from internal cache. 
 *									N is specified by *pnElementIDs and is less than or equal 
 *									to number of elements in internal cache. If N Is greater
 *									than actual number of elements then WYS_E_NUM_ELEMENT_IDS_EXIST
 *									is returned and pnElementIDs contains the actual
 *									number of Element IDs that were copied to pElementIDs.
*/

/**********************************************************************************************//**
 * 	Macros
 **************************************************************************************************/
#define IS_WYSPRESENT(wysflags)			((wysflags).WYSHW_IS_PRESENT)
#define IS_WYSCAPABLE(wysflags)			((wysflags).WYSHW_CONNECTED_TO_DISPLAY)
#define IS_WYSPRIMARY(wysflags)			((wysflags).WYSHW_IS_PRIMARY)
#define IS_WYSIMGWINOK(wysflags)		((wysflags).WYSIMGWIN_FULLY_IN_WYSDISPLAY)
#define IS_WYSIMGOK(wysflags)			((wysflags).WYSIMG_SUPPORTED_IN_CURR_MODE)
#define IS_WYSIMGWINPARTLY(wysflags)	((wysflags).WYSIMGWIN_PARTLY_IN_WYSDISPLAY)
#define IS_WYSIMGWININDISPLAY(wysflags)	((wysflags).WYSIMGWIN_IN_SOME_DISPLAY)
#define IS_WYSCAPSERROR(wysflags)		((wysflags).WYSCAPS_ERROR)
#define IS_WYSIMGFITDISPLAY(wysflags)	((wysflags).WYSIMGWIN_CAN_FIT_IN_WYSDISPLAY)

#define DECLARE_CRITSECT(cs)			CRITICAL_SECTION cs
#define INIT_CRITSECT(cs)				InitializeCriticalSection(&cs)
#define DELETE_CRITSECT(cs)				DeleteCriticalSection(&cs)
#define ENTER_SAFELY(cs)				EnterCriticalSection(&cs)
#define LEAVE_SAFELY(cs)				LeaveCriticalSection(&cs)

#if 0 //Can be removed?
#define WYS_DECLARE_CRITSECT(cs)
#define WYS_INIT_CRITSECT(cs)
#define WYS_DELETE_CRITSECT(cs)
#define WYS_ENTER_SAFELY(cs)
#define WYS_LEAVE_SAFELY(cs)
#else
#define WYS_DECLARE_CRITSECT(cs)		DECLARE_CRITSECT(cs)
#define WYS_INIT_CRITSECT(cs)			INIT_CRITSECT(cs)
#define WYS_DELETE_CRITSECT(cs)			DELETE_CRITSECT(cs)
#define WYS_ENTER_SAFELY(cs)			ENTER_SAFELY(cs)
#define WYS_LEAVE_SAFELY(cs)			LEAVE_SAFELY(cs)
#endif 

class WysUtil{
	static bool m_bUseBmp2Mpeg2;
public:
	inline static void SetUseBmp2Mpeg2(bool bset) { m_bUseBmp2Mpeg2 = bset; }
	inline static bool isUseBmp2Mpeg2() { return m_bUseBmp2Mpeg2; }
	static unsigned int bytesToInt(INT_BYTES bytes);
	static unsigned short bytesToShort(SHORT_BYTES bytes);
	static void shortToBytes(unsigned short value, SHORT_BYTES *pBytes);
	static void intToBytes(unsigned int value, INT_BYTES *pBytes);
};

}
#endif // _WYS_UTIL_H_