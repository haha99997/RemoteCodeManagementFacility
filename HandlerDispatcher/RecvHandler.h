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
*	This module is used for handling received messages. It is used together
*	with RecvHandlers.
*	
*	RecvHandler - Abstract class for handling message.
*	---------(Following classes are defined in RecvHandlers.h)--------------
*	UploadHandler - Handles upload message.
*	DownloadHandler - Handles download message.
*	TextSearchHandler - Handles text search message.
*	FileListHandler - Handles file list message.
*	FeedBackUploadHandler - Handles feedback message.
*
* Required Files:
* ---------------
* ISendr.h, Cpp11-BlockingQueue.h(cpp)
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

#include <string>
#include "../Sender/ISendr.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

//Abstract class for handling message.
class RecvHandler
{
public:
	RecvHandler(ISendr& s);
	void enQ(std::string msg) { blockQ.enQ(msg); }
	virtual void Process(const std::string& msg) = 0;
	virtual ~RecvHandler(){}
protected:
	BlockingQueue<std::string> blockQ;
	std::thread t;
	ISendr& sender;
};
