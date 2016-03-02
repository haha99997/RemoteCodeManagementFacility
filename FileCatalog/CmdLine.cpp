///////////////////////////////////////////////////////////////////////
// CmdLine.h - The module to parse command line arguments            //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdLine.h"

//Match command line argument
bool CmdLine::match(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Invalid arguments. Will now exit." << std::endl;
		return false;
	}
	path = argv[1];
	splitString(argv[2], patterns, ";");
	for (int i = 3; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.compare("/s") == 0)
			recurse = true;
		else if (arg.compare("/d") == 0)
			duplicate = true;
		else if (arg.compare("/f") == 0)
			text_search = true;
	}
	return true;
}

// Split a string s with the seperator of string c and return the result to the vector v
void CmdLine::splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

// Test stub
#ifdef TEST_CMDLINE

int main(int argc, char* argv[])
{
	CmdLine cmdLine;
	if (!cmdLine.match(argc, argv))
		return EXIT_FAILURE;
	std::cout << cmdLine.isDuplicate() << cmdLine.isTextSearch() << cmdLine.isRecurse() << cmdLine.getPath();
	for (auto &item : cmdLine.getPatterns())
	{
		std::cout << item;
	}
}

#endif