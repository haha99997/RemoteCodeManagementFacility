///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.3                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stack>
#include "XmlDocument.h"

using namespace XmlProcessing;

// Constructor
XmlDocument::XmlDocument(const std::string& src, bool isFile)
{
	XmlBuilder xmlBuilder(src, isFile);
	pDocElement_ = xmlBuilder.buildXmlTree();
	doc = std::make_shared<XmlDocument>(*this);
}

// Copy constructor
XmlDocument::XmlDocument(const XmlDocument& other) : found_(other.found_)
{
	doc = other.doc;
	pDocElement_ = other.pDocElement_;
}

// Move constructor
XmlDocument::XmlDocument(XmlDocument&& other)
{
	doc = std::move(other.doc);
	pDocElement_ = std::move(other.pDocElement_);
	found_ = std::move(other.found_);
}

// Copy assignment
XmlDocument& XmlDocument::operator=(const XmlDocument& other)
{
	if (this == &other) return *this;
	doc = other.doc;
	pDocElement_ = other.pDocElement_;
	found_ = (other.found_);
	return *this;
}

// Move assignment
XmlDocument& XmlDocument::operator=(XmlDocument&& other)
{
	if (this == &other) return *this;
	doc = std::move(other.doc);
	pDocElement_ = std::move(other.pDocElement_);
	found_ = std::move(other.found_);
	return *this;
}

// Add a root to the document
void XmlDocument::addRoot(const std::shared_ptr<AbstractXmlElement>& element)
{
	if (element == nullptr)
		pDocElement_ = makeDocElement();
	else
		pDocElement_ = element;
}

// Get single element
XmlDocument::xDocPtr XmlDocument::element(const std::string& tag)
{
	std::stack<sPtr> dfsStack;
	auto children = pDocElement_->getChildren();
	for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
		dfsStack.push(*iter);
	while (!dfsStack.empty())
	{
		sPtr elem = dfsStack.top();
		dfsStack.pop();
		if (elem->getTag() == tag)
		{
			found_.push_back(elem);
			doc.reset(new XmlDocument(elem));
			return doc;
		}
		auto children = elem->getChildren();
		for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
			dfsStack.push(*iter);
	}
	sPtr tmp = nullptr;
	doc.reset(new XmlDocument(tmp));
	return doc;
}

// Test if an element contains a certain attribute
bool XmlDocument::containAttrib(const std::vector<std::pair<std::string, std::string>>& attribs,
	const std::string& attr, const std::string& value)
{
	for (auto iter = attribs.begin(); iter != attribs.end(); ++iter)
	{
		if (iter->first == attr && iter->second == value)
			return true;
	}
	return false;
}

// Get element by id
XmlDocument::xDocPtr XmlDocument::elementById(const std::string& attrib, const std::string& value)
{
	std::stack<sPtr> dfsStack;
	auto children = pDocElement_->getChildren();
	for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
		dfsStack.push(*iter);
	while (!dfsStack.empty())
	{
		sPtr elem = dfsStack.top();
		dfsStack.pop();
		auto attribs = elem->getAttribs();
		if (containAttrib(elem->getAttribs(), attrib, value))
		{
			found_.push_back(elem);
			doc.reset(new XmlDocument(elem));
			return doc;
		}
		auto children = elem->getChildren();
		for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
			dfsStack.push(*iter);
	}
	sPtr tmp = nullptr;
	doc.reset(new XmlDocument(tmp));
	return doc;
}

// Get direct children
XmlDocument::xDocPtr XmlDocument::children(const std::string& tag)
{
	std::stack<sPtr> dfsStack;
	auto children = pDocElement_->getChildren();
	if (tag == "")
		found_ = std::move(children);
	else
	{
		for (sPtr elem : children)
			if (elem->getTag() == tag)
				found_.push_back(elem);
	}
	sPtr root = makeTaggedElement("");
	for (sPtr elem : found_)
		root->addChild(elem);
	doc.reset(new XmlDocument(root));
	return doc;
}

// Get decendents
XmlDocument::xDocPtr XmlDocument::descendents(const std::string& tag)
{
	std::stack<sPtr> dfsStack;
	auto children = pDocElement_->getChildren();
	for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
		dfsStack.push(*iter);
	while (!dfsStack.empty())
	{
		sPtr elem = dfsStack.top();
		dfsStack.pop();
		if (tag == "" || (tag != "" && elem->getTag() == tag))
		{
			found_.push_back(elem);
		}
		auto children = elem->getChildren();
		for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
			dfsStack.push(*iter);
	}
	sPtr root = makeTaggedElement("");
	for (sPtr elem : found_)
		root->addChild(elem);
	doc.reset(new XmlDocument(root));
	return doc;
}

// open a file and construct document
void XmlDocument::open(const std::string& src, bool isFile)
{
	XmlBuilder xmlBuilder(src, isFile);
	pDocElement_ = xmlBuilder.buildXmlTree();
	doc = std::make_shared<XmlDocument>(*this);
}

// write current tree to file
void XmlDocument::save(const std::string& fileSpec)
{
	std::ofstream ofs;
	ofs.open(fileSpec, std::ofstream::out);
	ofs << doc->root()->toString();
	ofs.close();
}

// Test stub
#ifdef TEST_XMLDOCUMENT
int main()
{
	title("Testing XmlDocument class");

	XmlDocument doc("LectureNote.xml");

	std::cout << doc.root()->toString() << std::endl;
	std::cout << doc.element("reference")->children()->root()->toString() << std::endl;
	std::cout << doc.element("LectureNote")->descendents()->root()->toString() << std::endl;
	std::cout << typeid(*(doc.element("note")->root())).name() << std::endl;
	doc.save("result.txt");
}
#endif