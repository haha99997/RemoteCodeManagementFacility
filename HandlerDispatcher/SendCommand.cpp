/////////////////////////////////////////////////////////////////////
// SendCommand.h   Constructs send  messages                       //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "SendCommand.h"

MsgMgr SendCommand::msgmgr;		// Static declaration

//Send
void SendCommand::Send(ISendr& sender, std::string arg)
{
	std::string msg = buildMessage(arg);
	sender.sendMessage(msg);
}

