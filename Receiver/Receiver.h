#pragma once
/////////////////////////////////////////////////////////////////////
// Receiver.h    Receives message, put result into queue           //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
*	Receiver is used for listening to a port. When a connection reaches, it spawns a 
* child thread and uses RequestHandler to handle messages. Each connection have their
* own RequestHandler.

* Required Files:
* ---------------
* QServer.h(cpp), Sockets.h(cpp), MsgMgr.h(cpp), MsgMgr.h(cpp), MsgType.h(cpp)
*
* Build Process:
* --------------
* devenv MsgPassComm.sln /build Debug /project "Receiver\Receiver.vcxproj" /projectconfig Debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include "../QServer/QServer.h"
#include "../Sockets/Sockets.h"
#include "../MsgMgr/MsgMgr.h"
#include "../MsgMgr/MsgType.h"
#include "IRecvr.h"
#include <fstream>
#include <unordered_set>
#include <string>

// Handles messages, including headers and body(string or byte array)
class RequestHandler
{
public:
	RequestHandler() : blockQ(QServer<std::string, 0>().get()){}				//Constructor
	RequestHandler(const RequestHandler& other) : blockQ(other.blockQ) {}		//Copy constructor
	void operator()(Socket& socket_);											//Used as functor
	bool CommandHandling(Socket& socket_);										//Handles string messages.
	bool BufferHandling(Socket& socket_);										//Handles file upload.
private:
	bool UploadFileHandling(Socket& socket_);
	BlockingQueue<std::string> &blockQ;
	MsgMgr msgmgr;
	std::ofstream file;
	int block_size;
	std::unordered_set<std::string> file_open;
};

class Receiver : public IRecvr
{
public:
	enum IpVer {IPV4, IPV6};
	Receiver(size_t port, Receiver::IpVer ipver = IPV6);								//Constructor
	std::string getMessage() override;													//Get a message from blocking queue.
	~Receiver();
private:
	SocketSystem ss;
	SocketListener *sl;
	RequestHandler rh;
	BlockingQueue<std::string> &blockQ = QServer<std::string, 0>().get();
};
