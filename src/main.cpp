/*
#include <iostream>
#include <exception>
#include <random>

#include <Windows.h>
#include "ggponet.h"
#include "GLFW/glfw3.h"

#include "State.h"

*/

#include "Application.h"

int main(int argc, char** argv) {
	sng::Application app;
	app.init(argc, argv);
	app.updateLoop();
	return 0;
}

/*

void mainLoop() {
	int inputs[MAX_PLAYERS] = { 0 };
	int disconnect_flags;
	std::cout << "Listening on port: " << localPort << std::endl;
	int i = 0;
	srand(timeGetTime());
	while (true) {
	}
}

void advance_game_state(int inputs[MAX_PLAYERS]) {
	return;
	gameState.position1 += inputs[0];
	gameState.position2 += inputs[1];
}

void startGGPOSession(int numPlayers) {
}

void endGGPOSession() {
	result = ggpo_close_session(ggpo);
	std::cout << "Session closed" << std::endl;
}
*/
