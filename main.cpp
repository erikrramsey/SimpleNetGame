#include <iostream>
#include <exception>

#include <Windows.h>
#include "vendor/ggponet.h"

GGPOSession* ggpo;
GGPOErrorCode result;
GGPOPlayer p1, p2;
GGPOPlayerHandle player_handles[2];

int localPort;
int remotePort;

void startGGPOSession();
void endGGPOSession();
void mainLoop();

GGPOSessionCallbacks cb;
bool begin_game_cb(const char* game);
bool advance_frame_cb(int flags);
bool load_game_state_cb(unsigned char* buffer, int length);
bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame);
void free_buffer_cb(void* buffer);
bool on_event_cb(GGPOEvent * info);


int main(int argc, char** argv) {
	WSADATA wd = { 0 };
	wchar_t wide_ip_buffer[128];
	unsigned int wide_ip_buffer_size = (unsigned int)ARRAYSIZE(wide_ip_buffer);
	WSAStartup(MAKEWORD(2, 2), &wd);

	localPort = atoi(argv[1]);
	remotePort = atoi(argv[2]);

	startGGPOSession();
	mainLoop();
	endGGPOSession();
	return 0;
}

void mainLoop() {
	int inputs[2] = { 0 };
	int disconnect_flags;
	int cms;
	std::cout << "Listening on port: " << localPort << std::endl;
	while (true) {
		cms = timeGetTime();
		int input = rand();
		result = ggpo_add_local_input(ggpo, player_handles[0], &input, sizeof(input));
		result = ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * 2, &disconnect_flags);

		ggpo_advance_frame(ggpo);
		ggpo_idle(ggpo, 100);
		Sleep(16 - (timeGetTime() - cms));
		double fps = 1000.0 / (timeGetTime() - cms);
	}
}

void startGGPOSession() {
	cb.begin_game = begin_game_cb;
	cb.advance_frame = advance_frame_cb;
	cb.load_game_state = load_game_state_cb;
	cb.save_game_state = save_game_state_cb;
	cb.free_buffer = free_buffer_cb;
	cb.on_event = on_event_cb;

	result = ggpo_start_session(&ggpo, &cb, "SimpleNetGame", 2, sizeof(int), localPort);
	if (result)	throw std::exception("Failed to start ggpo session");


	p1.size = sizeof(p1);
	p1.type = GGPO_PLAYERTYPE_LOCAL;
	p1.player_num = 1;
	p2.size = sizeof(p2);
	p2.type = GGPO_PLAYERTYPE_REMOTE;
	p2.player_num = 2;
	strcpy(p2.u.remote.ip_address, "127.0.0.1");
	p2.u.remote.port = remotePort;

	result = ggpo_add_player(ggpo, &p1, &player_handles[0]);
	if (result) throw std::exception("Error creating local player");

	result = ggpo_add_player(ggpo, &p2, &player_handles[1]);
	if (result) throw std::exception("Error creating remote player");


}

void endGGPOSession() {
	result = ggpo_close_session(ggpo);
	std::cout << "Session closed" << std::endl;
}
bool begin_game_cb(const char* game) {
	return false;
}

bool advance_frame_cb(int flags) {
	return false;
}

bool load_game_state_cb(unsigned char* buffer, int length) {
	return false;
}

bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame) {
	return false;
}

void free_buffer_cb(void* buffer) {
}

bool on_event_cb(GGPOEvent* info) {
	int progress;
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		std::cout << "Connected" << std::endl;
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		std::cout << "Synchronizing" << std::endl;
		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		std::cout << "Synchronized" << std::endl;
		break;
	case GGPO_EVENTCODE_RUNNING:
		std::cout << "Running" << std::endl;
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		std::cout << "Connection interrupted" << std::endl;
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		std::cout << "Connection resumed" << std::endl;
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		std::cout << "Disconnected" << std::endl;
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		std::cout << "Timesynce" << std::endl;
		break;
	}
	return true;
}
