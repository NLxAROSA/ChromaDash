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
#include <stdio.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include "SharedMemoryRenderer.h"

// Configuration properties
#define CHROMADASH_VERSION "v0.1.0"
#define POLL_TIME_IN_MILLIS 17L
#define ESC_KEY 27

SharedMemoryRenderer sharedMemoryRenderer;

void createSharedMemoryRenderer()	{
	// Process shared memory and log any errors
	sharedMemoryRenderer.enableLogging();
	sharedMemoryRenderer.render();
	// No more logging after initial setup
	sharedMemoryRenderer.disableLogging();
}

int main()	{
	printf("# CHROMADASH - PROJECT CARS RAZER CHROMA DASHBOARD %s\n", CHROMADASH_VERSION);
	printf("# (c) 2015 Lars Rosenquist\n\n");
	printf("# Press ESC to terminate\n\n");
	createSharedMemoryRenderer();
    
	// Keep polling until ESC is hit
	while (true)	{
		// Process shared memory
		sharedMemoryRenderer.render();

		// Check on exit criteria
		if (_kbhit() && _getch() == ESC_KEY)	{
			break;
		}

		// Be nice and don't hog the CPU
		std::this_thread::sleep_for(std::chrono::milliseconds(POLL_TIME_IN_MILLIS));
	}

	// We're done, exit
	return 0;
}