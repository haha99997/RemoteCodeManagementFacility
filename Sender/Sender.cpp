/////////////////////////////////////////////////////////////////////
// Sender.cpp    Send Messages, include file upload                //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <thread>
#include <stdexcept>
#include "Sender.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../FileSystem/FileSystem.h"
#include "../MsgMgr/MsgType.h"

using namespace ApplicationHelpers;

#pragma warning(disable: 4244)

//Constructor
Sender::Sender(std::string local_ip, size_t listen_port) : __local_ip(local_ip), __listen_port(listen_port)
{
	thread = std::thread(&Sender::ThreadProc,  this);
	thread.detach();
	Verbose::show("Sender created", always);
}

//Connect to an address
void Sender::CreateSendChannel(std::string ip, size_t port)
{
	sc.close();
	while (!sc.connect(ip, port))
	{
		++try_count;
		ApplicationHelpers::Verbose::show("client waiting to connect");
		::Sleep(100);
		if (try_count > CONNECT_TRY)
			throw std::runtime_error(("\nConnect to " + ip + ":" + std::to_string(port) + " failed").c_str());
	}
	Verbose::show("Successfully connected to " + ip + ":" + std::to_string(port) + "\n", always);
}

//Enqueue a msg
void Sender::sendMessage(const std::string& msg)
{
	blockQ.enQ(msg);
}

//Process UPLOAD_FILE command
void Sender::sendFile(MsgMgr& msgmgr)
{
	std::string fileSpec = msgmgr.GetAttrbVal(ATTR_FILENAME);
	std::ifstream is(fileSpec, std::ifstream::binary);
	is.seekg(0, is.end); int length = is.tellg(); is.seekg(0, is.beg);
	if (is)
	{
		Verbose::show("Uploading File \"" + fileSpec + "\"", always);
		std::string filename = FileSystem::Path::getName(fileSpec);
		msgmgr.RemoveAttrb(ATTR_FILENAME);
		msgmgr.AddAttrb(ATTR_FILENAME, filename);
		char *buf = new char[length];
		is.read(buf, length);
		char *ptr = buf;
		while (length)
		{
			int len = BLOCK_SIZE < length ? BLOCK_SIZE : length;
			msgmgr.RemoveAttrb(ATTR_BLOCK_SIZE); msgmgr.AddAttrb(ATTR_BLOCK_SIZE, std::to_string(len));
			sc.sendString(msgmgr.CreateMessage()); sc.send(len, ptr);
			if (length == BLOCK_SIZE) {
				msgmgr.RemoveAttrb(ATTR_BLOCK_SIZE);
				msgmgr.AddAttrb(ATTR_BLOCK_SIZE, "0");
				sc.sendString(msgmgr.CreateMessage());
			}
			ptr += BLOCK_SIZE;
			length -= len;
		}
		delete[]buf;
	}
	else {
		throw std::runtime_error(("File \"" + fileSpec + "\" does not exist!").c_str());
	}
	is.close();
}

// A thread function
void Sender::ThreadProc()
{
	MsgMgr msgmgr;
	while (true)
	{
		try{
			std::string msg = blockQ.deQ();
			msgmgr.ParseMessage(msg);
			msgmgr.AddAttrb(ATTR_SRCIP, __local_ip);	msgmgr.AddAttrb(ATTR_SRCPORT, std::to_string(__listen_port));
			std::string cmd = msgmgr.GetAttrbVal(ATTR_COMMAND);
			if (cmd == DOWNLOAD_FILE || cmd == FIND_FILES){
				sc.sendString(msgmgr.CreateMessage());
				Verbose::show(msgmgr.CreateMessage());
			}
			else if (cmd == SEARCH_TEXT || cmd == FEEDBACK_UPLOAD || cmd == FEEDBACK_FILELIST || cmd == FEEDBACK_TEXTSEARCH)
			{
				CreateSendChannel(msgmgr.GetAttrbVal(ATTR_DESTIP), std::stoul(msgmgr.GetAttrbVal(ATTR_DESTPORT)));
				sc.sendString(msgmgr.CreateMessage());
				while (sc.bytesWaiting());
				sc.close();
			}
			else if (cmd == SERVER_UPLOAD_FILE)
			{
				CreateSendChannel(msgmgr.GetAttrbVal(ATTR_DESTIP), std::stoul(msgmgr.GetAttrbVal(ATTR_DESTPORT)));
				sendFile(msgmgr);
				while (sc.bytesWaiting());
				sc.close();
			}
			else if (cmd == CLOSE_CONNECTION)
				sc.close();
			else if (cmd == CONNECT)
				CreateSendChannel(msgmgr.GetAttrbVal(ATTR_DESTIP), std::stoul(msgmgr.GetAttrbVal(ATTR_DESTPORT)));
			else if (cmd == UPLOAD_FILE)
				sendFile(msgmgr);
			else if (cmd == QUIT) break;
		}
		catch (std::exception& e){
			std::cerr << e.what() << std::endl;
		}
	}
}

// Destructor
Sender::~Sender()
{
	thread.join();
	sc.close();
}

// Test stub
#ifdef TEST_SENDER
int main()
{
	ApplicationHelpers::Verbose(true);
	try{
		Sender sender("localhost", 9070);
		MsgMgr m;
		m.AddAttrb(ATTR_COMMAND, CONNECT);
		m.AddAttrb(ATTR_DESTIP, "localhost");
		m.AddAttrb(ATTR_DESTPORT, "9080");
		sender.sendMessage(m.CreateMessage());
		m.Init();
		std::string Prologue = "<?xml version=\"1 . 0 / 0\"?>\n<?xml-stylesheet type=\"text / xsl\" href=\"ivy - report.xsl\"?>\n<!-- top level comment -->\n\n";
		std::string test1 = Prologue;
		test1 += "<parent att1='val1' att2='val2'>\n<child1 />\n<child2>\nchild2 body\n<child1 />\n</child2>\n</parent>\n";
		m.AddAttrb(ATTR_COMMAND, UPLOAD_FILE);
		m.AddAttrb(ATTR_DESTIP, "localhost");
		m.AddAttrb(ATTR_DESTPORT, "9080");
		m.AddAttrb(ATTR_FILENAME, "../santi.pdf");
		//m.SetBody(test1);						// Optional
		std::string msg = m.CreateMessage();
		sender.sendMessage(msg);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
#endif
