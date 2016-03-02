#pragma once
///////////////////////////////////////////////////////////////////////
// FileMgr.h - File Manager                                          //
// ver 1.1                                                           //
// Language:    C++ 11, Visual Studio 2013                           //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE687, Project #1, Spring 2015                      //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module is used for detecting files in the current directory and subdirectories.
* When recurse = true, it will detect subdirectories also.
*
*
* Public Interface
* ================
* FileMgr fileMgr;							  // Create a new FileMgr
* fileMgr.setRecurse(true);                   // Set recurse to true such that it searches for subdirectories
* fileMgr.fileFiles(path);                    // File files in the certain path. path must be relative path
* fileMgr.addPattern(pattern);                // Add a pattern, eg: *.cs
* fileMgr.addPatterns(vector of patterns);    // Add a group of patterns
* fileMgr.getFiles();                         // Get the search result. returns an array for further use.
* fileMgr.getCurrentDirectory();			  // Get the current directory
* fileMgr.setCurrentDirectory("D:\");		  // Set the current directory
*
* Required Files:
* ===============
* FileMgr.h, FileMgr.cpp
*
*
* Build Process:
* --------------
*   devenv RemoteCodeManagementFacility.sln /release rebuild
*
*
* Maintenance History:
* --------------------
* ver 1.1: Apr 24, 2015
* Added getCurrentDirectory and setCurrentDirectory.
*
* ver 1.0: Jan 20, 2015
* first release.
*
*/

#include "FileSystem.h"
#include "FileStore.h"

class FileMgr
{
public:
	using FileSpec = std::pair < std::string, std::string >;
	FileMgr(FileStore& fs) : filestore_(fs) {}
	void setRecurse(bool x);
	void findFiles(const std::string& path);
	void addPattern(const std::string& pattern);
	void addPatterns(const std::vector<std::string>& patterns);
	std::string getCurrentDirectory();						// Get the current directory
	void setCurrentDirectory(const std::string& path);		// Set the current directory
	~FileMgr(){};
private:
	FileStore& filestore_;
	std::vector<std::string> patterns;
	bool recurse = false;
};


