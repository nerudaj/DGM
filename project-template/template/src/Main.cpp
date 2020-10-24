#include <DGM/dgm.hpp>

#include "appstates/AppStateBootstrap.hpp"
#include "Globals.hpp"

int main(int argc, char *argv[]) {
	std::string rootDir = "..";
	if (argc == 2) {
		rootDir = argv[1];
	}

	cfg::Ini ini;
	try {
		ini.loadFromFile(rootDir + "/app.ini");
	} catch (...) {}

	Settings settings;
	settings.loadFrom(ini);

	dgm::App app;
	app.window.open(ini);
	
	app.pushState(new AppStateBootstrap(rootDir, settings));
	app.run();

	app.window.close(ini);
	settings.saveTo(ini);

	ini.saveToFile(rootDir + "/app.ini");

	return 0;
}