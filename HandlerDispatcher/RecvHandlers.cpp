/////////////////////////////////////////////////////////////////////
// RecvHandlers.cpp   Handles send and receive process             //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "RecvHandlers.h"
#include "../MsgMgr/MsgType.h"
#include "../MsgMgr/MsgMgr.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../FileCatalog/FileMgr.h"
#include "../XmlDocument/XmlDocument.h"
#include "ThreadPool.h"
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace ApplicationHelpers;

//Handles upload message.
void UploadHandler::Process(const std::string& msg)
{
	MsgMgr msgmgr;
	msgmgr.ParseMessage(msg);
	std::string file = msgmgr.GetAttrbVal(ATTR_FILENAME);
	std::string state = msgmgr.GetAttrbVal(ATTR_UPLOAD_STATE);
	std::string srcip = msgmgr.GetAttrbVal(ATTR_SRCIP);
	std::string srcport = msgmgr.GetAttrbVal(ATTR_SRCPORT);

	msgmgr.Init();
	msgmgr.AddAttrb(ATTR_DESTIP, srcip);
	msgmgr.AddAttrb(ATTR_DESTPORT, srcport);
	msgmgr.AddAttrb(ATTR_COMMAND, FEEDBACK_UPLOAD);
	msgmgr.AddAttrb(ATTR_FILENAME, file);
	msgmgr.AddAttrb(ATTR_UPLOAD_STATE, state);
	sender.sendMessage(msgmgr.CreateMessage());

	Verbose::show("Feedback: Upload " + file + "\tState: " + state + " to " + srcip + ":" + srcport, always);
}

//Handles server upload message.
void ServerUploadHandler::Process(const std::string& msg)
{
	// no need to feedback to server. If needed, code can be added here.
}

//Handles download message.
void DownloadHandler::Process(const std::string& msg)
{
	MsgMgr msgmgr;
	msgmgr.ParseMessage(msg);
	std::string file = msgmgr.GetAttrbVal(ATTR_FILENAME);
	std::string srcip = msgmgr.GetAttrbVal(ATTR_SRCIP);
	std::string srcport = msgmgr.GetAttrbVal(ATTR_SRCPORT);

	msgmgr.Init();
	msgmgr.AddAttrb(ATTR_DESTIP, srcip);
	msgmgr.AddAttrb(ATTR_DESTPORT, srcport);
	msgmgr.AddAttrb(ATTR_COMMAND, SERVER_UPLOAD_FILE);
	msgmgr.AddAttrb(ATTR_FILENAME, file);
	sender.sendMessage(msgmgr.CreateMessage());

	Verbose::show("Download: " + file + "  Uploading:" + file + " to " + srcip + ":" + srcport, always);
}

// split a string by a delimiter.
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

// split a string by a delimiter.
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

//Get files from a path with patterns seperated by '|'
std::vector<std::string> getFiles(const std::string& path, const std::string& patterns_str)
{
	FileStore filestore;
	FileMgr filemgr(filestore);
	filemgr.setRecurse(true);
	std::string current_path = filemgr.getCurrentDirectory();
	auto patterns = split(patterns_str, '|');
	filemgr.addPatterns(patterns);
	filemgr.findFiles(path);
	filemgr.setCurrentDirectory(current_path);
	return filestore.getFiles();
}

// Get file list stored in XmlDocument
XmlProcessing::XmlDocument* TextSearchHandler::getXmlFileList()
{
	XmlProcessing::XmlDocument *xdoc = new XmlProcessing::XmlDocument;
	xdoc->addRoot(nullptr);
	auto declr = XmlProcessing::makeXmlDeclarElement();
	declr->addAttrib("version", "1.0");
	xdoc->root()->addChild(declr);
	auto root = XmlProcessing::makeTaggedElement("MatchedFiles");
	while (matched_files.size() != 0)
	{
		auto item = XmlProcessing::makeTaggedElement("File");
		std::string file = matched_files.deQ();
		auto text = XmlProcessing::makeTextElement(file);
		item->addChild(text);
		root->addChild(item);
	}
	xdoc->root()->addChild(root);
	return xdoc;
}

//Handles text search message.
void TextSearchHandler::Process(const std::string& msg)
{
	MsgMgr msgmgr; msgmgr.ParseMessage(msg);
	std::string srcip = msgmgr.GetAttrbVal(ATTR_SRCIP);
	std::string srcport = msgmgr.GetAttrbVal(ATTR_SRCPORT);
	std::string pattern_str = msgmgr.GetAttrbVal(ATTR_PATTERN_TEXT);
	std::string file_patterns_str = msgmgr.GetAttrbVal(ATTR_PATTERN_FILE);
	std::string path = msgmgr.GetAttrbVal(ATTR_PATH);
	std::string thread_cnt = msgmgr.GetAttrbVal(ATTR_THREAD_COUNT);
	std::string id = msgmgr.GetAttrbVal(ATTR_TEXT_SEARCH_ID);
	msgmgr.Init();
	msgmgr.AddAttrb(ATTR_COMMAND, FEEDBACK_TEXTSEARCH);
	msgmgr.AddAttrb(ATTR_DESTIP, srcip);
	msgmgr.AddAttrb(ATTR_DESTPORT, srcport);
	msgmgr.AddAttrb(ATTR_TEXT_SEARCH_ID, id);
	auto files = getFiles(path, file_patterns_str);
	ThreadPool pool(std::stoi(thread_cnt));
	std::vector<std::future<void>> result;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	try{
		std::regex rx(pattern_str);
		for (auto& file : files)
			result.push_back(pool.enqueue([&]{std::ifstream ifs(file); std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>())); if (std::regex_search(content.begin(), content.end(), rx)) matched_files.enQ(file); }));
		for (auto& r : result) 
			r.get();
	}
	catch (std::exception&)
	{
		msgmgr.SetBody("<?xml version=\"1.0\"?><Files><File>Malformed Regex</File></Files>");
		sender.sendMessage(msgmgr.CreateMessage());
		return;
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	msgmgr.AddAttrb(ATTR_SEARCH_TIME, std::to_string(time_span.count()));
	XmlProcessing::XmlDocument *xdoc = getXmlFileList();
	msgmgr.SetBody(xdoc->root()->toString());
	delete xdoc;
	sender.sendMessage(msgmgr.CreateMessage());
}

//Handles file list message.
void FileListHandler::Process(const std::string& msg)
{
	MsgMgr msgmgr;
	msgmgr.ParseMessage(msg);
	std::string srcip = msgmgr.GetAttrbVal(ATTR_SRCIP);
	std::string srcport = msgmgr.GetAttrbVal(ATTR_SRCPORT);
	std::string patterns_str = msgmgr.GetAttrbVal(ATTR_PATTERN_FILE);
	std::string path = msgmgr.GetAttrbVal(ATTR_PATH);

	auto files = getFiles(path, patterns_str);
	XmlProcessing::XmlDocument xdoc;
	xdoc.addRoot(nullptr);
	auto declr = XmlProcessing::makeXmlDeclarElement();
	declr->addAttrib("version", "1.0");
	xdoc.root()->addChild(declr);
	auto root = XmlProcessing::makeTaggedElement("Files");
	for (auto& file : files)
	{
		auto item = XmlProcessing::makeTaggedElement("File");
		auto text = XmlProcessing::makeTextElement(file);
		item->addChild(text);
		root->addChild(item);
	}
	xdoc.root()->addChild(root);
	msgmgr.Init();
	msgmgr.AddAttrb(ATTR_COMMAND, FEEDBACK_FILELIST);
	msgmgr.AddAttrb(ATTR_DESTIP, srcip);
	msgmgr.AddAttrb(ATTR_DESTPORT, srcport);
	msgmgr.SetBody(xdoc.root()->toString());
	sender.sendMessage(msgmgr.CreateMessage());
}

//Handles feedback message.
void FeedBackUploadHandler::Process(const std::string& msg)
{
	MsgMgr msgmgr;
	msgmgr.ParseMessage(msg);
	std::string file = msgmgr.GetAttrbVal(ATTR_FILENAME);
	std::string state = msgmgr.GetAttrbVal(ATTR_UPLOAD_STATE);
	Verbose::show("Upload " + file + "\tState: " + state, always);
}

// test stub
#ifdef TEST_RECVHANDLER
int main()
{
	std::shared_ptr<RecvHandler> upload, download, textsearch, filelist, upload_feedback;
	upload = std::make_shared<UploadHandler>(sender);
	download = std::make_shared<DownloadHandler>(sender);
	textsearch = std::make_shared<TextSearchHandler>(sender);
	filelist = std::make_shared<FileListHandler>(sender);
	upload_feedback = std::make_shared<FeedBackUploadHandler>(sender);
	return 0;
}
#endif