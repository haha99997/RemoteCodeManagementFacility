#pragma once
/////////////////////////////////////////////////////////////////////
// ChannelFactory.h    Send Messages, include file upload          //
// Ver 1.0                                                         //
// Application: CSE687 Pr#4, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	This is a factory for creating Sender, Receiver, SendHandler, RecvDispatcher
*	instances wrapped in their interface pointers. It is used by ClientCLR, a
*	managed environment, to implement managed code running native code.

* Required Files:
* ---------------
*	ISendr.h, IRecvr.h, ISendHandler.h, IRecvDispatcher.h
*
* Build Process:
* --------------
* devenv RemoteCodeManagementFacility.sln /build Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 21 Apr 15
*   - first release
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include "../Sender/ISendr.h"
#include "../Receiver/IRecvr.h"
#include "../HandlerDispatcher/ISendHandler.h"
#include "../HandlerDispatcher/IRecvDispatcher.h"
#include <memory>

extern "C" {
	struct ChannelFactory
	{
		enum IpVer{IPV4, IPV6};
		DLL_DECL ISendr* CreateSender(const std::string& local_ip, size_t port);
		DLL_DECL IRecvr* CreateReceiver(size_t port, IpVer ipver = IPV6);
		DLL_DECL ISendHandler* CreateSendHandler(ISendr& sender);
		DLL_DECL IRecvDispatcher* CreateRecvDispatcher(ISendr& sender, IRecvr& receiver);
	};
}

