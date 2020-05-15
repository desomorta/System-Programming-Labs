// main.cpp

#include <iostream>
#include <Windows.h>
#include <string>

#define NUMBER_FILES 5

typedef BOOL(*readFunction)(const char*, const char*);
typedef BOOL(*writeFunction)(const char*, const char*, unsigned int, unsigned int);

HMODULE hModuleDLL;
CRITICAL_SECTION writeSection;
CRITICAL_SECTION readSection;
CRITICAL_SECTION deleteSection;

char message[80] = { '\0' };

DWORD WINAPI writeThread(LPVOID information) {
	std::string filePath = "D:/work/C++/3kurs/spovm/lab5/Debug/Result_File.txt";
	int size = 0;
	int offset = 0;

	writeFunction writeTestFile = reinterpret_cast<writeFunction>(GetProcAddress(hModuleDLL, "WriteTestFile"));

	while (!TryEnterCriticalSection(&deleteSection)) {
		EnterCriticalSection(&writeSection);

		offset = strlen(static_cast<char*>(information));
		writeTestFile(filePath.c_str(), static_cast<const char*>(information), offset, size);
		size += offset;

		LeaveCriticalSection(&writeSection);
		Sleep(70);
	}
	std::cout << "Ended writeThread" << std::endl;
	LeaveCriticalSection(&deleteSection);
	return 0;
}

DWORD WINAPI readThread(LPVOID information) {
	std::string filePath = "D:/work/C++/3kurs/spovm/lab5/Debug/0_File.txt";
	bool flagFirst = true;

	readFunction readTestFile = reinterpret_cast<readFunction>(GetProcAddress(hModuleDLL, "ReadTestFile"));

	EnterCriticalSection(&writeSection);
	EnterCriticalSection(&readSection);
	EnterCriticalSection(&deleteSection);

	CreateThread(nullptr, NULL, writeThread, message, NULL, nullptr);

	for (int index = 1; index <= NUMBER_FILES; index++) {
		if (flagFirst) {
			filePath[35]--;
			index--;
			flagFirst = false;
		}
		filePath[35]++;
		readTestFile(filePath.c_str(), message);

		LeaveCriticalSection(&writeSection);
		Sleep(50);
		EnterCriticalSection(&writeSection);

		if (index >= NUMBER_FILES)
			LeaveCriticalSection(&deleteSection);
		for (int i = 0; message[i]; i++)
			message[i] = '\0';
		Sleep(100);
	}

	EnterCriticalSection(&deleteSection);
	LeaveCriticalSection(&readSection);
	std::cout << "Ended readThread" << std::endl;
	return 0;
}

int main() {
	hModuleDLL = LoadLibraryA("testDLL.dll");

	InitializeCriticalSection(&writeSection);
	InitializeCriticalSection(&readSection);
	InitializeCriticalSection(&deleteSection);

	CreateThread(nullptr, NULL, readThread, message, NULL, nullptr);
	Sleep(100);
	EnterCriticalSection(&readSection);

	DeleteCriticalSection(&writeSection);
	DeleteCriticalSection(&readSection);
	DeleteCriticalSection(&deleteSection);

	FreeLibrary(hModuleDLL);
	return 0;
}