#pragma once
///////////////////////////////////////////////////////////////////
// XmlBuilder.h - Construct Xml Parse Tree                       //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ------------------
* This module accept a string representing a filename or a string,
* and construct the XMl parse tree, returning the root.
*
*	XmlBuilder xb("test.xml", true);
*	XmlBuilder xb("<?xml version=\"1.0\"?><root/>", false);
*	std::shared_ptr < XmlProcessing::AbstractXmlElement > root = xb.BuildXmlTree();
*	std::shared_ptr < XmlProcessing::AbstractXmlElement > root2 = xb2.BuildXmlTree();
*
* Required Files:
* ---------------
*   - IRulesandActions.h, IRulesandActions.cpp, XmlElement.cpp, XmlElement.cpp
*     Parser.h, Parser.cpp, RulesandActions.h, RulesandActions.cpp
*     Tokenizer.h, Tokenizer.cpp, xmlElementParts.h, xmlElementsParts.cpp
*     Repository.h, Repository.cpp
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

#include <vector>
#include <string>
#include "../XmlElement/XmlElement.h"
#include "../XmlElementParts/xmlElementParts.h"
#include "../XmlElementParts/Tokenizer.h"
#include "../RulesAndActions/IRulesandActions.h"
#include "../Parser/Parser.h"

class XmlBuilder
{
public:
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
	using elems = std::vector < sPtr > ;

	XmlBuilder() = delete;
	XmlBuilder(const std::string& src, bool isFile = true);
	~XmlBuilder();
	sPtr buildXmlTree();
private:
	Parser parser;
	Toker *toker;
	XmlParts *parts;
	Repository repo;

	std::shared_ptr<IRule> detectDeclar = makeDetectDeclarElement();
	std::shared_ptr<IRule> detectComment = makeDetectCommentElement();
	std::shared_ptr<IRule> detectText = makeDetectTextElement();
	std::shared_ptr<IRule> detectTag = makeDetectTaggedElement();
	std::shared_ptr<IRule> detectTagLeave = makeDetectTaggedElementWithLeave();
	std::shared_ptr<IRule> detectProcInstr = makeDetectProcInstrElement();
	std::shared_ptr<IRule> detectLeave = makeDetectLeavingScope();

	std::shared_ptr<IAction> pushStack = makePushStack(repo);
	std::shared_ptr<IAction> popStack = makePopStack(repo);
	std::shared_ptr<IAction> create = makeCreateElement(repo);
	std::shared_ptr<IAction> addChild = makeAddChildRelation(repo);

	void initialize();
};

