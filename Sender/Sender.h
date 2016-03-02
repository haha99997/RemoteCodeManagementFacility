#pragma once
/////////////////////////////////////////////////////////////////////
// Sender.h    Send Messages, include file upload                  //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	Sender is used for sending messages. It has a queue for storing messages to be sent.
	A special type of message is file upload message. To upload a file, we need to send
	multiple blocks.

* Module Operation:
  -----------------
  All we need to do is to call sendMessage(). Child thread will automatically deal with dequeing
  and sending.
  	Sender sender("localhost", 9070);			//Sender is created with Receiver listening to 9070;
	sender.sendMessage("test");

* Required Files:
* ---------------
* Cpp11-BlockingQueue.h(cpp), Sockets.h(cpp), MsgMgr.h(cpp)
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "Sender\Sender.vcxproj" /projectconfig Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../MsgMgr/MsgMgr.h"
#include "ISendr.h"

class Sender : public ISendr
{
public:
	enum IpVer {IPV4, IPV6};

	Sender(std::string local_ip, size_t listen_port);

	// disable copy construction and assignment
	Sender(const Sender& s) = delete;
	Sender& operator=(const Sender& s) = delete;

	Sender& operator=(Sender&& s);
	void CreateSendChannel(std::string ip, size_t port);			//Connect to an address
	void sendMessage(const std::string& msg) override;				//Enqueue a message.
	~Sender();
private:
	void ThreadProc();
	void sendFile(MsgMgr& msgmgr);
	std::thread thread;
	BlockingQueue<std::string> blockQ;
	SocketSystem ss;
	SocketConnecter sc;
	int try_count = 0;
	std::string __local_ip;
	size_t __listen_port;
};

