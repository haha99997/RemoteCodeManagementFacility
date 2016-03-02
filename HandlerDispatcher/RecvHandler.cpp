/////////////////////////////////////////////////////////////////////
// RecvHandler.cpp   Handles send and receive process              //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
#include "RecvHandler.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../MsgMgr/MsgType.h"
#include <thread>

using namespace ApplicationHelpers;

//Abstract class for handling message.
RecvHandler::RecvHandler(ISendr& s) : sender(s)
{
	t = std::thread([&]{
		while (true)
		{
			std::string msg = blockQ.deQ();
			if (msg == ATTR_COMMAND + ":" + QUIT) break;
			Process(msg);
		}
	});
	t.detach();
}
