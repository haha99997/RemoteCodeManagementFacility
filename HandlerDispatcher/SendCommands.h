#pragma once

/////////////////////////////////////////////////////////////////////
// SendCommands.h   Constructs send  messages                      //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	This module is used for constructing send messages.

*   Operation example:
*	SendCommand *uf, *ts, *con, *close;
*	uf = new UploadFileSendCommand();
*	ts = new TextSearchSendCommand();
*	con = new ConnectSendCommand();
*	close = new CloseSendCommand();
*	Sender sender;
*	con->Send(sender, "localhost:9080");
*	uf->Send(sender, "../proxy2.txt");
*	ts->Send(sender, "adsf");
*	close->Send(sender, "");

* Required Files:
* ---------------
* ISendr.h(cpp), MsgMgr.h(cpp)
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

#include "SendCommand.h"

// Constructing CONNECT msg
struct ConnectSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~ConnectSendCommand(){}
private:
	MsgMgr mm;
};

//Construct CLOSE msg
struct CloseSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~CloseSendCommand(){}
private:
	MsgMgr mm;
};

//Construct UPLOAD_FILE msg
struct UploadFileSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~UploadFileSendCommand(){}
private:
	MsgMgr mm;
};

//Construct DOWNLOAD_FILE msg
struct DownloadFileSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~DownloadFileSendCommand(){}
private:
	MsgMgr mm;
};

//Construct TEXT_SEARCH msg
struct TextSearchSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~TextSearchSendCommand(){}
private:
	MsgMgr mm;
};

//Construct FILE_SEARCH msg
struct FileSearchSendCommand : SendCommand
{
	virtual std::string buildMessage(std::string s) override;
	virtual ~FileSearchSendCommand(){}
private:
	MsgMgr mm;
};

