#include "Files.h"
#include"Threads.h"

bool Files::folders()
{
	return folders(mainFolder);
}

bool Files::folders(char* mainFolder)
{
	size_t argLength;
	
	WIN32_FIND_DATA findData;
	TCHAR folderSize[MAX_PATH];
	HANDLE hFindFile;

	// �������� ����� ���� � �����
	StringCchLengthA((TCHAR*)mainFolder, MAX_PATH, &argLength);
	if (argLength > (MAX_PATH - 3))
	{
		std::cout << "Error 3: Path is too long\n";
		return false;
	}
	
	// ���������� ����� ������� �����
	StringCchCopyA(folderSize, MAX_PATH, (TCHAR*)mainFolder);
	StringCchCatA(folderSize, MAX_PATH, TEXT("\\*"));
	
	//���������� ������ ��������������
	hFindFile = FindFirstFileA(folderSize, &findData);
	if (INVALID_HANDLE_VALUE == hFindFile) 
	{
		std::cout << "Error 4: Directory not found\n";
		return false;
	} 

	//����������� ���� �������������� � ������
	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!strcmp(findData.cFileName, ".") || !strcmp(findData.cFileName, ".."))
				continue;

			strcat(strcat(strcpy(subfolders[subfoldersAmount], mainFolder), "\\"), findData.cFileName);
			subfoldersAmount++;
		}
	}
	while (FindNextFileA(hFindFile, &findData) != 0);

	FindClose(hFindFile);
	return true;
}

bool Files::files()
{
	size_t argLength;
	WIN32_FIND_DATA findData;
	TCHAR folderSize[MAX_PATH];
	HANDLE hFindFile = INVALID_HANDLE_VALUE;

	for(int i = 0; i < subfoldersAmount; i++)
	{
		// �������� ����� ���� � �����
		StringCchLengthA((TCHAR*)subfolders[i], MAX_PATH, &argLength);
		if (argLength > MAX_PATH - 3)
		{
			std::cout << "Error 3: path is too long\n";
			return false;
		}

		// ���������� ����� ������� �����
		StringCchCopyA(folderSize, MAX_PATH, (TCHAR*)subfolders[i]);
		StringCchCatA(folderSize, MAX_PATH, TEXT("\\*"));
	
		// ���������� ������� ����� � �����
		hFindFile = FindFirstFileA(folderSize, &findData);
		if (INVALID_HANDLE_VALUE == hFindFile) 
		{
			std::cout << "Error 6: file not found in directory\n";
			return false;
		}
   
		// ������ ������ � �����
		char rootFolder[MAX_PATH];
		strcpy(rootFolder, subfolders[i]);
		strcat(rootFolder, ".txt");
		Threads fThread(subfolders[i], rootFolder, findData, hFindFile);
	}
	FindClose(hFindFile);
	return true;
}