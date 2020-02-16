#pragma once

#include <vector>
#include <DGM/DGM.hpp>

class Soundbox {
private:
    std::vector<sf::Sound> channels;
    const dgm::ResourceManager &resmgr;

    bool isChannelActive(unsigned channel) {
        return channels[channel].getStatus() == sf::Sound::Status::Playing;
    }

public:
    void playSound(const std::string &soundName, unsigned channel, bool force = false);

    void stopChannel(unsigned channel);

    void setChannelCount(unsigned channelCount);

    void setVolume(float volume);

    Soundbox(const dgm::ResourceManager &resmgr) : resmgr(resmgr) {}

    ~Soundbox();
};