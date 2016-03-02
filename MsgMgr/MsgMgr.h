#pragma once

/////////////////////////////////////////////////////////////////////
// MsgMgr.h    Constructs, parses and handles messages             //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module constructs messages in the following manner:
*    MsgMgr m;
*    std::string Prologue = "<?xml version=\"1 . 0 / 0\"?><?xml-stylesheet type=\"text / xsl\" href=\"ivy - report.xsl\"?><!-- top level comment -->";
*    std::string test1 = Prologue;
*    test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
*    m.addAttrb("ATTR_COMMAND", "UPLOAD_FILE");
*    m.addAttrb("ATTR_DESTIP", "123.32.54.8");
*    m.addAttrb("ATTR_DESTPORT", "12423");
*    m.setBody(test1);						// Optional
*	 std::string msg = m.CreateMessage();
*
*  Parse Message:
*    mm.ParseMessage(msg);
*    mm.addAttrb("BufLength", "1024");
*	 String cmd = mm.getAttrbVal("ATTR_COMMAND");
*
* Required Files:
* ---------------
* None
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "MsgMgr\MsgMgr.vcxproj" /projectconfig Debug 
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include <string>
#include <vector>
#include <unordered_map>

// Message Manager. Constructs, modifies and parses messages.
class MsgMgr
{
public:
	MsgMgr(){}												//default constructor
	MsgMgr(const MsgMgr& other) : body(other.body), 
		header(other.header), attrbs(other.attrbs){}		// Copy constructor
	std::string CreateMessage();							//Creates a msg after adding certain attributes.
	void Init();											//Initialize. Set all fields empty.
	void ParseMessage(const std::string& msg);						// Parses a message for query, modification and creation
	void AddAttrb(const std::string& attr, const std::string& value);		// add an attribute-value 
	bool RemoveAttrb(const std::string& attr);						// remove an attribute-value pair by attribute
	std::string GetAttrbVal(const std::string& attr);				// get the value of attribute-value pair by attribute
	static std::pair<std::string, std::string> ParseAttrb(const std::string& line);	//Parse a line to attribute-value pair
	void SetBody(const std::string& text);						// Set message body
	std::string GetBody();									// Get message body
	std::string GetHeader();								// Get message header. 
	std::vector<std::string> GetHeaderAttribs();			// Get attributes in each line
	~MsgMgr(){};
private:
	std::string body;
	std::string header;
	std::unordered_map<std::string, std::string> attrbs;
};

