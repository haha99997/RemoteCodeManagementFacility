#pragma once

///////////////////////////////////////////////////////////////////////
// Display.h - Display Module                                        //
// ver 1.0                                                           //
// Language:    C++ 11, Visual Studio 2013                           //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE687, Project #1, Spring 2015                      //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module is used for showing results generated by Exec
*
*
* Public Interface
* ================
* Display::showDuplicates(std::map<std::string, FileStore::ListofRefs>& item)	//Show duplicate files
* Display::showTextSearch(std::vector<std::string>& item, std::string pattern)	//Show files containing the pattern.
* Display::showSummary(size_t cnt_files, size_t cnt_directories)				//Show numbers of files and directories.
*
* Required Files:
* ===============
* Display.h, Display.cpp
*
*
* Maintenance History:
* --------------------
* ver 1.0: Jan 20, 2015
* first release.
*
*/
#include "FileStore.h"

class Display
{
public:
	static void showDuplicates(std::map<std::string, FileStore::ListofRefs>& item);
	static void showTextSearch(std::vector<std::string>& item, std::string pattern);
	static void showSummary(size_t cnt_files, size_t cnt_directories);
};

