/*

Copyright 2015 L.S. Rosenquist

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
#pragma once
#include "stdafx.h"
#include "ChromaKeyboardRenderer.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;
HMODULE m_ChromaSDKModule;

ChromaKeyboardRenderer::ChromaKeyboardRenderer()	{
	if (Initialize())	{
		printf("INFO: Successfully initialized Chroma SDK library\n");
	}else{
		printf("ERROR: Unable to initialize Chroma SDK library: is the latest Synapse installed?\n");
	}
}

ChromaKeyboardRenderer::~ChromaKeyboardRenderer()	{
	UnInitialize();
}

BOOL ChromaKeyboardRenderer::Initialize()	{
	
	if (m_ChromaSDKModule == NULL)	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == NULL)	{
			ASSERT(GetLastError() == ERROR_SUCCESS);
			return FALSE;
		}
	}

	if (Init == NULL)	{
		RZRESULT Result = RZRESULT_INVALID;
		Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
		if (Init)	{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)	{
				CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
				CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
				SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
				DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
				QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");

				if (CreateEffect &&
					CreateKeyboardEffect &&
					SetEffect &&
					DeleteEffect &&
					QueryDevice)	{
					return TRUE;
				}else{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL ChromaKeyboardRenderer::UnInitialize()	{

	if (m_ChromaSDKModule != NULL)
	{
		RZRESULT Result = RZRESULT_INVALID;
		UNINIT UnInit = (UNINIT)GetProcAddress(m_ChromaSDKModule, "UnInit");
		if (UnInit)	{
			Result = UnInit();
			ASSERT(Result == RZRESULT_SUCCESS);
		}

		FreeLibrary(m_ChromaSDKModule);
		m_ChromaSDKModule = NULL;

		return TRUE;
	}

	return FALSE;
}

void renderRpm(float rpm, float maxRpm, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{

	// RPMs can go beyond limiter in case of engine failure, but meter shouldn't.
	if (rpm > maxRpm)	{
		rpm = maxRpm;
	}

	// Only light up keys if there are actually any RPMs
	if (maxRpm > 0 && rpm > 0) {
		// Function keys as RPM
		UINT currentRpm = UINT(rpm / maxRpm * 12.0); // 12 function keys

		for (UINT i = 0; i < currentRpm; i++)	{
			// F1 key starts from row 0, column 3 to column column 15
			Effect->Color[0][3 + i] = RGB(((i / 12.0) * 255), (100 - (i / 12.0) * 100), 0);
		}
	}

}

void renderGear(int gear, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{
	// Only render gears 1 through 9
	if (gear > 0 && gear < 10) {
		Effect->Color[1][1 + gear] = YELLOW;
	}

}

void ChromaKeyboardRenderer::render(float rpm, float maxRpm, int gear)	{

	if (CreateKeyboardEffect) {

		RZRESULT Result = RZRESULT_INVALID;
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect;
		// Initialize the array by setting to black.
		ZeroMemory(&Effect, sizeof(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE));

		renderRpm(rpm, maxRpm, &Effect);
		renderGear(gear, &Effect);

		Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}