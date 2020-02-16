#include "Soundbox.hpp"

void Soundbox::playSound(const std::string& soundName, unsigned channel, bool force) {
	if (force || !isChannelActive(channel)) {
		channels[channel].setBuffer(resmgr.get<sf::SoundBuffer>(soundName));
		channels[channel].play();
	}
}

void Soundbox::stopChannel(unsigned channel) {
	channels[channel].stop();
}

void Soundbox::setChannelCount(unsigned channelCount) {
	channels.resize(channelCount);
}

void Soundbox::setVolume(float volume) {
	for (auto& channel : channels) {
		channel.setVolume(volume);
	}
}

Soundbox::~Soundbox() {
	for (auto& channel : channels) channel.stop();
}
