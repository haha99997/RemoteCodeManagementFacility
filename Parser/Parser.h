#pragma once

///////////////////////////////////////////////////////////////////
// Parser.h - Parser detects code constructs defined by rules    //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ------------------
* This module defines the following class:
*   Parser  - a collection of IRule
*	it parses a ITokenCollection and will trigger certain actions.
*
* Required Files:
* ---------------
*   - IRulesandActions.h, IRulesandActions.cpp, XmlElement.cpp, XmlElement.cpp
*     Parser.h, Parser.cpp, RulesandActions.h, RulesandActions.cpp
*     Tokenizer.h, Tokenizer.cpp, xmlElementParts.h, xmlElementsParts.cpp
*
* Build Process:
* --------------
*   devenv XMLDocumentModel.sln /debug rebuild /Project Parser
*
* Maintenance History:
* --------------------
* ver 1.0 : 13 Mar 15
*   - first release
*/

#include <vector>
#include "../RulesAndActions/IRulesandActions.h"
#include "../XmlElementParts/itokcollection.h"

// Parser adds a collection of IRules and parse ITokenCollection.
class Parser
{
public:
	Parser(){};
	void add(IRule* rule) { rules.push_back(rule); }
	void parse(ITokCollection& semi);					//// parse toks
	~Parser(){};
private:
	std::vector<IRule*> rules;
};

