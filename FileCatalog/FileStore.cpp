///////////////////////////////////////////////////////////////////////
// FileStore.cpp - Stores files and directories                      //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileStore.h"

//Saving files
void FileStore::save(const std::string& path, const std::string& filename)
{
	cnt_files++;
	PathRef ref = paths.find(path);
	if (ref == paths.end())
	{
		paths.insert(path);
		ref = paths.find(path);
		cnt_directories++;
	}
	
	if (files.find(filename) == files.end())
	{
		ListofRefs refs;
		files[filename] = std::move(refs);
	}
	files[filename].push_back(ref);
}

std::vector<std::string> FileStore::getFiles()
{
	std::vector<std::string> result;
	for (auto iter = files.begin(); iter != files.end(); ++iter)
	{
		for (auto& path : iter->second)
		{
			result.push_back(*path + iter->first);
		}
	}
	return result;
}

// Test stub
#ifdef TEST_FILESTORE

int main()
{
	FileStore fs;
	fs.save(".", "a.txt");
	fs.save(".", "b.txt");
	fs.save(".", "c.txt");
	fs.save("Debug", "a.txt");
	for (auto &f : fs)
	{
		std::cout << f.first << " ";
		for (auto &r : f.second)
		{
			std::cout << *r << " ";
		}
		std::cout << std::endl;
	}
}

#endif