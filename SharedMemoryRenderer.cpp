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
#include "SharedMemoryRenderer.h"
#include "SharedMemory.h"
#include <cstdio>
#include "ChromaKeyboardRenderer.h"

#define MAP_OBJECT_NAME "$pcars$"

ChromaKeyboardRenderer* renderer;
bool loggingEnabled = false;

void SharedMemoryRenderer::enableLogging()	{
	loggingEnabled = true;
}

void SharedMemoryRenderer::disableLogging() {
	loggingEnabled = false;
}

SharedMemoryRenderer::SharedMemoryRenderer()	{
	renderer = new ChromaKeyboardRenderer();
}

SharedMemoryRenderer::~SharedMemoryRenderer()	{
	delete renderer;
}

void logMessage(const char* message)	{
	if (loggingEnabled){
		printf(message);
	}
}

// Processes the shared memory
void processSharedMemoryData(const SharedMemory* sharedData) {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)
	{
		// Version conflict, log an error
		logMessage("ERROR: Data version mismatch, please make sure that your pCARS version matches your ChromaDash version\n");
	}else {
		// Got valid data, do some processing
		renderer->render(sharedData->mRpm, sharedData->mMaxRPM, sharedData->mGear);
	}
}

// Processes the memory mapped file
void processFile(HANDLE fileHandle) {

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));

	if (sharedData == NULL) {
		// File found, but could not be mapped to shared memory data
		logMessage("ERROR: No data available to process\n");
	}else {
		// Process file
		processSharedMemoryData(sharedData);
		// Unmap file
		UnmapViewOfFile(sharedData);
	}

}

void SharedMemoryRenderer::process() {
	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMappingA(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);

	if (fileHandle == NULL) {
		// File is not available
		logMessage("ERROR: No data available to process, is Project CARS running and Shared Memory enabled?\n");
	}else {
		// File is available, process the file
		processFile(fileHandle);
		// Close the file when done
		CloseHandle(fileHandle);
	}
}