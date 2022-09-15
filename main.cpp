#include <iostream>
#include "vendor/ggponet.h"

using namespace std;

bool begin_game_cb(const char* game);
bool advance_frame_cb(int flags);
bool load_game_state_cb(unsigned char* buffer, int length);
bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame);
void free_buffer_cb(void* buffer);
bool on_event_cb(GGPOEvent * info);

int main() {
	GGPOSession* ggpo;
	GGPOErrorCode result;
	GGPOSessionCallbacks cb;

	cb.begin_game = begin_game_cb;
	cb.advance_frame = advance_frame_cb;
	cb.load_game_state = load_game_state_cb;
	cb.save_game_state = save_game_state_cb;
	cb.free_buffer = free_buffer_cb;
	cb.on_event = on_event_cb;

	result = ggpo_start_session(&ggpo, &cb, "SimpleNetGame", 2, sizeof(int), 8001);

	cout << "Hello CMake." << endl;
	char i;
	cin >> i;
	return 0;
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
	return false;
}
