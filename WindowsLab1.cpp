#include <iostream>
#include <windows.h>
#include <chrono>

using namespace std;

int main() {
	STARTUPINFO si; 
  PROCESS_INFORMATION pi; 
	ZeroMemory(&si, sizeof(si));
	if (!CreateProcess(*PATH*, NULL, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) { 
 		cout << "Creating process error" << endl; 
		return 1;
	}

	cout << "Time of father process - ";
	std::cout << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
	WaitForSingleObject(pi.hProcess,INFINITE); 
	CloseHandle(pi.hProcess); 
	CloseHandle(pi.hThread);
	return 0;
}
