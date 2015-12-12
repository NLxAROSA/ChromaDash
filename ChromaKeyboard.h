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

*/#pragma once
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEMAT_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5

#define MAX_WHEELS 4

typedef enum WHEEL_INFO_TYPE {
	TYRE_WEAR = 0,
	BRAKE_DAMAGE = 1,
	SUSPENSION_DAMAGE = 2
} WHEEL_INFO_TYPE;

class ChromaKeyboard	{
public:
	ChromaKeyboard();
	~ChromaKeyboard();
	BOOL setup();
	BOOL teardown();
	void display(float rpm, 
		float maxRpm, 
		int gear, 
		const float tyreWear[MAX_WHEELS], 
		const float brakeDamage[MAX_WHEELS], 
		const float suspensionDamage[MAX_WHEELS], 
		const float aeroDamage, 
		const float engineDamage,
		bool engineWarning);
};
