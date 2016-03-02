/////////////////////////////////////////////////////////////////////
// Exec.h    Extrance								               //
// Ver 1.0                                                         //
// Application: CSE687 Pr#4, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "../HandlerDispatcher/HandlerFactory.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"
#include "../ApplicationHelpers/AppHelpers.h"

using namespace ApplicationHelpers;

struct Cosmetic
{
	~Cosmetic()
	{
		std::cout << "\n  press key to exit: ";
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

int main(int argc, char* argv[])
{
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
	if (argc != 2)
	{
		std::cout << "Invalid arguments" << std::endl;
	}
	std::string localaddress = argv[1];
	int pos = localaddress.find_last_of(":");
	std::string local_ip = localaddress.substr(0, pos);
	int listen_port = std::stoi(localaddress.substr(pos + 1));
	std::cout << "Server Running on " + localaddress + "\n===================\n" << std::endl;
	try{
		Sender sender(local_ip, listen_port);
		Receiver receiver(listen_port, Receiver::IPV4);
		std::shared_ptr<SendHandlerFactory> sf(new SendHandlerFactory);
		std::shared_ptr<RecvDispatcherFactory> rf(new RecvDispatcherFactory);
		auto sh = sf->Create(sender);
		auto rd = rf->Create(sender, receiver);
		std::cout << "\n\nPress any key to exit server.\n\n";
		std::cin.get();
		sender.sendMessage(ATTR_COMMAND + ":" + QUIT);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}