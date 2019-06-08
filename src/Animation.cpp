#include <DGM\dgm.hpp>
#include <cassert>
#include <fstream>
#include <Strings.hpp>
#include <json.hpp>

using dgm::Animation;
using dgm::AnimationStates;

const AnimationStates NullStates = {
	{ "NullState", dgm::Clip() }
};

bool Animation::update(const dgm::Time &time) {
	elapsedTime += time.getElapsed();
	if (elapsedTime > timePerFrame) {
		elapsedTime -= timePerFrame;

		currentFrameIndex++;
		if (currentFrameIndex == currentState->second.getFrameCount()) {
			currentFrameIndex = 0;
			if (not isLooping()) {
				return false;
			}
		}

		updateSpriteTextureRect();
	}

	return true;
}

void Animation::setState(const std::string &state, bool looping) {
	if (isCurrentStateValid() && currentState->first == state) return;

	auto newState = states.get().find(state);
	if (newState == states.get().end()) {
		throw dgm::GeneralException("Cannot find animation state '" + state + "'");
	}

	currentState = newState;
	setLooping(looping);

	reset();
}

void Animation::bindSprite(sf::Sprite *sprite) {
	assert(sprite != nullptr);
	boundSprite = sprite;
}

void Animation::setSpeed(int framePerSecond) {
	timePerFrame = sf::milliseconds(1000.f / framePerSecond);
}

Animation::Animation() : states(NullStates) {
	localInstance = nullptr;
	boundSprite = nullptr;
	elapsedTime = sf::seconds(0);

	setSpeed(30);
	currentFrameIndex = 0;
	currentState = states.get().begin();

	setLooping(false);
}

Animation::Animation(const std::string &filename, int framesPerSecond) : states(NullStates) {
	instantiateLocally();
	*localInstance = Animation::loadStatesFromFile(filename);

	states = *localInstance;
	boundSprite = nullptr;
	elapsedTime = sf::seconds(0);

	setSpeed(framesPerSecond);
	currentFrameIndex = 0;
	currentState = states.get().end();

	setLooping(false);
}

void Animation::instantiateLocally() {
	localInstance = new AnimationStates;
	if (!localInstance) {
		throw dgm::GeneralException("Cannot allocate memory for animation states!");
	}
}

Animation::Animation(Animation &&other) : states(other.states) {
	localInstance = other.localInstance;
	other.localInstance = nullptr;

	boundSprite = other.boundSprite;
	elapsedTime = other.elapsedTime;
	timePerFrame = other.timePerFrame;
	currentFrameIndex = other.currentFrameIndex;
	currentState = other.currentState;
	looping = other.looping;
}

Animation::Animation(const Animation &other) : states(other.states) {
	if (other.isLocallyInstantiated()) {
		instantiateLocally();
		*localInstance = *other.localInstance;
		states = *localInstance;
	}
	else {
		localInstance = nullptr;
	}

	boundSprite = other.boundSprite;
	elapsedTime = other.elapsedTime;
	timePerFrame = other.timePerFrame;
	currentFrameIndex = other.currentFrameIndex;
	currentState = other.currentState;
	looping = other.looping;
}

Animation::Animation(const AnimationStates &states, int framesPerSecond) : states(states) {
	localInstance = nullptr;
}

Animation::~Animation() {
	if (isLocallyInstantiated()) {
		delete localInstance;
		localInstance = nullptr;
	}
}

sf::Vector2i getVec2iFromJsonArray(const nlohmann::json &json) {
	return { json[0], json[1] };
}

sf::IntRect getIntRectFromJsonArray(const nlohmann::json &json) {
	return { json[0], json[1], json[2], json[3] };
}

AnimationStates Animation::loadStatesFromFile(const std::string &filename) {
	std::ifstream load(filename);
	nlohmann::json file;
	load >> file;
	load.close();
	load.clear();

	AnimationStates result;

	// Parse defaults from file
	sf::Vector2i defaultFrameSize = { 8, 8 };
	sf::Vector2i defaultFrameOffset = { 0, 0 };
	if (file.contains("defaults")) {
		if (file["defaults"].contains("frame")) {
			auto frame = file["defaults"]["frame"];
			if (frame.contains("size")) {
				defaultFrameSize = getVec2iFromJsonArray(frame["size"]);
			}
			if (frame.contains("offset")) {
				defaultFrameOffset = getVec2iFromJsonArray(frame["offset"]);
			}
		}
	}

	// Parse through states
	for (auto& state : file["states"].items()) {
		sf::Vector2i frameSize = defaultFrameSize;
		sf::Vector2i frameOffset = defaultFrameOffset;
		unsigned frameCount = 0;

		std::string name = state.key();
		auto data = state.value();

		auto bounds = getIntRectFromJsonArray(data.at("bounds"));
		if (data.contains("frame")) {
			auto frame = data["frame"];
			if (frame.contains("size")) {
				frameSize = getVec2iFromJsonArray(frame["size"]);
			}
			if (frame.contains("offset")) {
				frameOffset = getVec2iFromJsonArray(frame["offset"]);
			}
			if (frame.contains("count")) {
				frameCount = frame["count"];
			}
		}

		result[name].init(frameSize, bounds, frameCount, frameOffset);
	}

	return result;
}
