// Dependencies
#include <stdio.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include "SharedMemoryProcessor.h"

// Configuration properties
#define CHROMADASH_VERSION "v0.1.0"
#define POLL_TIME_IN_MILLIS 17L
#define ESC_KEY 27

int main()	{

	// Print some information on the console
	printf("# CHROMADASH - PROJECT CARS RAZER CHROMA DASHBOARD %s\n", CHROMADASH_VERSION);
	printf("# (c) 2015 Lars Rosenquist\n\n");
	printf("# Press ESC to terminate\n\n");

	SharedMemoryProcessor sharedMemoryProcessor = SharedMemoryProcessor();
    
	// Process shared memory, print any errors
	sharedMemoryProcessor.process(true);

	// Keep polling until ESC is hit
	while (true)	{
		// Process shared memory, don't print any errors at ~59FPS ;)
		sharedMemoryProcessor.process(false);

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