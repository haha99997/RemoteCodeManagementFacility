/////////////////////////////////////////////////////////////////////
// Receiver.cpp    Receives message, put result into queue         //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "Receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"

using namespace ApplicationHelpers;

//Constructor
Receiver::Receiver(size_t port, Receiver::IpVer ipver)
{
	if (ipver == IPV4)
		sl = new SocketListener(port, Socket::IP4);
	else if (ipver == IPV6)
		sl = new SocketListener(port, Socket::IP6);
	sl->start(rh);
	Verbose::show("Receiver created", always);
}

//Get a message from blocking queue.
std::string Receiver::getMessage()
{
	return blockQ.deQ();
}

Receiver::~Receiver()
{
	sl->shutDown();
	sl->close();
	delete sl;
}

//Used as functor
void RequestHandler::operator()(Socket& socket_)
{
	while (true)
	{
		if (!CommandHandling(socket_))
		{
			Verbose::show("Closing Connection", always);
			break;
		}
	}
	Verbose::show("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	Verbose::show("ClientHandler thread terminating");
}

bool RequestHandler::UploadFileHandling(Socket& socket_)
{
	std::string filename = msgmgr.GetAttrbVal(ATTR_FILENAME);
	if (!file.is_open()){
		try{
			file.open(filename, std::ofstream::binary);
		}
		catch (std::exception&) {
			Verbose::show("Exception opening file " + filename);
			msgmgr.RemoveAttrb(ATTR_BLOCK_SIZE);
			msgmgr.AddAttrb(ATTR_UPLOAD_STATE, UPLOAD_FAIL);
			blockQ.enQ(msgmgr.CreateMessage());
		}
	}
	block_size = std::stoi(msgmgr.GetAttrbVal(ATTR_BLOCK_SIZE));
	if (!BufferHandling(socket_))
	{
		Verbose::show("Receive failed", always);
		if (file.is_open())
			file.close();
		return false;
	}
	if (block_size < BLOCK_SIZE)
	{
		file.close();
		msgmgr.RemoveAttrb(ATTR_BLOCK_SIZE);
		msgmgr.AddAttrb(ATTR_UPLOAD_STATE, UPLOAD_SUCESS);
		blockQ.enQ(msgmgr.CreateMessage());
	}
	return true;
}

//Handles string messages.
bool RequestHandler::CommandHandling(Socket& socket_)
{
	std::string str = socket_.recvString();
	if (socket_ == INVALID_SOCKET){
		Verbose::show("connection shutdown");
		if (file.is_open())
			file.close();
		return false;
	}
	if (str.size() > 0)
	{
		Verbose::show(str);
		msgmgr.ParseMessage(str);
		std::string command = msgmgr.GetAttrbVal(ATTR_COMMAND);
		if (command == UPLOAD_FILE || command == SERVER_UPLOAD_FILE)
			return UploadFileHandling(socket_);
		else if (command == QUIT)
		{
			blockQ.enQ(str);
			return false;
		}
		else
		{
			blockQ.enQ(str);
			return true;
		}
	}
	return false;
}

//Handles file upload.
bool RequestHandler::BufferHandling(Socket& socket_)
{
	if (block_size == 0)
		return true;
	char *buf = new char[block_size];
	bool ok;
	ok = socket_.recv(block_size, buf);
	if (ok && socket_ != INVALID_SOCKET)
	{
		file.write(buf, block_size);
		delete[]buf;
		return true;
	}
	else
	{
		Verbose::show("connection shutdown");
		if (file.is_open())
			file.close();
		delete[]buf;
		return false;
	}
}

// test stub
#ifdef TEST_RECEIVER

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

int main(int argc, char *argv[])
{
	try{
		Verbose verbose(true);
		Receiver receiver(9080, Receiver::IPV6);
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
	return 0;
}
#endif
