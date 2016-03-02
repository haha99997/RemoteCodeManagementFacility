#pragma once
/////////////////////////////////////////////////////////////////////
// SendCommand.h   Constructs send  messages                       //
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
	uf = new UploadFileSendCommand();
	ts = new TextSearchSendCommand();
	con = new ConnectSendCommand();
	close = new CloseSendCommand();
	Sender sender;
	con->Send(sender, "localhost:9080");
	uf->Send(sender, "../proxy2.txt");
	ts->Send(sender, "adsf");
	close->Send(sender, "");

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

#include "../Sender/ISendr.h"
#include "../MsgMgr/MsgMgr.h"

// Abstract class
class SendCommand
{
public:
	virtual void Send(ISendr& sender, std::string arg);
	virtual std::string buildMessage(std::string s) = 0;
	virtual ~SendCommand(){}
protected:
	static MsgMgr msgmgr;		//Shared by all derived classes
};

