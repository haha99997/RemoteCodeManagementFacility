#pragma once

///////////////////////////////////////////////////////////////////
// RulesandActions.h - Concrete rules and actions for parsing xml//
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ------------------
* This module defines oncrete rules for detecting different xml parts:
*    DetectDeclarElement
*    DetectCommentElement
*    DetectTextElement
*    DetectTaggedElement
*    DetectTaggedElementWithLeave
*    DetectProcInstrElement
*    DetectLeavingScope
* It also defines actions:
*    PushStack
*    PopStack
*    CreateElement
*    AddChildRelation
*
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
*   devenv XMLDocumentModel.sln /debug rebuild /Project RulesAndActions
*
* Maintenance History:
* --------------------
* ver 1.0 : 13 Mar 15
*   - first release
*/

#include "IRulesandActions.h"
#include "../XmlElement/XmlElement.h"
#include "../XmlBuilder/Repository.h"

/******************Rules************************/

class DetectDeclarElement : public ARule
{
public:
	DetectDeclarElement(){}
	bool test(ITokCollection& semi) override;
	~DetectDeclarElement(){}
};


class DetectCommentElement : public ARule
{
public:
	DetectCommentElement(){}
	bool test(ITokCollection& semi) override;
	~DetectCommentElement(){}
};


class DetectTextElement : public ARule
{
public:
	DetectTextElement(){}
	bool test(ITokCollection& semi) override;
	~DetectTextElement(){}
};


class DetectTaggedElement : public ARule
{
public:
	DetectTaggedElement(){}
	bool test(ITokCollection& semi) override;
	~DetectTaggedElement(){}
};


class DetectTaggedElementWithLeave : public ARule
{
public:
	DetectTaggedElementWithLeave(){}
	bool test(ITokCollection& semi) override;
	~DetectTaggedElementWithLeave(){}
};


class DetectProcInstrElement : public ARule
{
public:
	DetectProcInstrElement(){}
	bool test(ITokCollection& semi) override;
	~DetectProcInstrElement(){}
};


class DetectLeavingScope :public ARule
{
public:
	DetectLeavingScope(){};
	bool test(ITokCollection& semi) override;
	~DetectLeavingScope(){};
};


/********************Actions**************************/

class CreateElement : public AAction
{
public:
	CreateElement(Repository& _repo) : repo(_repo){}
	void doAction(SemiExp& semi) override;
	~CreateElement(){}
private:
	void makeXmlDeclarElement(SemiExp& semi);
	void makeCommentElement(SemiExp& semi);
	void makeTextElement(SemiExp& semi);
	void makeTaggedElement(SemiExp& semi);
	void makeProcInstrElement(SemiExp& semi);
	Repository& repo;
};


class PushStack : public AAction
{
public:
	PushStack(Repository& _repo) : repo(_repo){}
	void doAction(SemiExp& semi) override;
	~PushStack(){}
private:
	Repository& repo;
};


class PopStack : public AAction
{
public:
	PopStack(Repository& _repo) : repo(_repo){}
	void doAction(SemiExp& semi) override;
	~PopStack(){}
private:
	Repository& repo;
};


class AddChildRelation : public AAction
{
public:
	AddChildRelation(Repository& _repo) : repo(_repo){}
	void doAction(SemiExp& semi) override;
	~AddChildRelation(){}
private:
	Repository& repo;
};
