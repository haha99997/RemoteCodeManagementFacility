/////////////////////////////////////////////////////////////////////
// HandlerFactory.h    Creates SendHandler and RecvDispatcher      //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "HandlerFactory.h"
#include "SendCommands.h"
#include "RecvHandlers.h"
#include "Handler.h"
#include "../MsgMgr/MsgType.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include <memory>

//Factory Create
ISendHandler* SendHandlerFactory::Create(ISendr& sender)
{
	SendHandler* handler = new SendHandler(sender);
	std::shared_ptr<SendCommand> connect, close, upload, download, textsearch, filelist;
	connect = std::make_shared<ConnectSendCommand>();
	close = std::make_shared<CloseSendCommand>();
	upload = std::make_shared<UploadFileSendCommand>();
	download = std::make_shared<DownloadFileSendCommand>();
	textsearch = std::make_shared<TextSearchSendCommand>();
	filelist = std::make_shared<FileSearchSendCommand>();
	handler->addCommand(CONNECT, connect);
	handler->addCommand(CLOSE_CONNECTION, close);
	handler->addCommand(UPLOAD_FILE, upload);
	handler->addCommand(DOWNLOAD_FILE, download);
	handler->addCommand(SEARCH_TEXT, textsearch);
	handler->addCommand(FIND_FILES, filelist);
	return handler;
}

//Factory Create
IRecvDispatcher* RecvDispatcherFactory::Create(ISendr& sender, IRecvr& receiver)
{
	RecvDispatcher* dispatcher = new RecvDispatcher(receiver, sender);
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
	return dispatcher;
}

#ifdef TEST_HANDLERFACTORY

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

int main()
{
	//ApplicationHelpers::Verbose(true);
	Sender sender("localhost", 9080);
	Receiver receiver(9080, Receiver::IPV4);
	SendHandlerFactory *sf = new SendHandlerFactory();
	RecvDispatcherFactory *rf = new RecvDispatcherFactory;
	auto sh = sf->Create(sender);
	auto rd = rf->Create(sender, receiver);
	std::cin.get();
}
#endif