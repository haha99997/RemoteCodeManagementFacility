///////////////////////////////////////////////////////////////////////
// Catalog.cpp - Module to provide catalog fuctions for FileStore    //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Catalog.h"

//Get duplicate files
std::map<std::string, FileStore::ListofRefs> Catalog::getDuplicate()
{
	std::map<std::string, FileStore::ListofRefs> result;
	for (auto &file : filestore_)
	{
		if (file.second.size() > 1)
		{
			result[file.first] = file.second;
		}
	}
	return result;
}

//Get files containing a pattern
std::vector<std::string> Catalog::getMatchedFiles(std::string& pattern)
{
	std::vector<std::string> result;
	for (auto &file : filestore_)
	{
		for (auto &path : file.second) 
		{
			std::string fileSpec = *path + file.first;
			std::ifstream ifs(fileSpec);
			std::string content((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));
			std::regex rx (pattern);
			if (std::regex_search(content.begin(), content.end(), rx))
				result.push_back(fileSpec);
		}
	}
	return result;
}

// Test stub
#ifdef TEST_CATALOG
int main()
{
	FileStore fs;
	fs.save(".", "a.txt");
	fs.save(".", "b.txt");
	fs.save(".", "c.txt");
	fs.save("Debug", "a.txt");
	Catalog ctlg(fs);
	auto result = ctlg.getDuplicate();
	for (auto &item : result)
	{
		std::cout << "\n-----------------------\n" << item.first << ":  Appeared " <<
			item.second.size() << " times in:\n";
		for (auto &path : item.second)
			std::cout << *path << std::endl;

	}

	FileStore fs2;
	fs2.save("D:\\", "input.txt");
	fs2.save("D:\\", "input2.txt");
	ctlg.setFileStore(fs2);
	auto mat = ctlg.getMatchedFiles(std::string("010"));
	for (auto &r : mat)
	{
		std::cout << r << std::endl;
	}
}
#endif