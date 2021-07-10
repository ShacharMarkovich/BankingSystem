/*
//               INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2010 Intel Corporation. All Rights Reserved.
//
*/

// 
// A class to abstract the use of a PAVP device.
// This class internally uses the Intel Proprietary Auxiliary Device
// API/DDI definitions. 
// 
// This class inherits from the CAuxiliaryDevice class
//
#pragma once
#ifndef __d3d11_h__
#include <d3d11.h> 
#endif 
#ifndef _D3D9_H_
#include <d3d9.h>
#endif 

#include "intel_auxiliary_device.h"
#include "intel_pavp_api.h"
#include "le2be_macros.h"

class CPavpDevice;

typedef HRESULT (__thiscall CPavpDevice::*pInitializeDevice)		(IDirect3DDevice9 *pD3DDevice9);
typedef HRESULT (__thiscall CPavpDevice::*pInitializeDX11Device)	(ID3D11Device *pD3D11Device);
typedef HRESULT (__thiscall CPavpDevice::*pUninitializeDevice)		(); 
typedef HRESULT (__thiscall CPavpDevice::*pExecute)					(UINT uiFuncID, void *pDataIn, UINT uiSizeIn, void *pOutputData, UINT uiSizeOut);
typedef HRESULT (__thiscall CPavpDevice::*pDestroyAccelService)		(const GUID *pAccelGuid, void *pDestroyParams);
typedef HRESULT (__thiscall CPavpDevice::*pCheckForPavpGuid)		();

typedef enum 
{
    KEY_EXCHANGE_ENVIRONMENT_SIMULATION = 0,
    KEY_EXCHANGE_ENVIRONMENT_EMULATION = 1,
    KEY_EXCHANGE_ENVIRONMENT_POST_SILICON = 2
}E_KEY_EXCHANGE_ENVIRONMENT;

typedef enum
{
    e_Pavp = 0,
    e_GpuCp
} E_PAVPAPI;

typedef enum
{
    e_LiteMode = 0,
    e_HeavyMode = 1,
	e_IDMode = 2
} E_PAVPMODE;

typedef struct _GetMeSlotHandleIn 
{
   PAVPCmdHdr     Header;
} GetMeSlotHandleIn;

typedef struct _GetMeSlotHandleOut 
{
   PAVPCmdHdr     Header;
   UINT32         SlotHandle;
} GetMeSlotHandleOut;

typedef struct _InternalDevicePointers
{
    pInitializeDevice       pInitializeDeviceFunc;
    pInitializeDX11Device   pInitializeDX11DeviceFunc;
    pDestroyAccelService    pDestroyAccelServiceFunc;
    pExecute                pExecuteFunc;
    pCheckForPavpGuid       pCheckForPavpGuidFunc;

} InternalDevicePointers;

class CPavpDevice : CAuxiliaryDevice
{
public:
    CPavpDevice();
    ~CPavpDevice();
    
    void            CleanUp();    
    PavpEpidStatus InitializeDevice     (IDirect3DDevice9 *pD3DDevice9, 
							IDirect3DCryptoSession9* pCryptoSession = NULL, 
							E_PAVPAPI ePavpApi = e_Pavp);
    PavpEpidStatus InitializeDX11Device (ID3D11Device *pD3D11Device, 
							ID3D11DeviceContext *pD3D11DeviceContext,
							ID3D11VideoContext* pD3D11VideoContext, 
							ID3D11CryptoSession* pCryptoSession = NULL, 
							E_PAVPAPI ePavpApi = e_Pavp);
	PavpEpidStatus InitializeDeviceWYS  (IDirect3DDevice9 *pD3DDevice9, 
							IDirect3DCryptoSession9* pCryptoSession = NULL, 
							E_PAVPAPI ePavpApi = e_Pavp);
	PavpEpidStatus UninitializeDevice   ();
    PavpEpidStatus CreatePavpService    (PAVP_KEY_EXCHANGE_MASK eKeyExchangeProtocol, 
                                            StreamKey const * const pEncryptedFixedKey, 
                                            StreamKey const * const pFixedKey,
                                            StreamKey const * const pStreamKey) 
    {
        return CreatePavpService        (eKeyExchangeProtocol, PAVP_SESSION_TYPE_DECODE, 
                                            pEncryptedFixedKey, pFixedKey, pStreamKey);
    }
    PavpEpidStatus CreatePavpService    (PAVP_KEY_EXCHANGE_MASK eKeyExchangeProtocol, 
                                            PAVP_SESSION_TYPE eSessionType = PAVP_SESSION_TYPE_DECODE,
                                            StreamKey const * const pEncryptedFixedKey = NULL, 
                                            StreamKey const * const pFixedKey = NULL,
                                            StreamKey const * const pStreamKey = NULL);
    // To support Transcode for Legacy option
    PavpEpidStatus CreatePavpLegacyService    (PAVP_KEY_EXCHANGE_MASK eKeyExchangeProtocol, 
                                            PAVP_SESSION_TYPE eSessionType = PAVP_SESSION_TYPE_DECODE,
                                            StreamKey const * const pEncryptedFixedKey = NULL, 
                                            StreamKey const * const pFixedKey = NULL,
                                            StreamKey const * const pStreamKey = NULL);
	PavpEpidStatus DoKeyExchange        (bool bGetAudioKey = FALSE, 
                                            PAVPSessId AudioStreamId = 0,
											UINT eMemProtType = 0, 
											GetCapsOutBuff* pCapsOut = 0);

    E_KEY_EXCHANGE_ENVIRONMENT      GetKeyExchangeEnvironment();
	void		                    SetKeyExchangeEnvironment(E_KEY_EXCHANGE_ENVIRONMENT eKeyExchangeEnvironment);

#if PRE_SILICON_ENABLED == 1
	PavpEpidStatus                  PreSiKeyExchange(UINT eMemProtType);
#endif

    PavpEpidStatus RefreshKey           (bool bGetNewKey, 
                                              bool bEncodeKey = FALSE,
                                              PAVPPathId PathId = PAVP_VIDEO_PATH);
    DEVICE_STATE   GetDeviceState       ();
    PavpEpidStatus GetPavpCaps          (PAVP_QUERY_CAPS2 *pCaps);
#if LEGACY_CONNECTION_STATE == 1
    PavpEpidStatus GetConnectionState   (PAVP_GET_CONNECTION_STATE_PARAMS *pConnStateParams, DWORD dwNonce);
#endif // LEGACY_CONNECTION_STATE == 1
    PavpEpidStatus SetNewKey            (PAVP_SET_KEY_TYPE StreamType, StreamKey const * const pNewKey);
//    PavpEpidStatus SetProtectedMemory   (BOOL bUseProtectedMemory);
#if LEGACY_PLANE_ENABLE == 1
    PavpEpidStatus SetPlaneEnable       (PAVP_PLANE_ENABLES * const pSetPlaneEnable);
#endif
    PavpEpidStatus SetWindowPosition    (RECT rWindowPosition, RECT rContent, HWND hWnd);
    PavpEpidStatus GetPavpHandle		(unsigned int* pHandle);
    PavpEpidStatus GetPchCaps			(GetCapsOutBuff* pCapsOut);
    // Supports a little endian 32-bit counter (for backwards compatibility):
    PavpEpidStatus EncryptContent       (unsigned char* pPlainInput, int iPlainSize, 
                                              unsigned char* pCipherOutput, int iEncryptedSize,
                                              unsigned int dwCounter, PAVPPathId PathId = PAVP_VIDEO_PATH)
    {
        DWORD dwTmpCtr[4] = {0, 0, 0, dwCounter};
        return EncryptContentLe(pPlainInput, iPlainSize, pCipherOutput, iEncryptedSize, dwTmpCtr, PAVP_COUNTER_TYPE_A, PAVP_ENCRYPTION_AES128_CTR, PathId);
    }
    // Supports up to a 128-bit little endian counter
    // When setting this counter during decode, the app should convert to big endian before passing to the driver. 
    PavpEpidStatus EncryptContentLe     (unsigned char* pPlainInput, int iPlainSize, 
                                          unsigned char* pCipherOutput, int iEncryptedSize,
                                          DWORD dwAesCtr[4], PAVP_COUNTER_TYPE eCtrType = PAVP_COUNTER_TYPE_A,
                                          PAVP_ENCRYPTION_TYPE eEncryptionType = PAVP_ENCRYPTION_AES128_CTR,
                                          PAVPPathId PathId = PAVP_VIDEO_PATH)
    {
        DWORD dwTmpCtr[4] = {0, 0, 0, 0};
        if( NULL == dwAesCtr )
            return PAVP_STATUS_INVALID_PARAMS;
        memcpy_s(dwTmpCtr, sizeof(DWORD)*4, dwAesCtr, sizeof(DWORD)*4);
        if( PathId == PAVP_VIDEO_PATH )
        {
            if( !CounterEndianSwap(dwTmpCtr, eCtrType) )
                return PAVP_STATUS_INVALID_PARAMS;
        }
        return EncryptContent(pPlainInput, iPlainSize, pCipherOutput, iEncryptedSize, dwTmpCtr, eCtrType, eEncryptionType, PathId);
    }
    // Takes a 128-bit big endian counter
    PavpEpidStatus EncryptContent       (unsigned char* pPlainInput, int iPlainSize, 
                                          unsigned char* pCipherOutput, int iEncryptedSize,
                                          DWORD dwAesCtr[4], PAVP_COUNTER_TYPE eCtrType = PAVP_COUNTER_TYPE_A,
                                          PAVP_ENCRYPTION_TYPE eEncryptionType = PAVP_ENCRYPTION_AES128_CTR,
                                          PAVPPathId PathId = PAVP_VIDEO_PATH);
    PavpEpidStatus DecryptContentLe     (unsigned char* pCipherInput, int iEncryptedSize, 
                                          unsigned char* pPlainOutput, int iPlainSize, 
                                          DWORD dwAesCtr[4], PAVP_COUNTER_TYPE eCtrType = PAVP_COUNTER_TYPE_A,
                                          PAVP_ENCRYPTION_TYPE eEncryptionType = PAVP_ENCRYPTION_AES128_CTR)
    {
        DWORD dwTmpCtr[4] = {0, 0, 0, 0};
        if( NULL == dwAesCtr )
            return PAVP_STATUS_INVALID_PARAMS;
        memcpy_s(dwTmpCtr, sizeof(DWORD)*4, dwAesCtr, sizeof(DWORD)*4);
        if( !CounterEndianSwap(dwTmpCtr, eCtrType) )
            return PAVP_STATUS_INVALID_PARAMS;
        return DecryptContent(pCipherInput, iEncryptedSize, pPlainOutput, iPlainSize, dwTmpCtr, eCtrType, eEncryptionType);
    }
    PavpEpidStatus DecryptContent       (unsigned char* pCipherInput, int iEncryptedSize, 
                                          unsigned char* pPlainOutput, int iPlainSize, 
                                          DWORD dwAesCtr[4], PAVP_COUNTER_TYPE eCtrType = PAVP_COUNTER_TYPE_A,
                                          PAVP_ENCRYPTION_TYPE eEncryptionType = PAVP_ENCRYPTION_AES128_CTR);
    static BOOL    CounterEndianSwap    (DWORD dwCounter[4], PAVP_COUNTER_TYPE eCtrType);

    enum E_KeyId {
        keyIdDecode,
        keyIdEncode,
        keyIdAudioDecode,
    };
    PavpEpidStatus GetKey(E_KeyId id, void* buf, unsigned int &size);
    PavpEpidStatus ForceHeavyModeEarly(UINT ApiVersion);
	BOOL           m_bKeysExchanged;
	StreamKey      m_StreamKey;  // "Stream" key that would encrypt actual video
protected:
    InternalDevicePointers m_fnptrs;
    
private:
	PavpEpidStatus  EpidKeyExchange(bool bGetAudioKey = FALSE, PAVPSessId AudioStreamId = 0, UINT eMemProtType = 0, GetCapsOutBuff* pCapsOut = 0);
    PavpEpidStatus  LegacyKeyExchange       ();
    PavpEpidStatus  UpdateEncryptionKey     (PAVPPathId PathId);
    PavpEpidStatus  InjectKeyFreshness      (bool bEncodeKey);  
    HRESULT         CheckForPavpGuid        ();
    BOOL            CheckKeyExchange        (PAVP_KEY_EXCHANGE_MASK eKeyExchangeProtocol);
    HRESULT         InvalidateEncryptionKey (PAVPPathId PathId);
    void            ResetAllKeys            ();
    HRESULT         CleanupEpid             ();
    HRESULT         NegotiateKeyExchange    (UINT uiFuncID, void *pDataIn, UINT uiSizeIn, void *pOutputData, UINT uiSizeOut);
    HRESULT         DummyDestroyAccelService(const GUID *pAccelGuid, void *pDestroyParams);
    HRESULT         GpuCpInitializeDevice   (IDirect3DDevice9 *pD3DDevice9);
    HRESULT         DummyCheckForPavpGuid   ();

    
    BOOL                        m_bSessionIsWysiwys;
    DWORD                       m_dwApiVersion;
    UINT                        eProtType;
    E_PAVPAPI                   m_ApiInUse;
    E_KEY_EXCHANGE_ENVIRONMENT  m_eKeyExchangeEnvironment;
    PAVP_SESSION_TYPE           m_eSessionType;
    PAVP_KEY_EXCHANGE_MASK      m_eExchangeProtocol;    // The key exchange protocol in use
    PAVP_ENCRYPTION_TYPE        m_eAudioEncryptMode;    // AES type to use for audio
    PAVP_ENDIANNESS_TYPE        m_eAudioEndianessType;  // AES counter endianness type for audio
    SigmaSessionKey             m_SessionKey;           // "Session" key used to wrap stream keys
    StreamKey                   m_EncryptedFixedKey;    // For the legacy key exchange
    StreamKey                   m_FixedKey;             // For the legacy key exchange
              
    StreamKey                   m_EncodeKey;            // "Encrypted Encode" key that would decrypt securely encoded video
    StreamKey                   m_AudioKey;             // Stream key for the audio stream
    StreamKey                   m_ConnStateKey;         // Key to decrypt connection state if different key is required
    SigmaMacKey                 m_SigningKey;           // HMAC signing key
    PAVPSessId                  m_SigmaSessionId;	
    PAVPSessId                  m_AudioStreamId;
    IDirect3DCryptoSession9     *m_pDX9CryptoSession;
	
	ID3D11CryptoSession        *m_pDX11CryptoSession;
    ID3D11DeviceContext        *m_pDX11DeviceContext;          // the pointer to our Direct3D device context
    ID3D11VideoContext         *m_pDX11VideoContext;

	//make copy constructor private so it prevents inadvertant copying...
	CPavpDevice(const CPavpDevice &CurrentDevice) {};

};
