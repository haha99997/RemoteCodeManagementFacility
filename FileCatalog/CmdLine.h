#pragma once
///////////////////////////////////////////////////////////////////////
// CmdLine.h - The module to parse command line arguments            //
// ver 1.0                                                           //
// Language:    C# 5.0, Visual Studio 2013, .Net Framework 4.5       //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE681, Project #2, Fall 2014                        //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module provides a function to parse the command line arguments.
*
* Public Interface:
*
* Cmdline cmdLine;		//Build a new CmdLine
*
* **Match command line argument**
* =============================
* if(cmdLine.match(argc, argv))		//return true if successfully matched arguments.
*
* **Get the state and parameters for running Executive**
* =============================
* bool recurse = cmdLine.isRecurse();
* bool duplicate = cmdLine.isDuplicate();
* bool text_search = cmdLine.isTextSearch();
* std::vector<std::string> patterns = cmdLine.getPatterns();
* std::string path = cmdLine.getPath();
*
* Maintenance History:
* --------------------
* ver 1.0 first release.
*
*/

class CmdLine
{
public:
	CmdLine() : recurse(false), duplicate(false), text_search(false) {};
	bool isRecurse() { return recurse; }
	bool isDuplicate() { return duplicate; }
	bool isTextSearch() { return text_search; }
	std::vector<std::string> getPatterns() { return patterns; }
	std::string getPath() { return path; }
	bool match(int argc, char* argv[]);
private:
	bool recurse;
	bool duplicate;
	bool text_search;
	std::string path;
	std::vector<std::string> patterns;
	void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
};

