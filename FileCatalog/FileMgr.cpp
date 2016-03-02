///////////////////////////////////////////////////////////////////////
// FileMgr.cpp - File Manager                                        //
// ver 1.1                                                           //
// Language:    C++ 11, Visual Studio 2013                           //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE687, Project #1, Spring 2015                      //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileMgr.h"


// Set recurse to true such that it searches for subdirectories
void FileMgr::setRecurse(bool x)
{
	this->recurse = x;
}

void setCurrentDirectory(const std::string& path)
{
	FileSystem::Directory::setCurrentDirectory(path);
}

// File files in the certain path
void FileMgr::findFiles(const std::string& path)
{
	if (patterns.size() == 0)
		patterns.push_back("*.*");

	if (!FileSystem::Directory::exists(path))
		return;

	FileSystem::Directory::setCurrentDirectory(path);
	//std::cout << FileSystem::Path::getFullFileSpec(".") << std::endl;;
	for (std::string& pattern : patterns)
	{
		auto cur_files = FileSystem::Directory::getFiles(".", pattern);
		for (auto &file : cur_files)
		{
			file = FileSystem::Path::getFullFileSpec(file);
		}
		for (auto file : cur_files)
		{
			std::string path_ = FileSystem::Path::getPath(file);
			std::string file_ = FileSystem::Path::getName(file);
			filestore_.save(path_, file_);
		}
	}

	if (recurse)
	{
		auto directories = FileSystem::Directory::getDirectories();
		directories.erase(directories.begin());		//ignore .
		directories.erase(directories.begin());		//ignore ..

		for (auto dir : directories)
		{
			findFiles(dir);
			FileSystem::Directory::setCurrentDirectory("..");
		}
	}
}

// Add a pattern, eg: *.cs
void FileMgr::addPattern(const std::string& pattern)
{
	patterns.push_back(pattern);
}

void FileMgr::addPatterns(const std::vector<std::string>& patterns)
{
	for (auto &s : patterns)
		addPattern(s);
}

// Get the current directory
std::string FileMgr::getCurrentDirectory()
{
	return FileSystem::Directory::getCurrentDirectory();
}

// Set the current directory
void FileMgr::setCurrentDirectory(const std::string& path)
{
	FileSystem::Directory::setCurrentDirectory(path);
}

// test stub
#ifdef TEST_FILEMGR
int main()
{
	FileStore fs;
	FileMgr fm(fs);
	fm.setRecurse(true);
	fm.addPattern("*.*");
	//fm->addPattern("*.pch");
	fm.findFiles(".");
	for (auto &item : fs)
	{
		for (auto &path : item.second)
		{
			std::cout << *path << item.first << std::endl;
		}
	}
}
#endif
