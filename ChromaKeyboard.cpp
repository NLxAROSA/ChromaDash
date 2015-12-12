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

#define MAX_TYRES 4

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

// Displays the RPM as a meter on the function key row
void displayRpm(float rpm, float maxRpm, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{

	UINT currentRpm = UINT(rpm / maxRpm * 12.0);

	for (UINT i = 0; i < currentRpm; i++)	{
		Effect->Color[0][3 + i] = RGB(((i / 12.0) * 255), (100 - (i / 12.0) * 100), 0);
	}

}

// Displays the selected gear on the numerical keypad
void displayGear(int gear, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{
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

// Displays a tire wear bar for a single tire
void displayTyre(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect, int tyre, COLORREF tyreColor, UINT i)	{
	int row = 0;
	int column = 0;

	switch (tyre)	{
			
	case 0:
		row = 2;
		column = 2;
		break;
	case 1:
		row = 2;
		column = 7;
		break;
	case 2:
		row = 3;
		column = 2;
		break;
	case 3:
		row = 3;
		column = 7;
		break;

	}

	Effect->Color[row][column + i] = tyreColor;
}

// Displays tyre wear bar for a single tyre on the keyboard
void displayTyre(float tyreWear, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect, int tyre)	{

	COLORREF tyreColor = RGB((tyreWear * 255), ((1.0 - tyreWear) * 255), 0);

	/*
		4 keys lit = 75-100% thread left
		3 keys lit = 50-75% thread left
		2 keys lit = 25-50% thread left
		1 key lit = 0-25% thread left
		0 keys lit = thread is gone

		Note that in Project CARS you can still drive on the carcass after wearing out the thread. 
	*/

	UINT numberOfLitKeys = 1 + UINT((1.0 - tyreWear) * 4.0);
	numberOfLitKeys = numberOfLitKeys > 4 ? 4 : numberOfLitKeys;

	if (tyreWear >= 1.0f)
	{
		numberOfLitKeys = 4;
	}

	for (UINT i = 0; i < numberOfLitKeys; i++) {
		displayTyre(Effect, tyre, tyreColor, i);
	}
}

// Displays tyre wear on the keyboard
void displayTyreWear(float tyreWear[MAX_TYRES], ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{

	for (int i = 0; i < MAX_TYRES; i++)	{
		displayTyre(tyreWear[i], Effect, i);
	}

}

// Display the requested data on the Chroma keyboard
void ChromaKeyboard::display(float rpm, float maxRpm, int gear, float tyreWear[MAX_TYRES])	{

	if (CreateKeyboardEffect) {

		RZRESULT Result = RZRESULT_INVALID;
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect;
		// Initialize the array by setting to black.
		ZeroMemory(&Effect, sizeof(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE));

		if (rpm > 0)	{
			// Only display if engine is running, black out otherwise
			displayRpm(rpm, maxRpm, &Effect);
			displayGear(gear, &Effect);
			displayTyreWear(tyreWear, &Effect);
		}

		Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}