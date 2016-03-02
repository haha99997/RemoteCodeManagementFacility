#pragma once
///////////////////////////////////////////////////////////////////////
// Catalog.h - Module to provide catalog fuctions for FileStore      //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module provides functions to find duplicates and match files containing
* a text;
*
* Public Interface:
*
* FileStore fileStore;
* Catalog catalog(fileStore)		//Build a new Catalog using a FileStore
* catalog.setFileStore(fileStore)	//Use the given fileStore
* std::map<std::string, FileStore::ListofRefs> duplicate = catalog.getDuplicate()	//Get duplicate files
* std::vector<std::string> getMatchedFiles(std::string& pattern)					//Get files containing a pattern

* Maintenance History:
* --------------------
* ver 1.0 first release.
*
*/

#include "FileStore.h"

class Catalog
{

public:
	Catalog(FileStore& fs) : filestore_(fs){}
	void setFileStore(FileStore& fs) { filestore_ = fs; }
	std::map<std::string, FileStore::ListofRefs> getDuplicate();
	std::vector<std::string> getMatchedFiles(std::string& pattern);
private:
	FileStore& filestore_;
};

