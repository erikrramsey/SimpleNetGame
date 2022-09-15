#include <iostream>
#include <exception>

#include <Windows.h>
#include "vendor/ggponet.h"

bool begin_game_cb(const char* game);
bool advance_frame_cb(int flags);
bool load_game_state_cb(unsigned char* buffer, int length);
bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame);
void free_buffer_cb(void* buffer);
bool on_event_cb(GGPOEvent * info);
void startGGPOSession();

int main() {
	WSADATA wd = { 0 };
	wchar_t wide_ip_buffer[128];
	unsigned int wide_ip_buffer_size = (unsigned int)ARRAYSIZE(wide_ip_buffer);

	WSAStartup(MAKEWORD(2, 2), &wd);

	startGGPOSession();
	return 0;
}

void startGGPOSession() {
	GGPOSession* ggpo;
	GGPOErrorCode result;
	GGPOSessionCallbacks cb;

	cb.begin_game = begin_game_cb;
	cb.advance_frame = advance_frame_cb;
	cb.load_game_state = load_game_state_cb;
	cb.save_game_state = save_game_state_cb;
	cb.free_buffer = free_buffer_cb;
	cb.on_event = on_event_cb;

	result = ggpo_start_session(&ggpo, &cb, "SimpleNetGame", 2, sizeof(int), 7000);
	if (result)	throw std::exception("Failed to start ggpo session");

	GGPOPlayer p1, p2;
	GGPOPlayerHandle player_handles[2];

	p1.size = sizeof(p1);
	p1.type = GGPO_PLAYERTYPE_LOCAL;
	p1.player_num = 1;
	p2.size = sizeof(p2);
	p2.type = GGPO_PLAYERTYPE_REMOTE;
	p2.player_num = 2;
	strcpy(p2.u.remote.ip_address, "127.0.0.1");
	p2.u.remote.port = 7001;

	result = ggpo_add_player(ggpo, &p1, &player_handles[0]);
	if (result) throw std::exception("Error creating local player");

	result = ggpo_add_player(ggpo, &p2, &player_handles[1]);
	if (result) throw std::exception("Error creating remote player");


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

	return true;
}
