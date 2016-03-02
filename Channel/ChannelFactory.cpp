/////////////////////////////////////////////////////////////////////
// ChannelFactory.h    Send Messages, include file upload          //
// Ver 1.0                                                         //
// Application: CSE687 Pr#4, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "ChannelFactory.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../HandlerDispatcher/HandlerFactory.h"

ISendr* ChannelFactory::CreateSender(const std::string& local_ip, size_t port)
{
	return new Sender(local_ip, port);
}

IRecvr* ChannelFactory::CreateReceiver(size_t port, IpVer ipver)
{
	return new Receiver(port, ipver == IPV4? Receiver::IPV4 : Receiver::IPV6);
}

ISendHandler* ChannelFactory::CreateSendHandler(ISendr& sender)
{
	SendHandlerFactory* sdhdlr = new SendHandlerFactory;
	auto result = sdhdlr->Create(sender);
	delete sdhdlr;
	return result;
}

IRecvDispatcher* ChannelFactory::CreateRecvDispatcher(ISendr& sender, IRecvr& receiver)
{
	RecvDispatcherFactory* fact = new RecvDispatcherFactory;
	auto result = fact->Create(sender, receiver);
	delete fact;
	return result;
}
