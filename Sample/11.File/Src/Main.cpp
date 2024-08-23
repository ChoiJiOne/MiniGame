//#include 

#include <iostream>

#include "FileManager.h"

int main(void)
{
	GameMaker::FileManager::Get().Startup();
	{
		std::cout << "WORKING DIRECTORY(ANSI): " << GameMaker::FileManager::Get().GetWorkingDirectoryA() << std::endl;
		std::wcout << L"WORKING DIRECTORY(UNICODE): " << GameMaker::FileManager::Get().GetWorkingDirectoryW() << std::endl;
	}
	GameMaker::FileManager::Get().Shutdown();
	return 0;
}