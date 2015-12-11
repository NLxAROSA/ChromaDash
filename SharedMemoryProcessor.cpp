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
#include "SharedMemoryProcessor.h"
#include "SharedMemory.h"
#include <cstdio>
#include "ChromaKeyboardRenderer.h"

#define MAP_OBJECT_NAME "$pcars$"

ChromaKeyboardRenderer* renderer;

SharedMemoryProcessor::SharedMemoryProcessor()	{
	renderer = new ChromaKeyboardRenderer();
}

SharedMemoryProcessor::~SharedMemoryProcessor()	{
	delete renderer;
}

void logMessage(bool shouldLog, const char* message)	{
	if (shouldLog){
		printf(message);
	}
}

// Processes the shared memory
void processSharedMemoryData(const SharedMemory* sharedData, bool shouldLog) {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)
	{
		// Version conflict, log an error
		logMessage(shouldLog, "ERROR: Data version mismatch, please make sure that your pCARS version matches your ChromaDash version\n");
	}else {
		// Got valid data, do some processing
		renderer->render(sharedData->mRpm, sharedData->mMaxRPM, sharedData->mGear);
	}
}

// Processes the memory mapped file
void processFile(HANDLE fileHandle, bool shouldLog) {

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));

	if (sharedData == NULL) {
		// File found, but could not be mapped to shared memory data
		logMessage(shouldLog, "ERROR: No data available to process\n");
	}else {
		// Process file
		processSharedMemoryData(sharedData, shouldLog);
		// Unmap file
		UnmapViewOfFile(sharedData);
	}

}

void SharedMemoryProcessor::process(bool shouldLog) {
	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMappingA(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);

	if (fileHandle == NULL) {
		// File is not available
		logMessage(shouldLog, "ERROR: No data available to process, is Project CARS running and Shared Memory enabled?\n");
	}else {
		// File is available, process the file
		processFile(fileHandle, shouldLog);
		// Close the file
		CloseHandle(fileHandle);
	}
}