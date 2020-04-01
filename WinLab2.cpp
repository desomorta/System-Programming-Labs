#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

const wchar_t* toWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new WCHAR[cSize]; 
	mbstowcs(wc, c, cSize); 

	return wc;
}

LPWSTR toWS(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new WCHAR[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

HANDLE newProc(int procNumber, const std::string& parentPath)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	const std::wstring path = toWC(parentPath.c_str());
	char args[3];
	sprintf(args + 1, "%d", procNumber);
	args[0] = ' ';

	if (!CreateProcess(path.c_str(), toWS(args), NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
	{
		std::cout << "Failed creating process" << std::endl;
		exit(-1);
	}
	return pi.hProcess;
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "RUSSIAN");
	if (argc > 1) 
	{
		HANDLE writeEvent = OpenEvent(EVENT_ALL_ACCESS, false, toWS((std::string("WriteEvent_") + argv[argc - 1]).c_str()));

		while (true)
		{
			WaitForSingleObject(writeEvent, INFINITE);
			printf_s("Process ");
			for (size_t i = 0; i < strlen(argv[argc - 1]); i++)
			{
				printf_s("%c", argv[argc - 1][i]);
				Sleep(500);
			}
			printf_s("\n");
			while (WaitForSingleObject(writeEvent, 0) == WAIT_OBJECT_0);
		}
	}

	std::vector<std::pair<HANDLE, HANDLE>> children;
	children.reserve(9);
	unsigned currentWriting = 0;
	bool alreadyAdded = false, alreadyDeleted = false; 

	std::cout << "Press '+' to create new process" << std::endl;
	std::cout << "Press '-' to kill last process" << std::endl;
	std::cout << "Press 'q' to exit the program" << std::endl;

	while (true)
	{
		if (GetKeyState(0x51))
		{
			while (!children.empty())
			{
				std::pair<HANDLE, HANDLE> child = children.back();
				TerminateProcess(child.first, EXIT_SUCCESS); 
				CloseHandle(child.second);
				CloseHandle(child.first);
				children.pop_back();
			}
			break;
		}
		if (GetKeyState(VK_OEM_PLUS) < 0 && !alreadyAdded) 
		{
			alreadyAdded = true;
			if (children.size() < 9)
			{
				HANDLE WriteEvent = CreateEvent(nullptr, TRUE, FALSE,
					toWS((std::string("WriteEvent_") + std::to_string(children.size() + 1)).
						c_str())); 
				HANDLE process = newProc(children.size() + 1, argv[0]); 
				children.emplace_back(process, WriteEvent);
			}
		}
		if (GetKeyState(VK_OEM_PLUS) > 0) //Если кнопка + отжата
			alreadyAdded = false;
		if (GetKeyState(VK_OEM_MINUS) < 0 && !alreadyDeleted) 
		{
			alreadyDeleted = true;
			if (!children.empty())
			{
				const std::pair<HANDLE, HANDLE> child = children.back();
				TerminateProcess(child.first, EXIT_SUCCESS);
				CloseHandle(child.second);
				CloseHandle(child.first);
				children.pop_back();
				if (children.empty())
					std::cout << "\nNo more processes running. To add new one press \"+\" or \"q\" to exit" << std::endl;
			}
			else
				std::cout << "\nNo processes running. To add new one press \"+\" or \"q\" to exit)" << std::endl;
		}
		if (GetKeyState(VK_OEM_MINUS) > 0) 
			alreadyDeleted = false;
		if (!children.empty())
		{
			if (++currentWriting >= children.size())
				currentWriting = 0; 
			SetEvent(children[currentWriting].second);
			while (WaitForSingleObject(children[currentWriting].second, 0) == WAIT_OBJECT_0); 
		}
	}
}
