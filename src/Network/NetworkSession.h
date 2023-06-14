#pragma once

namespace sng {
const int MAX_P = 4;

struct SessionState;
class NetworkSession {
public:
	virtual void add_player(const char* ip, const int port) = 0;
	virtual void start_session() = 0;
	virtual void update(int input) = 0;

	virtual SessionState* getSessionState() = 0;
};

}