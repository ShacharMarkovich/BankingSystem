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
#include <unordered_map>
#include "WYSUtil.h"

using namespace std;


#define	WYS_INVALID_VALUE					(-1)	
#define	WYS_INVALID_WYS_BUTTON				WYS_INVALID_VALUE

/**********************************************************************************************//**
 * 	Values that represent LOGO details. 
 **************************************************************************************************/
#define WYS_LOGO_BITSPERPIXEL						24
#define WYS_LOGO_BYTESPERPIXEL						(WYS_LOGO_BITSPERPIXEL/8)

#define  SUB_COMM_BUILD_WINDOW     1 // build a standard window (PIN pad, CAPTCHA or OK box)
#define  SUB_COMM_GET_IMAGE_CHUNK  3 // retrieve the locally rendered image block by block
#define  SUB_COMM_SUBMIT_INPUT     4 // submit user input to a standard window
#define  SUB_COMM_CANCEL           5 // cancel the WYS process started earlier (will clear the FW internal context)

#define READ_IMAGE_CHUNK_SIZE ((UINT32)(8.5*1024.0))

#pragma pack(1)

typedef struct _OBJ_BOUNDS {
	unsigned int x;						// x position of object
	unsigned int y;						// y position of object
	unsigned int w;						// width position of object
	unsigned int h;						// height position of object
}OBJ_BOUNDS, *POBJ_BOUNDS;

typedef struct _METADATA_OBJECT {
	OBJ_BOUNDS objBounds;
	unsigned char  objShape;
	unsigned char  objActionEffect;
	unsigned char  objActionType;		// if not ACTION_EFFECT_TYPE_NONE and objActionBoundsCount is 0
										// then action render bounds is taken to be same as objBounds
	unsigned char  objActionDraw;
	unsigned short objActionCustom;		// custom field 
	unsigned short objActionIndex;		// index into the ACTION_EFFECT_LIST
	unsigned short objActionBoundsCount;// 0 - n if objShape not OBJ_SHAPE_POLYGON then should be 0 or 1
}METADATA_OBJECT, *PMETADATA_OBJECT;

//
// Request message for SUB_COMM_GET_IMAGE_CHUNK
//
typedef struct _GET_IMAGE_CHUNK_MSG
{
	unsigned char  subCommand; // SUB_COMM_GET_IMAGE_CHUNK
	INT_BYTES      handle;     // the handle to the rendered image, previously returned in CREATE_WINDOW_RESPONSE_MSG
	INT_BYTES      size;       // the size of the chunk to read
} GET_IMAGE_CHUNK_MSG;

/**********************************************************************************************//**
 * 	Values that represent WysImageType. 
 **************************************************************************************************/
enum WysImageType{
	WysStandardImagePinPad	= 1,				// pinpad for user input pin
	WysStandardImageOK		= 2,				// ok button to test user presence
	WysStandardImageCaptcha = 3,				// captcha to validate user input
	WysCustomImage			= 4					// custom wys image e.g. remote wys mode
};

/**********************************************************************************************//**
 * 	Values that represent WysImageObjActionRenderType. 
 **************************************************************************************************/
enum WysImageObjActionRenderType{
	RenderTypeNone		= 0,					// no rendering on user action
	StandardWys			= 1,					// render default on user action
	CustomFullImage		= 2,					// overlay full image over render area
	CustomAlphaImage	= 3						// overlay an alpha image over render area
};


class WysImage {
public:
	unsigned int					m_wysImageId;
	WysImageType					m_wysImageType;
	RECT							m_wysImgRenderRect;
	void							*m_pDecoderRenderTargets;
	XYPAIR_VECTOR					m_clickRecords; 
	ELEMENTID_VECTOR				m_elementIDs;
	METADATA_OBJECT					*m_pMetadataObjs;
	UINT32							m_actionObjColor;
	WysImageObjActionRenderType		m_wysObjActionRenderType;
	XY_PAIR_BYTES					m_WysButtonDown;
	XY_PAIR_BYTES					m_WysButtonClick;
	int								m_nWysButtonDown;
	int								m_nMetadataObjs;
	bool							m_bValid;
	UINT8							*m_pMetaClicked;
	bool							m_bUniqueMetaClick;
	WYSIMAGESTATS					m_stats;
	WYSIMAGECONFIG					m_WysImgConfig;

	WysImage( unsigned int id, RECT size, void *pDecoderRenderTargets, WYSIMAGECONFIG *wysImgConfig = NULL,
		WysImageType imgType = WysCustomImage, WysImageObjActionRenderType wysObjActionRenderType = RenderTypeNone,
		unsigned int actionObjColor = 0)
	{ 
		m_wysImageId				= id;
		m_wysImgRenderRect.left		= size.left;
		m_wysImgRenderRect.top		= size.top;
		m_wysImgRenderRect.right	= size.left + size.right;
		m_wysImgRenderRect.bottom	= size.top + size.bottom;
		m_pDecoderRenderTargets		= pDecoderRenderTargets;
		m_wysImageType				= imgType;
		m_wysObjActionRenderType	= wysObjActionRenderType;
		m_actionObjColor			= actionObjColor; 
		m_nMetadataObjs				= 0;
		m_pMetadataObjs				= NULL;
		m_bValid					= true;
		m_nWysButtonDown			= WYS_INVALID_WYS_BUTTON;
		m_stats.totalClicks			= 0;
		m_stats.numMetaClicks		= 0;
		m_pMetaClicked				= NULL;
		m_bUniqueMetaClick			= false;
		m_WysButtonDown.xwyh		= 0;
		m_WysButtonClick.xwyh		= 0;
		m_WysImgConfig				= *wysImgConfig;
	}
	
	~WysImage() 
	{ 
		m_clickRecords.clear();
		m_elementIDs.clear();
		if( m_pMetadataObjs != NULL ) { delete[] m_pMetadataObjs;	m_pMetadataObjs = NULL; }
		if( m_pMetaClicked	!= NULL ) { delete[] m_pMetaClicked;	m_pMetaClicked	= NULL; }
	}
	
	bool init(int nMetaObjs, METADATA_OBJECT *pmetaObjs)
	{
		bool bRet = true;

		if( nMetaObjs > 0 ) {
			m_pMetadataObjs = new METADATA_OBJECT[nMetaObjs];
			if( m_pMetadataObjs != NULL ) {
				memcpy_s( m_pMetadataObjs, nMetaObjs*sizeof(METADATA_OBJECT), pmetaObjs, nMetaObjs*sizeof(METADATA_OBJECT) );
				m_pMetaClicked = new UINT8[ nMetaObjs ];
				if( m_pMetaClicked != NULL ) {
					memset( m_pMetaClicked, 0, nMetaObjs );
					m_nMetadataObjs = nMetaObjs;
				} else {
					delete [] m_pMetadataObjs;
					m_pMetadataObjs = NULL;
					bRet = false;
				}
			} else {
				bRet = false;
			}
		}

		return bRet;
	}

	UINT32 getMetaClicks	() { UINT32 n = 0; if( m_pMetaClicked != NULL ) for( int i=0; i<m_nMetadataObjs; i++ ) n += (m_pMetaClicked[i] > 0 ? 1: 0); return n; }
	WYSIMAGESTATS getStats	() { m_stats.numMetaClicks = getMetaClicks(); return m_stats; }
	void ClearClickRecords	() { m_clickRecords.clear(); m_stats.totalClicks = 0; m_stats.numMetaClicks = 0; memset( m_pMetaClicked, 0, m_nMetadataObjs ); }
	void ClearElementIDs	(UINT32 n) { m_elementIDs.erase( m_elementIDs.begin(), m_elementIDs.begin() + n ); }
};

typedef unordered_map<UINT32, WysImage*> WYSIMAGEOBJS_HASH_MAP;