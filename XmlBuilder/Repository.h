#pragma once

///////////////////////////////////////////////////////////////////
// Repository.h - holds a stack                                  //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package hols a stack for push and pop elements.
*
* Required Files:
* ---------------
*   - IRulesandActions.h, IRulesandActions.cpp, XmlElement.cpp, XmlElement.cpp
*     Parser.h, Parser.cpp, RulesandActions.h, RulesandActions.cpp
*     Tokenizer.h, Tokenizer.cpp, xmlElementParts.h, xmlElementsParts.cpp
*     XmlBuilder.h, XmlBuilder.cpp
*
* Build Process:
* --------------
*   devenv XMLDocumentModel.sln /debug rebuild /Project XmlBuilder
*
* Maintenance History:
* --------------------
* ver 1.0 : 13 Mar 15
*   - first release
*/

#include <stack>
#include <vector>
#include "../XmlElement/XmlElement.h"

// Repository for XmlBuilder to build XmlElement hierarchy.
class Repository
{
public:
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement > ;
	using bufferElem = std::stack < sPtr >;
	Repository(){}
	bufferElem& stack() { return _buffer; }			//return a stack
	sPtr& tmp() { return _tmp; }					//return last element.
	std::string& xml() { return _xml; }				//return xml string.
	~Repository(){}
private:
	std::string _xml;
	sPtr _tmp;
	bufferElem _buffer;
};

