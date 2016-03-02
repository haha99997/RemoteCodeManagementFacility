/////////////////////////////////////////////////////////////////////
// MsgMgr.cpp    Constructs, parses and handles messages           //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "MsgMgr.h"
#include "MsgType.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

//Creates a msg after adding certain attributes.
std::string MsgMgr::CreateMessage()
{
	return header + "\n" + body;
}

void MsgMgr::Init()
{
	header = "";
	body = "";
	attrbs.clear();
}

// Parse message
void MsgMgr::ParseMessage(const std::string& msg)
{
	Init();
	std::istringstream ss(msg);
	std::string line;
	while (true)
	{
		std::getline(ss, line);
		if (line == "") break;
		header += line + "\n";
		auto attrb = MsgMgr::ParseAttrb(line);
		attrbs.insert(attrb);
	}
	char c;
	while ((c = ss.get()) != EOF)
		body += c;
}

// add one attribute
void MsgMgr::AddAttrb(const std::string& attr, const std::string& value)
{
	header += attr + ":" + value + "\n";
	attrbs[attr] = value;
}

// remove an attribute-value pair by attribute
bool MsgMgr::RemoveAttrb(const std::string& attr)
{
	std::stringstream ss(header);
	std::string line;
	auto iter = attrbs.find(attr);
	if (iter == attrbs.end())
		return false;
	attrbs.erase(iter);
	while (true)
	{
		std::getline(ss, line);
		if (line == "") return false;
		int pos1 = 0, pos2 = line.find_first_of(":") - 1;
		while (isspace(line[pos1])) ++pos1;
		while (isspace(line[pos2])) --pos2;
		if (line.substr(pos1, pos2 + 1) == attr)
		{
			ss.clear();
			int pos = header.find(line);
			header.erase(pos, line.length() + 1);
			return true;
		}
	}
	return false;
}

// get value of a certain attribute
std::string MsgMgr::GetAttrbVal(const std::string& attr)
{
	auto result = attrbs.find(attr);
	if (result == attrbs.end())
		return "";
	else return result->second;
}

//Parse a line to attribute-value pair
std::pair<std::string, std::string> MsgMgr::ParseAttrb(const std::string& line)
{
	std::string first, second;
	int pos = line.find_first_of(":");
	if (pos == -1) throw std::runtime_error("malformed message");
	int pos1, pos2;
	pos1 = pos - 1;
	pos2 = pos + 1;
	while (isspace(line.at(pos1))) pos1--;
	while (isspace(line.at(pos2))) pos2++;
	first = line.substr(0, pos1 + 1);
	second = line.substr(pos2);
	return std::make_pair(first, second);
}

// Set message body
void MsgMgr::SetBody(const std::string& text)
{
	body = text;
}

// Get message body
std::string MsgMgr::GetBody()
{
	return body;
}

// Get message header.
std::string MsgMgr::GetHeader()
{
	return header;
}

// Get attributes in each line
std::vector<std::string> MsgMgr::GetHeaderAttribs()
{
	std::vector<std::string> result;
	std::istringstream ss(header);
	std::string line;
	while (true)
	{
		std::getline(ss, line);
		if (line == "") return std::move(result);
		result.push_back(line);
	}
}

// test stub
#ifdef TEST_MSGMGR
int main()
{
	MsgMgr m, mm;
	std::string Prologue = "<?xml version=\"1 . 0 / 0\"?>\n<?xml-stylesheet type=\"text / xsl\" href=\"ivy - report.xsl\"?>\n<!-- top level comment -->\n\n";
	std::string test1 = Prologue;
	test1 += "<parent att1='val1' att2='val2'>\n<child1 />\n<child2>\nchild2 body\n<child1 />\n</child2>\n</parent>\n";
	m.AddAttrb("ATTR_COMMAND", "UPLOAD_FILE");
	m.AddAttrb("ATTR_DESTIP", "123.32.54.8");
	m.AddAttrb("ATTR_DESTPORT", "12423");
	m.SetBody(test1);						// Optional
	std::string msg = m.CreateMessage();
	mm.ParseMessage(msg);
	mm.AddAttrb("BufLength", "1024");
	auto attrbs = mm.GetHeaderAttribs();
	mm.RemoveAttrb("BufLength");
	std::cout << mm.GetHeader() << mm.GetBody() << std::endl;
	return 0;
}
#endif
