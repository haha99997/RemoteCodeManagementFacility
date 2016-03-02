#pragma once
///////////////////////////////////////////////////////////////////////
// FileStore.h - Stores files and directories                        //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module provides a store for storing files. When method save is called, 
* a new file will be added. It also provides interface for iterating. Files 
* names and paths are stored seperatedly to save space.
*
*
* Public Interface:
*
* **Declaring a new FileStore**
* =============================
* FileStore filestore;
*
* **Saving files**
* =============================
* filestore.save("D:\\", "input.txt");
* 
* **Getting numbers of files and directories**
* =============================
* int cnt_files = filestore.getFilesCount();
* int cnt_directoreis = filestore.getDirectoriesCount();
*
* **Iterating FileStore**
* =============================
* for(auto iter = filestore.begin(); iter != filestore.end(); ++iter) {...}
*
* Required Files:
* ===============
* FileStore.h, FileStore.cpp
*
*
* Build Process:
* --------------
*   devenv RemoteCodeManagementFacility.sln /release rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 first release.
*
*/

#include <map>
#include <set>

class FileStore
{
public:
	FileStore() : cnt_files(0), cnt_directories(0){}
	using File = std::string;
	using Path = std::string;
	using PathRef = std::set<Path>::iterator;
	using ListofRefs = std::vector < PathRef >;
	using iterator = std::map<File, ListofRefs>::iterator;
	void save(const std::string& path, const std::string& filename);
	iterator begin() { return files.begin(); }
	iterator end() { return files.end(); }
	std::vector<std::string> getFiles();
	int getFilesCount() { return cnt_files; }
	int getDirectoriesCount() { return cnt_directories; }
private:
	std::map<File, ListofRefs> files;
	std::set<Path> paths;
	int cnt_files, cnt_directories;
};

