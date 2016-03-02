///////////////////////////////////////////////////////////////////////
// Display.cpp - Display Module                                      //
// ver 1.0                                                           //
// Language:    C++ 11, Visual Studio 2013                           //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE687, Project #1, Spring 2015                      //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Display.h"

//Show duplicate files
void Display::showDuplicates(std::map<std::string, FileStore::ListofRefs>& item){
	for (auto &item : item)
	{
		std::cout << item.first << ":  Appeared " <<
			item.second.size() << " times in:\n";
		for (auto &path : item.second)
			std::cout << *path << std::endl;
		std::cout << "\n-----------------------\n";
	}
}

//Show files containing the pattern.
void Display::showTextSearch(std::vector<std::string>& item, std::string pattern)
{
	std::cout << item.size() << " files contain \"" << pattern << "\"" << std::endl;
	for (auto &fileSpec : item)
	{
		std::cout << fileSpec << std::endl;
	}
}

//Show numbers of files and directories.
void Display::showSummary(size_t cnt_files, size_t cnt_directories)
{
	std::cout << cnt_files << " files in " << cnt_directories << " directories" << std::endl;
}

// Test stub
#ifdef TEST_DISPLAY
int main()
{
	Display::showSummary(2,3);
}
#endif