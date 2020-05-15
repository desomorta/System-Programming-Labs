// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

BOOL ReadTestFile(const char* filePath, const char* message) {
	std::cout << "DLL - ReadTestFile - Start" << std::endl;

	OVERLAPPED overlappedStructure;
	overlappedStructure.Offset = NULL;
	overlappedStructure.OffsetHigh = NULL;
	overlappedStructure.hEvent = NULL;
	HANDLE readFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	ReadFile(readFile, (LPVOID)message, 80, NULL, &overlappedStructure);
	WaitForSingleObject(readFile, INFINITE);
	CloseHandle(readFile);

	std::cout << "DLL - ReadTestFile - Finish" << std::endl;
	return TRUE;
}

BOOL WriteTestFile(const char* filePath, const char* message, unsigned int size, unsigned int offset) {
	std::cout << "DLL - WriteTestFile - Start" << std::endl;

	OVERLAPPED overlappedStructure;
	overlappedStructure.Offset = offset;
	overlappedStructure.OffsetHigh = NULL;
	overlappedStructure.hEvent = NULL;
	HANDLE writeFile = CreateFileA(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	WriteFile(writeFile, message, size, NULL, &overlappedStructure);
	WaitForSingleObject(writeFile, INFINITE);
	CloseHandle(writeFile);

	std::cout << "DLL - WriteTestFile - Finish" << std::endl;
	std::cout << "=============================" << std::endl;
	return TRUE;
}