//////////////////////////////////////////////////////////////////////
// RulesandActions.cpp - Concrete rules and actions for parsing xml //
// ver 1.0                                                          //
// Application: CSE687 Pr#2, Spring 2015                            //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013      //
// Author:      Chao Zuo                                            //
//////////////////////////////////////////////////////////////////////

#include "RulesandActions.h"
#include "../XmlElementParts/xmlElementParts.h"

#define DECLARATION		"declar"
#define COMMENT			"comment"
#define TEXT			"text"
#define TAG				"tag"
#define PROCINSTR		"instr"
#define LEAVE			"leave"

inline bool isSpecialCharacter(std::string& c)
{
	if (c == "." || c == "-" || c == "/" || c == ":")
		return true;
	return false;
}

// Extract tag, since ".", "-", "/", ":" may seperate the tag into different tokens
std::string extractIdentifier(ITokCollection& semi, int& start)
{
	if (start < semi.length())
	{
		if (isSpecialCharacter(semi[start + 1]))
		{
			int index = start + 2;
			while (index < semi.length() && semi[index] != "="
				&& semi[index] != "/" && semi[index] != ">")
				++index;
			if (semi[index] == "=")
			{
				index -= 2;
				std::string result;
				for (int i = start; i <= index; ++i)
					result += semi[i];
				start = index + 1;
				return result;
			}
			else
			{
				index -= 1;
				std::string result;
				for (int i = start; i <= index; ++i)
					result += semi[i];
				start = index;
				return result;
			}
		}
		else
		{
			++start;
			return semi[start - 1];
		}
	}
	else
	{
		return "";
	}
}

// Extract attribute, since ".", "-", "/" may seperate the tag into different tokens
std::string extractAttribute(ITokCollection& semi, int& start)
{
	if (start < semi.length())
	{
		if (isSpecialCharacter(semi[start + 1]))
		{
			int index = start + 2;
			while (index < semi.length() && semi[index] != "="
				&& semi[index] != "/" && semi[index] != ">")
				++index;

			index -= 1;
			std::string result;
			for (int i = start; i <= index; ++i)
				result += semi[i];
			start = index + 2;
			return result;

		}
		else
		{
			start += 2;
			return semi[start - 2];
		}
	}
	else
	{
		return "";
	}
}

// Extract value
std::string extractValue(ITokCollection& semi, int& start)
{
	int tmp = start++;
	return semi[tmp].substr(1, semi[tmp].length() - 2);
}

// Detect Declaration
bool DetectDeclarElement::test(ITokCollection& semi)
{
	int len = semi.length();
	if (semi[0] == "<" && semi[1] == "?" && semi[2] == "xml" && semi[3] != "-"
		&&semi[len - 2] == "?" && semi[len - 1] == ">")
	{
		SemiExp instr;
		instr.push_back(DECLARATION);
		for (int i = 3; i < len - 2;)
		{
			instr.push_back(extractAttribute(semi, i));
			instr.push_back(extractValue(semi, i));
		}
		doActions(instr);
		return true;
	}
	return false;
}

// Detect Comment
bool DetectCommentElement::test(ITokCollection& semi)
{
	int len = semi.length();
	if (semi[0] == "<" && semi[1] == "!" && semi[2] == "--"
		&&semi[len - 2] == "--" && semi[len - 1] == ">")
	{
		SemiExp instr;
		instr.push_back(COMMENT);
		for (int i = 3; i < len - 2; ++i)
			instr.push_back(semi[i]);
		doActions(instr);
		return true;
	}
	return false;
}

// Detect Text
bool DetectTextElement::test(ITokCollection& semi)
{
	if (semi[0] != "<" && semi[semi.length() - 1] != ">")
	{
		SemiExp instr;
		instr.push_back(TEXT);
		for (int i = 0; i < semi.length(); ++i)
			instr.push_back(semi[i]);
		doActions(instr);
		return true;
	}
	return false;
}

// Detect Tagged Element
bool DetectTaggedElement::test(ITokCollection& semi)
{
	int len = semi.length();
	if (semi[0] == "<" && semi[1] != "!" && semi[1] != "?" && semi[1] != "/"
		&& semi[len - 2] != "/" && semi[len - 1] == ">")
	{
		int start = 1;
		SemiExp instr;
		instr.push_back(TAG);
		instr.push_back(extractIdentifier(semi, start));
		for (int i = start; i < len - 1;)
		{
			instr.push_back(extractAttribute(semi, i));
			instr.push_back(extractValue(semi, i));
		}
		doActions(instr);
		return true;
	}
	return false;
}

// Detect elements like <tag/>
bool DetectTaggedElementWithLeave::test(ITokCollection& semi)
{
	int len = semi.length();
	if (semi[0] == "<" && semi[1] != "!" && semi[1] != "?" && semi[1] != "/"
		&& semi[len - 2] == "/" && semi[len - 1] == ">")
	{
		int start = 1;
		SemiExp instr;
		instr.push_back(TAG);
		instr.push_back(extractIdentifier(semi, start));
		for (int i = start; i < len - 2;)
		{
			instr.push_back(extractAttribute(semi, i));
			instr.push_back(extractValue(semi, i));
		}
		doActions(instr);
		return true;
	}
	return false;
}

// detect process instructions
bool DetectProcInstrElement::test(ITokCollection& semi)
{
	int len = semi.length();
	if (semi[0] == "<" && semi[1] == "?" &&
		(semi[2] != "xml" || (semi[2] == "xml" && semi[3] == "-"))
		&& semi[len - 2] == "?" && semi[len - 1] == ">")
	{
		int start = 2;
		SemiExp instr;
		instr.push_back(PROCINSTR);
		instr.push_back(extractIdentifier(semi, start));
		for (int i = start; i < len - 2;)
		{
			instr.push_back(extractAttribute(semi, i));
			instr.push_back(extractValue(semi, i));
		}
		doActions(instr);
		return true;
	}
	return false;
}

// detect elements like </tag>
bool DetectLeavingScope::test(ITokCollection& semi)
{
	if (semi[0] == "<" && semi[1] == "/" && semi[semi.length() - 1] == ">")
	{
		SemiExp instr;
		instr.push_back(LEAVE);
		int i = 2;
		instr.push_back(extractIdentifier(semi, i));
		doActions(instr);
		return true;
	}
	return false;
}

void CreateElement::makeXmlDeclarElement(SemiExp& semi)
{
	auto elem = XmlProcessing::makeXmlDeclarElement();
	for (size_t i = 1; i < semi.size(); i += 2)
		elem->addAttrib(semi[i], semi[i + 1]);
	repo.tmp() = elem;
}

void CreateElement::makeCommentElement(SemiExp& semi)
{
	std::string comment;
	for (size_t i = 1; i < semi.size(); ++i)
		comment += semi[i] + " ";
	auto elem = XmlProcessing::makeCommentElement(comment);
	repo.tmp() = elem;
}

void CreateElement::makeTextElement(SemiExp& semi)
{
	std::string text;
	for (size_t i = 1; i < semi.size(); ++i)
		text += semi[i] + " ";
	auto elem = XmlProcessing::makeTextElement(text);
	repo.tmp() = elem;
}

void CreateElement::makeTaggedElement(SemiExp& semi)
{
	auto elem = XmlProcessing::makeTaggedElement(semi[1]);
	for (size_t i = 2; i < semi.size(); i += 2)
		elem->addAttrib(semi[i], semi[i + 1]);
	repo.tmp() = elem;
}

void CreateElement::makeProcInstrElement(SemiExp& semi)
{
	auto elem = XmlProcessing::makeProcInstrElement(semi[1]);
	for (size_t i = 2; i < semi.size(); i += 2)
		elem->addAttrib(semi[i], semi[i + 1]);
	repo.tmp() = elem;
}

// Create a concrete element
void CreateElement::doAction(SemiExp& semi)
{
	if (semi[0] == DECLARATION)
		makeXmlDeclarElement(semi);
	else if (semi[0] == COMMENT)
		makeCommentElement(semi);
	else if (semi[0] == TEXT)
		makeTextElement(semi);
	else if (semi[0] == TAG)
		makeTaggedElement(semi);
	else if (semi[0] == PROCINSTR)
		makeProcInstrElement(semi);
}

// push stack
void PushStack::doAction(SemiExp& semi)
{
	repo.stack().push(repo.tmp());
}

// pop stack
void PopStack::doAction(SemiExp& semi)
{
	repo.tmp() = repo.stack().top();
	repo.stack().pop();
}

// add child relation
void AddChildRelation::doAction(SemiExp& semi)
{
	repo.stack().top()->addChild(repo.tmp());
}

// ---------------<test stub>----------------
#ifdef TEST_RULESANDACTIONS
int main()
{
	Repository repo;

	auto detectDeclar = makeDetectDeclarElement();
	auto detectComment = makeDetectCommentElement();
	auto detectText = makeDetectTextElement();
	auto detectTag = makeDetectTaggedElement();
	auto detectTagLeave = makeDetectTaggedElementWithLeave();
	auto detectProcInstr = makeDetectProcInstrElement();
	auto detectLeave = makeDetectLeavingScope();

	auto pushStack = makePushStack(repo);
	auto popStack = makePopStack(repo);
	auto create = makeCreateElement(repo);
	auto addChild = makeAddChildRelation(repo);

	detectComment->add(create.get());
	detectComment->add(pushStack.get());
	detectComment->add(popStack.get());
	detectComment->add(addChild.get());

	detectDeclar->add(create.get());
	detectDeclar->add(pushStack.get());
	detectDeclar->add(popStack.get());
	detectDeclar->add(addChild.get());

	detectText->add(create.get());
	detectText->add(pushStack.get());
	detectText->add(popStack.get());
	detectText->add(addChild.get());

	detectTag->add(create.get());
	detectTag->add(pushStack.get());

	detectTagLeave->add(create.get());
	detectTagLeave->add(pushStack.get());
	detectTagLeave->add(popStack.get());
	detectTagLeave->add(addChild.get());

	detectProcInstr->add(create.get());
	detectProcInstr->add(pushStack.get());
	detectProcInstr->add(popStack.get());
	detectProcInstr->add(addChild.get());

	detectLeave->add(popStack.get());
	detectLeave->add(addChild.get());

	std::string Prologue = "<?xml version=\"1 . 0 / 0\"?><?xml-stylesheet type=\"text / xsl\" href=\"ivy - report.xsl\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	std::string test2 = Prologue;
	test2 += "<Dir><path>/TestFile/Dir1</path><File date='notCurrent'><name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";
	Toker toker(test1, false);
	toker.setMode(Toker::xml);
	XmlParts xmlParts(&toker);
	auto doc = XmlProcessing::makeDocElement(nullptr);
	repo.stack().push(doc);
	while (xmlParts.get())
	{
		std::cout << xmlParts.show().c_str() << std::endl;
		detectDeclar->test(xmlParts);
		detectComment->test(xmlParts);
		detectText->test(xmlParts);
		detectTag->test(xmlParts);
		detectTagLeave->test(xmlParts);
		detectProcInstr->test(xmlParts);
		detectLeave->test(xmlParts);
	}
	std::cout << doc->toString() << std::endl;
}
#endif