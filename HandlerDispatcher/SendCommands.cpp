/////////////////////////////////////////////////////////////////////
// SendCommands.h   Constructs send  messages                      //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "SendCommands.h"
#include "../MsgMgr/MsgType.h"

// Constructing CONNECT msg
std::string ConnectSendCommand::buildMessage(std::string s)
{
	int pos = s.find_last_of(":");
	std::string port = s.substr(pos + 1);
	std::string ip = s.substr(0, pos);
	msgmgr.Init();
	msgmgr.AddAttrb(ATTR_DESTIP, ip);
	msgmgr.AddAttrb(ATTR_DESTPORT, port);

	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, CONNECT);
	return mm.CreateMessage();
}

// Constructing CLOSE msg
std::string CloseSendCommand::buildMessage(std::string s)
{
	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, CLOSE_CONNECTION);
	return mm.CreateMessage();
}

// Constructing UPLOAD_FILE msg
std::string UploadFileSendCommand::buildMessage(std::string s)
{
	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, UPLOAD_FILE);
	mm.AddAttrb(ATTR_FILENAME, s);
	return mm.CreateMessage();
}

// Constructing TEXT_SEARCH msg
std::string TextSearchSendCommand::buildMessage(std::string s)
{
	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, SEARCH_TEXT);
	mm.AddAttrb(ATTR_PATTERN_TEXT, s);
	return mm.CreateMessage();
}

// Constructing DOWNLOAD_FILE msg
std::string DownloadFileSendCommand::buildMessage(std::string s)
{
	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, DOWNLOAD_FILE);
	mm.AddAttrb(ATTR_FILENAME, s);
	return mm.CreateMessage();
}

// Constructing FILE_SEARCH msg
std::string FileSearchSendCommand::buildMessage(std::string s)
{
	MsgMgr arg;
	arg.ParseMessage(s);
	mm.ParseMessage(msgmgr.CreateMessage());
	mm.AddAttrb(ATTR_COMMAND, FIND_FILES);
	std::string pattern = arg.GetAttrbVal(ATTR_PATTERN_FILE);
	std::string path = arg.GetAttrbVal(ATTR_PATH);
	mm.AddAttrb(ATTR_PATTERN_FILE, pattern);
	mm.AddAttrb(ATTR_PATH, path);
	return mm.CreateMessage();
}

//test stub
#ifdef TEST_SENDCOMMAND
int main()
{
	SendCommand *uf, *ts, *con, *close;
	uf = new UploadFileSendCommand();
	ts = new TextSearchSendCommand();
	con = new ConnectSendCommand();
	close = new CloseSendCommand();
	try{
		Sender sender;
		con->Send(sender, "localhost:9080");
		uf->Send(sender, "../proxy2.txt");
		ts->Send(sender, "adsf");
		close->Send(sender, "");
		std::cin.get();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
#endif
