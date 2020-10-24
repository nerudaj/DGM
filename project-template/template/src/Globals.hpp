#pragma once

#include <DGM/dgm.hpp>

struct Settings {
	float soundVolume = 50.f;
	float musicVolume = 50.f;

	void loadFrom(const cfg::Ini& ini) {
		if (!ini.hasSection("Audio")) return;

		if (ini["Audio"].hasKey("soundVolume")) {
			soundVolume = ini["Audio"].at("soundVolume").asFloat();
		}
		if (ini["Audio"].hasKey("musicVolume")) {
			musicVolume = ini["Audio"].at("musicVolume").asFloat();
		}
	}

	void saveTo(cfg::Ini& ini) {
		ini["Audio"]["soundVolume"] = soundVolume;
		ini["Audio"]["musicVolume"] = musicVolume;
	}
};