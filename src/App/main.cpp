#include "Application.h"

int main(int argc, char** argv) {
	sng::Application app;
	app.init(argc, argv);
	app.updateLoop();
	return 0;
}
