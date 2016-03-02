#pragma once

#include <string>

struct IRecvr
{
	virtual std::string getMessage() = 0;
};