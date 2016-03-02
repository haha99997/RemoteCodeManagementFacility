///////////////////////////////////////////////////////////////////////
// Exec.cpp - Entry point for the application                        //
// ver 1.0                                                           //
// Language:    C++ 11, Visual Studio 2013                           //
// Platform:    Lenovo Y40-70, Win8.1                                //
// Application: CSE687, Project #1, Spring 2015                      //
// Author:      Chao Zuo                                             //
///////////////////////////////////////////////////////////////////////
/* Module Operations:
* This module defines the entry point for the application.
*
*
* Required Files:
* ===============
* All other header files and source code files.
*
*
* Maintenance History:
* --------------------
* ver 1.0: Jan 20, 2015
* first release.
*
*/

#include "stdafx.h"
#include "FileStore.h"
#include "FileMgr.h"
#include "Catalog.h"
#include "CmdLine.h"
#include "Display.h"

#if !defined(TEST_FILEMGR) && !defined(TEST_CATALOG) && !defined(TEST_FILESTORE) \
	&& !defined(TEST_CMDLINE) && !defined(TEST_DISPLAY)
int main(int argc, char* argv[])
{
	CmdLine cmd;
	if (!cmd.match(argc, argv))
		return EXIT_FAILURE;

	FileStore filestore;
	FileMgr filemgr(filestore);

	Catalog catalog(filestore);
	filemgr.setRecurse(cmd.isRecurse());
	filemgr.addPatterns(cmd.getPatterns());
	filemgr.findFiles(cmd.getPath());

	auto result = filestore.getFiles();

	if (cmd.isDuplicate())
	{
		auto result = catalog.getDuplicate();
		Display::showDuplicates(result);
	}
	if (cmd.isTextSearch())
	{
		std::string input;
		for (std::cin >> input; input.compare("quit") != 0; std::cin >> input)
		{
			if (input.substr(0, 3).compare("/f=") == 0)
			{
				auto result = catalog.getMatchedFiles(input.substr(3));
				Display::showTextSearch(result, input.substr(3));
			}
		}
	}
	Display::showSummary(filestore.getFilesCount(), filestore.getDirectoriesCount());
	return EXIT_SUCCESS;
}
#endif

