#pragma once

///////////////////////////////////////////////////////////////////
// IRulesandActions.h - Define interfaces and abstract classes   //
//    for Rules and Actions as well as factory functions         //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module defines the following classes:
*   IRule   - interface contract for Rules
*   ARule   - abstract base class for Rules that defines some common ops
*   IAction - interface contract for rule actions
*   AAction - abstract base class for actions that defines common ops
* Besides, it also defines factories for creating concrete rules and actions.
*   std::shared_ptr<IRule> makeDetectDeclarElement();
*   std::shared_ptr<IRule> makeDetectCommentElement();
*   std::shared_ptr<IRule> makeDetectTextElement();
*   std::shared_ptr<IRule> makeDetectTaggedElement();
*   std::shared_ptr<IRule> makeDetectTaggedElementWithLeave();
*   std::shared_ptr<IRule> makeDetectProcInstrElement();
*   std::shared_ptr<IRule> makeDetectLeavingScope();
*   std::shared_ptr<IAction> makeCreateElement(Repository& repo);
*   std::shared_ptr<IAction> makePushStack(Repository& repo);
*   std::shared_ptr<IAction> makePopStack(Repository& repo);
*   std::shared_ptr<IAction> makeAddChildRelation(Repository& repo);
*
* Required Files:
*   IRuleandAction.h, IRulesandActions.cpp
*
* Build command:
*   Interfaces and abstract base classes only so no build
*
* Maintenance History:
* --------------------
* Ver 1.0 : Mar 13 15
* - first release
*/

#include "../XmlElementParts/itokcollection.h"
#include "../XmlBuilder/Repository.h"

#include <vector>
#include <memory>

class IAction
{
public:
	using SemiExp = std::vector < std::string >;
	virtual void doAction(SemiExp& semi) = 0;
	virtual ~IAction(){};
};

class AAction : public IAction
{
public:
	virtual void doAction(SemiExp& semi) = 0;
	virtual ~AAction(){};
};

class IRule
{
public:
	using SemiExp = std::vector < std::string > ;
	virtual void add(IAction* action) = 0;
	virtual bool test(ITokCollection& semi) = 0;
	virtual ~IRule(){};
};

class ARule :
	public IRule
{
public:
	void add(IAction* action) override;
	virtual bool test(ITokCollection& semi) = 0;
	void doActions(SemiExp& semi);
	virtual ~ARule(){};
private:
	std::vector<IAction*> actions;
};


std::shared_ptr<IRule> makeDetectDeclarElement();
std::shared_ptr<IRule> makeDetectCommentElement();
std::shared_ptr<IRule> makeDetectTextElement();
std::shared_ptr<IRule> makeDetectTaggedElement();
std::shared_ptr<IRule> makeDetectTaggedElementWithLeave();
std::shared_ptr<IRule> makeDetectProcInstrElement();
std::shared_ptr<IRule> makeDetectLeavingScope();
std::shared_ptr<IAction> makeCreateElement(Repository& repo);
std::shared_ptr<IAction> makePushStack(Repository& repo);
std::shared_ptr<IAction> makePopStack(Repository& repo);
std::shared_ptr<IAction> makeAddChildRelation(Repository& repo);