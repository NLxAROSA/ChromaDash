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

// Determines where to display brake damage info for each wheel
ChromaSDK::Keyboard::RZKEY determineTyreKey(int wheel) {
	switch (wheel) {
	case 0:
		return ChromaSDK::Keyboard::RZKEY_Q;
	case 1:
		return ChromaSDK::Keyboard::RZKEY_W;
	case 2:
		return ChromaSDK::Keyboard::RZKEY_A;
	case 3:
		return ChromaSDK::Keyboard::RZKEY_S;
	}
	return ChromaSDK::Keyboard::RZKEY_SPACE; // Just in case there are more than 4 tyres ;)
}

// Determines where to display suspension damage info for each wheel
ChromaSDK::Keyboard::RZKEY determineSuspensionDamageKey(int wheel) {
	switch (wheel) {
	case 0:
		return ChromaSDK::Keyboard::RZKEY_R;
	case 1:
		return ChromaSDK::Keyboard::RZKEY_T;
	case 2:
		return ChromaSDK::Keyboard::RZKEY_F;
	case 3:
		return ChromaSDK::Keyboard::RZKEY_G;
	default: return ChromaSDK::Keyboard::RZKEY_SPACE; // Just in case there are more than 4 tyres ;)
	}
}

// Determines where to display tyre wear info for each wheel
ChromaSDK::Keyboard::RZKEY determineBrakeDamageKey(int wheel) {
	switch (wheel) {
	case 0:
		return ChromaSDK::Keyboard::RZKEY_U;
	case 1:
		return ChromaSDK::Keyboard::RZKEY_I;
	case 2:
		return ChromaSDK::Keyboard::RZKEY_J;
	case 3:
		return ChromaSDK::Keyboard::RZKEY_K;
	}
	return ChromaSDK::Keyboard::RZKEY_SPACE; // Just in case there are more than 4 tyres ;)
}

ChromaSDK::Keyboard::RZKEY determineWheelInfoKey(WHEEL_INFO_TYPE infoType, int wheel)	{
	switch(infoType)	{
		case TYRE_WEAR: return determineTyreKey(wheel);
		case BRAKE_DAMAGE: return determineBrakeDamageKey(wheel);
		case SUSPENSION_DAMAGE: return determineSuspensionDamageKey(wheel);
		default: return determineSuspensionDamageKey(wheel);
	}
}

// Displays a single tyre on the assigned key
void displayWheel(WHEEL_INFO_TYPE infoType, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect, float wheelInfo, int wheel){
	ChromaSDK::Keyboard::RZKEY key = determineWheelInfoKey(infoType, wheel);
	COLORREF color = RGB((wheelInfo * 255), ((1.0 - wheelInfo) * 255), 0);
	Effect->Color[HIBYTE(key)][LOBYTE(key)] = color;
}

// Displays wheel info per type
void displayWheelInfo(WHEEL_INFO_TYPE infoType, const float wheelInfo[MAX_WHEELS], ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{

	for (int i = 0; i < MAX_WHEELS; i++) {
		displayWheel(infoType, Effect, wheelInfo[i], i);
	}
}

void displayAeroDamage(float aeroDamage, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect)	{
	ChromaSDK::Keyboard::RZKEY key = ChromaSDK::Keyboard::RZKEY_HOME;
	COLORREF color = RGB((aeroDamage * 255), ((1.0 - aeroDamage) * 255), 0);
	Effect->Color[HIBYTE(key)][LOBYTE(key)] = color;
}

void displayEngineDamage(float engineDamage, ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect) {
	ChromaSDK::Keyboard::RZKEY key = ChromaSDK::Keyboard::RZKEY_INSERT;
	COLORREF color = RGB((engineDamage * 255), ((1.0 - engineDamage) * 255), 0);
	Effect->Color[HIBYTE(key)][LOBYTE(key)] = color;
}

void displayEngineWarning(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE* Effect) {
	ChromaSDK::Keyboard::RZKEY key = ChromaSDK::Keyboard::RZKEY_PAGEUP;
	Effect->Color[HIBYTE(key)][LOBYTE(key)] = RED;
}

// Display the requested data on the Chroma keyboard
void ChromaKeyboard::display(float rpm, 
	float maxRpm, 
	int gear, 
	const float tyreWear[MAX_WHEELS], 
	const float brakeDamage[MAX_WHEELS], 
	const float suspensionDamage[MAX_WHEELS], 
	const float aeroDamage,
	const float engineDamage,
	bool engineWarning)	{

	if (CreateKeyboardEffect) {

		RZRESULT Result = RZRESULT_INVALID;
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect;
		// Initialize the array by setting to black.
		ZeroMemory(&Effect, sizeof(ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE));

		if (rpm > 0)	{
			// Only display if engine is running, black out otherwise
			displayRpm(rpm, maxRpm, &Effect);
			displayGear(gear, &Effect);
			displayWheelInfo(TYRE_WEAR, tyreWear, &Effect);
			displayWheelInfo(BRAKE_DAMAGE, brakeDamage, &Effect);
			displayWheelInfo(SUSPENSION_DAMAGE, suspensionDamage, &Effect);
			displayEngineDamage(engineDamage, &Effect);
			displayAeroDamage(aeroDamage, &Effect);

			if (engineWarning)	{
				displayEngineWarning(&Effect);
			}
		}

		Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}