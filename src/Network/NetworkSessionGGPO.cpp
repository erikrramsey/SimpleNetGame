#include "NetworkSessionGGPO.h"
#include "Application.h"

/*
 * Simple checksum function stolen from wikipedia:
 *
 *   http://en.wikipedia.org/wiki/Fletcher%27s_checksum
 */

int fletcher32_checksum(short *data, size_t len) {
    int sum1 = 0xffff, sum2 = 0xffff;

    while (len) {
        size_t tlen = len > 360 ? 360 : len;
        len -= tlen;
        do {
            sum1 += *data++;
            sum2 += sum1;
        } while (--tlen);
        sum1 = (sum1 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    }

    /* Second reduction step to reduce sums to 16 bits */
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
    return sum2 << 16 | sum1;
}

namespace sng {

SessionState NetworkSessionGGPO::state;
Scene* NetworkSessionGGPO::m_scene;

NetworkSessionGGPO::NetworkSessionGGPO() {
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);

	state.cb.begin_game = begin_game_cb;
	state.cb.advance_frame = advance_frame_cb;
	state.cb.load_game_state = load_game_state_cb;
	state.cb.save_game_state = save_game_state_cb;
	state.cb.free_buffer = free_buffer_cb;
	state.cb.on_event = on_event_cb;
}

void NetworkSessionGGPO::setScene(Scene* scene) {
	m_scene = scene;
}

void NetworkSessionGGPO::add_player(const char* ip, const int port) {
	auto p = state.numPlayers;
	state.players[p].size = sizeof(state.players[p]);
	state.players[p].player_num = p + 1;
	if (strcmp(ip, "local") == 0) {
        std::cout << "Player location: " << p << std::endl;
		state.players[p].type = GGPO_PLAYERTYPE_LOCAL;
		state.localPort = port;
	} else {
		state.players[p].type = GGPO_PLAYERTYPE_REMOTE;
		strcpy(state.players[p].u.remote.ip_address, ip);
		state.players[p].u.remote.port = port;
	}

	state.numPlayers++;
}

void NetworkSessionGGPO::start_session() {
	std::cout << "Starting GGPO session" << std::endl;
	auto result = ggpo_start_session(&state.ggpo, &state.cb, "SimpleNetGame", state.numPlayers, sizeof(int), state.localPort);
    //ggpo_start_synctest(&state.ggpo, &state.cb, "SimpleNetGame", state.numPlayers, sizeof(int), 1);
	if (result)	throw std::exception("Failed to start ggpo session");
	ggpo_set_disconnect_timeout(state.ggpo, 3000);
	ggpo_set_disconnect_notify_start(state.ggpo, 1000);


	for (int i = 0; i < state.numPlayers; i++) {
		std::cout << "Adding player: " << state.players[i].u.remote.ip_address << ':'
				  << state.players[i].u.remote.port << std::endl;
		result = ggpo_add_player(state.ggpo, &state.players[i], &state.player_handles[i]);
		if (state.players[i].type == GGPO_PLAYERTYPE_LOCAL) {
            state.localPlayer = i;
            ggpo_set_frame_delay(state.ggpo, state.player_handles[i], 2);
        }
		if (result)
            throw std::exception("Error creating player");
	}
}

void NetworkSessionGGPO::update(int input) {
	int inputs[state.MAX_PLAYERS];
	int disconnect_flags;

	auto result = ggpo_add_local_input(state.ggpo, state.player_handles[state.localPlayer], &input, sizeof(input));
	if (GGPO_SUCCEEDED(result)) {
		result = ggpo_synchronize_input(state.ggpo, (void*)inputs, sizeof(int) * state.MAX_PLAYERS, &disconnect_flags);
		if (GGPO_SUCCEEDED(result)) {
			m_scene->advanceFrameCB(inputs);
			ggpo_advance_frame(state.ggpo);
		}
	}

	ggpo_idle(state.ggpo, 10);
}

void NetworkSessionGGPO::startSyncTest() {
    auto result = ggpo_start_synctest(&state.ggpo, &state.cb, "SimpleNetGame", state.numPlayers, sizeof(char), state.localPort);
    if (result)	throw std::exception("Failed to start ggpo synctest");
    for (int i = 0; i < state.numPlayers; i++) {
        std::cout << "Adding player: " << state.players[i].u.remote.ip_address << ':'
                  << state.players[i].u.remote.port << std::endl;
        result = ggpo_add_player(state.ggpo, &state.players[i], &state.player_handles[i]);
        if (state.players[i].type == GGPO_PLAYERTYPE_LOCAL) state.localPlayer = i;
        if (result) throw std::exception("Error creating player");
    }
}

bool NetworkSessionGGPO::begin_game_cb(const char* game) {
	return true;
}

bool NetworkSessionGGPO::advance_frame_cb(int flags) {
	int inputs[state.MAX_PLAYERS] = { 0 };
	int disconnect_flags;
	ggpo_synchronize_input(state.ggpo, (void*)inputs, sizeof(int) * state.MAX_PLAYERS, &disconnect_flags);
	m_scene->advanceFrameCB(inputs);
	ggpo_advance_frame(state.ggpo);

	return true;
}


bool NetworkSessionGGPO::load_game_state_cb(unsigned char* buffer, int length) {
	//std::cout << "Rollback occured" << std::endl;
	memcpy(&m_scene->getSharedState(), buffer, length);
	return true;
}

bool NetworkSessionGGPO::save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame) {
	SharedState& gs = m_scene->getSharedState();
	*length = sizeof(gs);
	*buffer = new unsigned char[*length];
	memcpy(*buffer, &gs, *length);
    *checksum = fletcher32_checksum((short *)*buffer, *length / 2);
	return true;
}

void NetworkSessionGGPO::free_buffer_cb(void* buffer) {
	delete buffer;
}

bool NetworkSessionGGPO::on_event_cb(GGPOEvent* info) {
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		//std::cout << "Connected to peer: " << info->u.connected.player << std::endl;
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		//std::cout << "Synchronizing" << std::endl;
		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		//std::cout << "Synchronized" << std::endl;
		break;
	case GGPO_EVENTCODE_RUNNING:
		//std::cout << "Running" << std::endl;
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		//std::cout << "Connection interrupted" << std::endl;
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		//std::cout << "Connection resumed" << std::endl;
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		//std::cout << "Player Disconnected: " << info->u.disconnected.player << std::endl;
		break;
	case GGPO_EVENTCODE_TIMESYNC:
        // Was having issues where the timesync would suddenly need to wait for 7 frames, i think it
        // has to do with the update loop already waiting a fixed amount of time
		//std::cout << "Timesync: " << info->u.timesync.frames_ahead << std::endl;
		//Sleep(500 * info->u.timesync.frames_ahead / 60);
		break;
	}
	return true;
}

}