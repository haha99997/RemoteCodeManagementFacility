#pragma once
/////////////////////////////////////////////////////////////////////
// RecvHandler.h   Handles send and receive process                //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	This module is used for handling received messages.
*
*	UploadHandler - Handles upload message.
*	DownloadHandler - Handles download message.
*	TextSearchHandler - Handles text search message.
*	FileListHandler - Handles file list message.
*	FeedBackUploadHandler - Handles feedback message.
*
* Required Files:
* ---------------
* ISendr.h, Cpp11-BlockingQueue.h(cpp), FileStore.h(cpp), RecvHandler.h(cpp),
* XmlDocument.h(cpp)
*
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "HandlerDispatcher\HandlerDispatcher.vcxproj" /projectconfig Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include "RecvHandler.h"
#include "../FileCatalog/FileStore.h"
#include "../XmlDocument/XmlDocument.h"

//Handles upload message.
struct UploadHandler : RecvHandler
{
	UploadHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~UploadHandler(){ t.join(); }
};

struct ServerUploadHandler : RecvHandler
{
	ServerUploadHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~ServerUploadHandler(){ t.join(); }
};

//Handles download message.
struct DownloadHandler : RecvHandler
{
	DownloadHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~DownloadHandler(){ t.join(); }
};

//Handles text search message.
struct TextSearchHandler : RecvHandler
{
	TextSearchHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~TextSearchHandler(){ t.join(); }
private:
	XmlProcessing::XmlDocument* getXmlFileList();
	BlockingQueue<std::string> matched_files;
};

//Handles file list message.
struct FileListHandler : RecvHandler
{
	FileListHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~FileListHandler(){ t.join(); }
};

//Handles feedback message.
struct FeedBackUploadHandler : RecvHandler
{
	FeedBackUploadHandler(ISendr& s) : RecvHandler(s){}
	virtual void Process(const std::string& msg) override;
	virtual ~FeedBackUploadHandler(){ t.join(); }
};