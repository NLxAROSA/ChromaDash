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
#include "ChromaKeyboard.h"

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

ChromaKeyboard::ChromaKeyboard()	{
	if (!setup())	{
		printf("ERROR: Unable to initialize Chroma SDK library: is the latest Synapse installed?\n");
	}
}

ChromaKeyboard::~ChromaKeyboard()	{
	teardown();
}

BOOL ChromaKeyboard::setup()	{
	
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

BOOL ChromaKeyboard::teardown()	{

	if (m_ChromaSDKModule != NULL)	{
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

	// Only light up keys if there are actually any RPMs to display
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
	// Only display gears 1 through 9 and neutral (0)
	if (gear >= -1 && gear < 10) {

		switch (gear) {
		case -1: // Reverse
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD_SUBTRACT)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD_SUBTRACT)] = YELLOW;
			break;
		case 1: // First gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD1)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD1)] = YELLOW;
			break;
		case 2: // Second gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD2)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD2)] = YELLOW;
			break;
		case 3: // Third gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD3)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD3)] = YELLOW;
			break;
		case 4: // Fourth gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD4)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD4)] = YELLOW;
			break;
		case 5: // Fifth gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD5)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD5)] = YELLOW;
			break;
		case 6: // Sixth gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD6)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD6)] = YELLOW;
			break;
		case 7: // Seventh gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD7)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD7)] = YELLOW;
			break;
		case 8: // Eighth gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD8)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD8)] = YELLOW;
			break;
		case 9: // Ninth gear
			Effect->Color[HIBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD9)][LOBYTE(ChromaSDK::Keyboard::RZKEY_NUMPAD9)] = YELLOW;
			break;
		}
	}

}

void renderTyreWear(float tyreWear[4], ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{

	COLORREF frontLeft = RGB((tyreWear[0] * 255), ((1.0 - tyreWear[0]) * 255), 0);
	COLORREF frontRight = RGB((tyreWear[1] * 255), ((1.0 - tyreWear[1]) * 255), 0);
	COLORREF rearLeft = RGB((tyreWear[2] * 255), ((1.0 - tyreWear[2]) * 255), 0);
	COLORREF rearRight = RGB((tyreWear[3] * 255), ((1.0 - tyreWear[3]) * 255), 0);

	UINT frontLeftLitKeys = 1 + UINT((1.0 - tyreWear[0]) * 4.0);
	UINT frontRightLitKeys = 1 + UINT((1.0 - tyreWear[1]) * 4.0);
	UINT rearLeftLitKeys = 1 + UINT((1.0 - tyreWear[2]) * 4.0);
	UINT rearRightLitKeys = 1 + UINT((1.0 - tyreWear[3]) * 4.0);

	frontLeftLitKeys = frontLeftLitKeys > 4 ? 4 : frontLeftLitKeys;
	frontRightLitKeys = frontRightLitKeys > 4 ? 4 : frontRightLitKeys;
	rearLeftLitKeys = rearLeftLitKeys > 4 ? 4 : rearLeftLitKeys;
	rearRightLitKeys = rearRightLitKeys > 4 ? 4 : rearRightLitKeys;

	for (UINT i = 0; i < frontLeftLitKeys; i++) {
		Effect->Color[2][2 + i] = frontLeft;
	}

	for (UINT i = 0; i < frontRightLitKeys; i++) {
		Effect->Color[2][7 + i] = frontRight;
	}

	for (UINT i = 0; i < rearLeftLitKeys; i++) {
		Effect->Color[3][2 + i] = rearLeft;
	}

	for (UINT i = 0; i < rearRightLitKeys; i++) {
		Effect->Color[3][7 + i] = rearRight;
	}

}

void ChromaKeyboard::display(float rpm, float maxRpm, int gear, float tyreWear[4])	{

	if (CreateKeyboardEffect) {

		RZRESULT Result = RZRESULT_INVALID;
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect;
		// Initialize the array by setting to black.
		ZeroMemory(&Effect, sizeof(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE));

		if (rpm > 0)	{
			// Only render if engine is running
			renderRpm(rpm, maxRpm, &Effect);
			renderGear(gear, &Effect);
			renderTyreWear(tyreWear, &Effect);
		}

		Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}