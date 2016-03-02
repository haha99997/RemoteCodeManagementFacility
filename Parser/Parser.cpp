///////////////////////////////////////////////////////////////////
// Parser.cpp - Parser detects code constructs defined by rules  //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

#include "Parser.h"
#include "../XmlBuilder/Repository.h"
#include "../XmlElementParts/Tokenizer.h"
#include "../XmlElementParts/xmlElementParts.h"

// parse toks
void Parser::parse(ITokCollection& semi)
{
	for(IRule* rule : rules)
	{
		if (rule->test(semi))
			break;
	}
}

/************test stub***************/

#ifdef TEST_PARSER
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

	Parser parser;
	parser.add(detectComment.get());
	parser.add(detectDeclar.get());
	parser.add(detectText.get());
	parser.add(detectTag.get());
	parser.add(detectTagLeave.get());
	parser.add(detectProcInstr.get());
	parser.add(detectLeave.get());

	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
	std::string test2 = Prologue;
	test2 += "<Dir><path>/TestFile/Dir1</path><File date='notCurrent'><name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";
	Toker toker("LectureNote.xml");
	toker.setMode(Toker::xml);
	XmlParts xmlParts(&toker);
	auto doc = XmlProcessing::makeDocElement(nullptr);
	repo.stack().push(doc);
	while (xmlParts.get())
	{
		std::cout << xmlParts.show().c_str() << std::endl;
		parser.parse(xmlParts);
	}

	std::cout << doc->toString() << std::endl;
}
#endif
