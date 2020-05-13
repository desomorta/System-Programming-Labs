#include "Files.h"
#include <iostream>

int main()
{
	char folder[] = {"C:\\Users\\mrsto\\Desktop\\lab5\\files"};
	Files find(folder);
	
	if(!find.folders())
	{
		std::cout << "Error 1: Directory not found\n";
		std::cin.get();
		return -1;
	}
	if(!find.files())
	{
		std::cout << "Error 2: File not found\n";
		std::cin.get();
		return -1;
	}
	std::cout << "\n";
	system("pause");
	return 0;
}