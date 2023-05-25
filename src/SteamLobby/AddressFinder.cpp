#include "AddressFinder.h"

namespace sng {

	std::string AddressFinder::GetPublicIPAddress() {
		HINTERNET net = InternetOpen("IP retriever",
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			0);

		HINTERNET conn = InternetOpenUrl(net,
			"http://myexternalip.com/raw",
			NULL,
			0,
			INTERNET_FLAG_RELOAD,
			0);

		char buffer[4096];
		DWORD read;

		InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read);
		InternetCloseHandle(net);

		return std::string(buffer, read);
	}


	std::string AddressFinder::GetLocalIPAddress() {
		IPv4 myIP;
		char szBuffer[1024];

		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 0);
		if (::WSAStartup(wVersionRequested, &wsaData) != 0)
			return false;


		if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
		{
			WSACleanup();
			return false;
		}

		struct hostent* host = gethostbyname(szBuffer);
		if (host == NULL)
		{
			WSACleanup();
			return false;
		}

		//Obtain the computer's IP
		myIP.b1 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b1;
		myIP.b2 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b2;
		myIP.b3 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b3;
		myIP.b4 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b4;

		WSACleanup();
		return std::to_string(myIP.b1) + '.' +
			   std::to_string(myIP.b2) + '.' +
			   std::to_string(myIP.b3) + '.' +
			   std::to_string(myIP.b4);
    }
}