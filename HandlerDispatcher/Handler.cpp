/////////////////////////////////////////////////////////////////////
// Handler.h   Handles send and receive process                    //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "Handler.h"
#include "../MsgMgr/MsgType.h"
#include <stdexcept>

//Constructor
RecvDispatcher::RecvDispatcher(IRecvr& r, ISendr& s) : receiver(r), sender(s)
{
	t = std::thread([&]{
		MsgMgr msgmgr;
		while (true)
		{
			try{
				std::string msg = receiver.getMessage();
				blockQ.enQ(msg);
				msgmgr.ParseMessage(msg);
				std::string command = msgmgr.GetAttrbVal(ATTR_COMMAND);
				invoke(command, msg);
			}
			catch (std::exception&)
			{

			}
		}
	});
	t.detach();
}

RecvDispatcher::~RecvDispatcher()
{
	for (auto iter : commands)
		iter.second->enQ(ATTR_COMMAND + ":" + QUIT);
}

//Add Command-Handler map relation
void SendHandler::addCommand(std::string command, std::shared_ptr<SendCommand> processor)
{
	if (commands.find(command) != commands.end())
		throw std::runtime_error(("Command " + command + " already exist!").c_str());
	commands[command] = processor;
}

//Invoke command handler with a certain argument
void SendHandler::invoke(std::string command, std::string arg)
{
	try{
		commands[command]->Send(sender, arg);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

//Add Command-Handler map relation
void RecvDispatcher::addCommand(std::string command, std::shared_ptr<RecvHandler> processor)
{
	if (commands.find(command) != commands.end())
		throw std::runtime_error(("Command " + command + " already exist!").c_str());
	commands[command] = processor;
}

//Invoke command handler with a certain argument
void RecvDispatcher::invoke(std::string command, std::string arg)
{
	if (commands.find(command) == commands.end())
		throw std::runtime_error(("Command " + command + " is invalid!").c_str());
	if (command == QUIT)
	{
		for(auto& cmd : commands)
			cmd.second->enQ(command);
	}
	else
		commands[command]->enQ(arg);
}

std::string RecvDispatcher::getMessage()
{
	return blockQ.deQ();
}

//test stub
#ifdef TEST_HANDLER
int main()
{
	std::shared_ptr<RecvDispatcher> dispatcher(new RecvDispatcher(receiver, sender));
	std::shared_ptr<RecvHandler> upload, download, textsearch, filelist, upload_feedback;
	upload = std::make_shared<UploadHandler>(sender);
	download = std::make_shared<DownloadHandler>(sender);
	textsearch = std::make_shared<TextSearchHandler>(sender);
	filelist = std::make_shared<FileListHandler>(sender);
	upload_feedback = std::make_shared<FeedBackUploadHandler>(sender);
	dispatcher->addCommand(UPLOAD_FILE, upload);
	dispatcher->addCommand(DOWNLOAD_FILE, download);
	dispatcher->addCommand(SEARCH_TEXT, textsearch);
	dispatcher->addCommand(FIND_FILES, filelist);
	dispatcher->addCommand(FEEDBACK_UPLOAD, upload_feedback);
}
#endif
