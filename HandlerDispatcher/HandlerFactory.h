#pragma once
/////////////////////////////////////////////////////////////////////
// HandlerFactory.h    Creates SendHandler and RecvDispatcher      //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module contains two factories that constructs SendHandler and RecvDispatcher
*
* Required Files:
* ---------------
* Handler.h(cpp)
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "HandlerFactory\HandlerFactory.vcxproj" /projectconfig Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include "ISendHandler.h"
#include "IRecvDispatcher.h"
#include "../Sender/ISendr.h"
#include "../Receiver/IRecvr.h"

// Factory for creating SendHandler
class SendHandlerFactory
{
public:
	ISendHandler* Create(ISendr& sender);							//Factory Create
	~SendHandlerFactory(){}
};

// Factory for Creating RecvDispatcher
class RecvDispatcherFactory
{
public:
	IRecvDispatcher* Create(ISendr& sender, IRecvr& receiver);		//Factory Create
	~RecvDispatcherFactory(){}
};
