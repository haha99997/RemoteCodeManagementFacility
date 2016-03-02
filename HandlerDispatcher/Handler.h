#pragma once
/////////////////////////////////////////////////////////////////////
// Handler.h   Handles send and receive process                    //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	This module is used for handling send and receive messages.
*	SendHandler is used for constructing message before it is put in queue of Sender.
*	RecvDispatcher is used for dispatching messages to concrete handler
*   They both have a invoke() and addCommand() method that can map string command
*   to concrete handler.
*
* Required Files:
* ---------------
* SendCommand.h(cpp), Sender.h(cpp), MsgMgr.h(cpp), Receiver.h(cpp), RecvHandler.h(cpp)
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

#include "ISendHandler.h"
#include "IRecvDispatcher.h"
#include "SendCommand.h"
#include "RecvHandler.h"
#include "../Sender/ISendr.h"
#include "../MsgMgr/MsgMgr.h"
#include "../Receiver/IRecvr.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <string>
#include <unordered_map>
#include <memory>

//SendHandler is used for constructing message before it is put in queue of Sender.
class SendHandler : public ISendHandler
{
public:
	SendHandler(ISendr& s) : sender(s){}														//Constructor
	void addCommand(std::string command, std::shared_ptr<SendCommand> processor);		//Add Command-Handler map relation
	void invoke(std::string command, std::string arg) override;									//Invoke command handler with a certain argument
	~SendHandler(){}
private:
	std::unordered_map<std::string, std::shared_ptr<SendCommand>> commands;
	ISendr &sender;
};

//RecvDispatcher is used for dispatching messages to concrete handler
class RecvDispatcher : public IRecvDispatcher
{
public:
	RecvDispatcher(IRecvr& r, ISendr& s);											//Constructor
	void addCommand(std::string command, std::shared_ptr<RecvHandler> processor);	//Add Command-Handler map relation
	void invoke(std::string command, std::string arg) override;								//Invoke command handler with a certain argument
	std::string getMessage() override;
	~RecvDispatcher();
private:
	BlockingQueue<std::string> blockQ;
	std::unordered_map<std::string, std::shared_ptr<RecvHandler>> commands;
	std::thread t;
	IRecvr &receiver;
	ISendr &sender;
};