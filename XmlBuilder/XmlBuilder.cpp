///////////////////////////////////////////////////////////////////
// XmlBuilder.cpp - Construct Xml Parse Tree                     //
// ver 1.0                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

#include "XmlBuilder.h"

// constructor
XmlBuilder::XmlBuilder(const std::string& src, bool isFile)
{
	toker = new Toker(src, isFile);
	toker->setMode(Toker::xml);
	parts = new XmlParts(toker);
	initialize();
}

// initialize the XmlBuilder
void XmlBuilder::initialize()
{
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

	parser.add(detectComment.get());
	parser.add(detectDeclar.get());
	parser.add(detectText.get());
	parser.add(detectTag.get());
	parser.add(detectTagLeave.get());
	parser.add(detectProcInstr.get());
	parser.add(detectLeave.get());
}

// Destructor
XmlBuilder::~XmlBuilder()
{
	delete toker;
	delete parts;
}

// Build tree
XmlBuilder::sPtr XmlBuilder::buildXmlTree()
{
	auto doc = XmlProcessing::makeDocElement(nullptr);
	repo.stack().push(doc);
	while (parts->get())
		parser.parse(*parts);
	repo.stack().pop();
	return doc;
}

// --------------<test stub>----------------
#ifdef TEST_XMLBUILDER
int main()
{
	XmlBuilder builder("LectureNote.xml");
	auto result = builder.buildXmlTree();
	std::cout << result->toString() << std::endl;
}
#endif