///////////////////////////////////////////////////////////////////
// IRulesandActions.cpp - Define interfaces and abstract classes //
//    for Rules and Actions as well as factory functions         //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

#include "IRulesandActions.h"
#include "RulesandActions.h"

//--------------<Factory Methods>-------------------------

std::shared_ptr<IRule> makeDetectDeclarElement()
{
	std::shared_ptr<IRule> ptr(new DetectDeclarElement());
	return ptr;
}

std::shared_ptr<IRule> makeDetectCommentElement()
{
	std::shared_ptr<IRule> ptr(new DetectCommentElement());
	return ptr;
}

std::shared_ptr<IRule> makeDetectTextElement()
{
	std::shared_ptr<IRule> ptr(new DetectTextElement());
	return ptr;
}

std::shared_ptr<IRule> makeDetectTaggedElement()
{
	std::shared_ptr<IRule> ptr(new DetectTaggedElement());
	return ptr;
}

std::shared_ptr<IRule> makeDetectTaggedElementWithLeave()
{
	std::shared_ptr<IRule> ptr(new DetectTaggedElementWithLeave());
	return ptr;
}

std::shared_ptr<IRule> makeDetectProcInstrElement()
{
	std::shared_ptr<IRule> ptr(new DetectProcInstrElement());
	return ptr;
}

std::shared_ptr<IRule> makeDetectLeavingScope()
{
	std::shared_ptr<IRule> ptr(new DetectLeavingScope());
	return ptr;
}

std::shared_ptr<IAction> makeCreateElement(Repository& repo)
{
	std::shared_ptr<IAction> ptr(new CreateElement(repo));
	return ptr;
}

std::shared_ptr<IAction> makePushStack(Repository& repo)
{
	std::shared_ptr<IAction> ptr(new PushStack(repo));
	return ptr;
}

std::shared_ptr<IAction> makePopStack(Repository& repo)
{
	std::shared_ptr<IAction> ptr(new PopStack(repo));
	return ptr;
}

std::shared_ptr<IAction> makeAddChildRelation(Repository& repo)
{
	std::shared_ptr<IAction> ptr(new AddChildRelation(repo));
	return ptr;
}

// Add an IAction to ARule
void ARule::add(IAction* action)
{
	actions.push_back(action);
}

// Do actions.
void ARule::doActions(SemiExp& semi)
{
	for (auto action : actions)
	{
		action->doAction(semi);
	}
}


