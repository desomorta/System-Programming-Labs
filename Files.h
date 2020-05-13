#pragma once
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <strsafe.h>

class Files
{
	bool folders(char* mainFolder);
public:	
	char* mainFolder;
	int subfoldersAmount;
	char subfolders[MAX_PATH][100];
	
	bool files();
	bool folders();

	Files(char* mainFolder)
	{
		this->subfoldersAmount = 0;
		this->mainFolder = mainFolder;
	}
};

