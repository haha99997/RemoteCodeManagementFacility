#pragma once
/////////////////////////////////////////////////////////////////////
// ISendHandler.h   Interface for SendHandler                      //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/**
* Module Operations:
* ------------------
* This module provides interface for SendHandler created by HandlerFactory.
*
* Required Files:
* ---------------
* None
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "HandlerDispatcher\HandlerDispatcher.vcxproj" /projectconfig Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 21 Apr 15
*   - first release
*/

#include <string>
#include <memory>
#include "SendCommand.h"

struct ISendHandler
{
	virtual void invoke(std::string command, std::string arg) = 0;								//Invoke command handler with a certain argumen
};