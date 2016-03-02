#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 1.3                                                       //
// Application: CSE687 Pr#2, Spring 2015                         //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Chao Zuo                                         //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to help students in CSE687 - Object Oriented Design
* get started with Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv XMLDocumentModel.sln /debug rebuild /Project XmlDocument
*
* Maintenance History:
* --------------------
* ver 1.3 : 13 Mar 15
* - Completed missing methods
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*
* ToDo:
* -----
* This is the beginning of an XmlDocument class for Project #2.
* It lays out a suggested design, which you are not compelled to follow.
* If it helps then use it.  If not you don't have to.
*
* Note that I've simply roughed in a design that is similar to that
* used in the .Net Framework XDocument class.
*/

#include <memory>
#include <string>
#include "../XmlElement/XmlElement.h"
#include "../XmlBuilder/XmlBuilder.h"

namespace XmlProcessing
{
	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement > ;
		using xDocPtr = std::shared_ptr < XmlDocument > ;
		XmlDocument(){};
		XmlDocument(const std::string& src, bool isFile = true);
		XmlDocument(const std::shared_ptr<AbstractXmlElement>& element) : pDocElement_(element){ doc = std::make_shared<XmlDocument>(*this); }
		XmlDocument(const XmlDocument& other);
		XmlDocument(XmlDocument&& other);
		XmlDocument& operator=(const XmlDocument& other);
		XmlDocument& operator=(XmlDocument&& other);
		void open(const std::string& src, bool isFile = true);
		void save(const std::string& fileSpec);
		void addRoot(const std::shared_ptr<AbstractXmlElement>& element = nullptr);
		// queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();
		xDocPtr element(const std::string& tag);           // found_[0] contains first element (DFS order) with tag
		xDocPtr elements(const std::string& tag) { return descendents(tag); }          // found_ contains all elements with tag
		xDocPtr elementById(const std::string& attrib, const std::string& value);
		xDocPtr children(const std::string& tag = "");     // found_ contains sPtrs to children of prior found_[0] 
		xDocPtr descendents(const std::string& tag = "");  // found_ contains sPtrs to descendents of prior found_[0]
		std::string toString() { return pDocElement_->toString(); }
		sPtr root() { return pDocElement_; }
	private:
		bool containAttrib(const std::vector<std::pair<std::string, std::string>>& attribs,
			const std::string& attr, const std::string& value);
		std::shared_ptr<XmlDocument> doc;
		sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
		std::vector<sPtr> found_;  // query results
	};
}
#endif
