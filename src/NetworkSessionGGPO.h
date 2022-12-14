#include <Windows.h>
#include <iostream>

#include "NetworkSession.h"
#include "ggponet.h"

#include "GameState.h"
#include "SessionState.h"
#include "GameplayManager.h"

namespace sng {

class NetworkSessionGGPO : public NetworkSession {
public:
	void add_player(const char* ip, const int port) override;
	void start_session() override;
	void update(int input) override;

	NetworkSessionGGPO(GameplayManager* manager);
private:
	static SessionState state;
	static GameplayManager* gp;

	// Callbacks
	static bool begin_game_cb(const char* game);
	static bool advance_frame_cb(int flags);
	static bool load_game_state_cb(unsigned char* buffer, int length);
	static bool save_game_state_cb(unsigned char** buffer, int* length, int* checksum, int frame);
	static void free_buffer_cb(void* buffer);
	static bool on_event_cb(GGPOEvent * info);
};

}