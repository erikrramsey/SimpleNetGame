#pragma once

#include <Windows.h>
#include <WinInet.h>
#include <string>

namespace sng {
struct IPv4
{
	unsigned int b1, b2, b3, b4;
};

class AddressFinder {
public:
	static std::string GetPublicIPAddress();
	static std::string GetLocalIPAddress();
};

}