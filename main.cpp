#include <iostream>
#include <exception>
#include <random>

#include <Windows.h>
#include "vendor/ggponet.h"

const int MAX_PLAYERS = 6;
GGPOSession* ggpo;
GGPOErrorCode result;
GGPOPlayer players[MAX_PLAYERS];
GGPOPlayerHandle player_handles[MAX_PLAYERS];
int localPort;
int localPlayer;

void startGGPOSession(int numPlayers);
void advance_game_state(int inputs[MAX_PLAYERS]);
void endGGPOSession();
void mainLoop();

struct GameState {
	int position1;
	int position2;
} gameState;

struct SessionState {
} sessionState;

GGPOSessionCallbacks cb;
bool begin_game_cb(const char* game);
bool advance_frame_cb(int flags);
bool load_game_state_cb(unsigned char* buffer, int length);
bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame);
void free_buffer_cb(void* buffer);
bool on_event_cb(GGPOEvent * info);


int main(int argc, char** argv) {
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);

	int numPlayers = 0;
	for (int a = 1; a < argc; a += 2) {
		int i = a / 2;
		players[i].size = sizeof(players[i]);
		players[i].player_num = i + 1;
		if (strcmp(argv[a + 1], "local") == 0) {
			players[i].type = GGPO_PLAYERTYPE_LOCAL;
			localPort = atoi(argv[a]);
		} else {
			players[i].type = GGPO_PLAYERTYPE_REMOTE;
			strcpy(players[i].u.remote.ip_address, argv[a + 1]);
			players[i].u.remote.port = atoi(argv[a]);
		}


		numPlayers++;
	}

	startGGPOSession(numPlayers);
	mainLoop();
	endGGPOSession();
	return 0;
}


void mainLoop() {
	int inputs[MAX_PLAYERS] = { 0 };
	int disconnect_flags;
	std::cout << "Listening on port: " << localPort << std::endl;
	int i = 0;
	srand(timeGetTime());
	while (true) {
		int input = 0;
		if (i % 50 == 0)
			input = (rand() % 2) - 1;
		result = ggpo_add_local_input(ggpo, player_handles[localPlayer], &input, sizeof(input));
		if (GGPO_SUCCEEDED(result)) {
			result = ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * MAX_PLAYERS, &disconnect_flags);
			if (GGPO_SUCCEEDED(result)) {
				advance_game_state(inputs);
				ggpo_advance_frame(ggpo);
			}
		}
		
		ggpo_idle(ggpo, 100);
		Sleep(20);
		i++;
	}
}

void advance_game_state(int inputs[MAX_PLAYERS]) {
	return;
	gameState.position1 += inputs[0];
	gameState.position2 += inputs[1];
}

void startGGPOSession(int numPlayers) {
	cb.begin_game = begin_game_cb;
	cb.advance_frame = advance_frame_cb;
	cb.load_game_state = load_game_state_cb;
	cb.save_game_state = save_game_state_cb;
	cb.free_buffer = free_buffer_cb;
	cb.on_event = on_event_cb;

	result = ggpo_start_session(&ggpo, &cb, "SimpleNetGame", numPlayers, sizeof(int), localPort);
	if (result)	throw std::exception("Failed to start ggpo session");

	ggpo_set_disconnect_timeout(ggpo, 3000);
	ggpo_set_disconnect_notify_start(ggpo, 1000);

	for (int i = 1; i < MAX_PLAYERS; i++) {
	}

	for (int i = 0; i < numPlayers; i++) {
		std::cout << "Adding player: " << players[i].u.remote.ip_address << ':'
				  << players[i].u.remote.port << std::endl;
		result = ggpo_add_player(ggpo, &players[i], &player_handles[i]);
		if (players[i].type == GGPO_PLAYERTYPE_LOCAL) localPlayer = i;
		if (result) throw std::exception("Error creating player");
	}
}

void endGGPOSession() {
	result = ggpo_close_session(ggpo);
	std::cout << "Session closed" << std::endl;
}
bool begin_game_cb(const char* game) {
	return true;
}

bool advance_frame_cb(int flags) {
	int inputs[MAX_PLAYERS] = { 0 };
	int disconnect_flags;
	ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * MAX_PLAYERS, &disconnect_flags);
	advance_game_state(inputs);
	ggpo_advance_frame(ggpo);

	return true;
}


bool load_game_state_cb(unsigned char* buffer, int length) {
	memcpy(&gameState, buffer, length);
	return true;
}

bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame) {
	*length = sizeof(gameState);
	*buffer = new unsigned char[*length];
	memcpy(*buffer, &gameState, *length);
	return true;
}

void free_buffer_cb(void* buffer) {
	delete buffer;
}

bool on_event_cb(GGPOEvent* info) {
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		std::cout << "Connected to peer: " << info->u.connected.player << std::endl;
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
		std::cout << "Player Disconnected: " << info->u.disconnected.player << std::endl;
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		std::cout << "Timesync" << std::endl;
		Sleep(1000 * info->u.timesync.frames_ahead / 60);
		break;
	}
	return true;
}
