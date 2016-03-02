#pragma once

#include <string>

struct ISendr
{
	virtual void sendMessage(const std::string& msg) = 0;
};