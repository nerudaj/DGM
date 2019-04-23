#include <DGM\dgm.hpp>
#include <cassert>
#include <fstream>
#include <Strings.hpp>

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

		updateSprite();
	}

	return true;
}

void Animation::updateSprite() {
	assert(boundSprite);
	boundSprite->setTextureRect(currentState->second.getFrame(currentFrameIndex));
}

void Animation::setState(const std::string &state, bool looping) {
	if (states.get().find(state) == states.get().end()) {
		throw dgm::GeneralException("Cannot find animation state '" + state + "'");
	}

	currentState = states.get().find(state);
	setLooping(looping);
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
	currentState = states.get().begin();

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

AnimationStates Animation::loadStatesFromFile(const std::string &filename) {
	throw dgm::GeneralException("Animation::loadStatesFromFile is not implemented!");
	return AnimationStates();
}

/*
bool dgm::AnimationData::loadFromFile(const std::string &name) {
	std::ifstream load(name);
	std::string buffer;
	std::vector<std::string> split;
	int state = 0;

	sf::Vector2i globalFrameSize = sf::Vector2i(0, 0), localFrameSize, globalFrameOffset = sf::Vector2i(0, 0), localFrameOffset;
	unsigned int globalFrameCount = 0, localFrameCount;
	sf::IntRect boundaries;
	std::string stateName;
	dgm::Clip clip;

	try {
		while (load >> buffer) {
			if (buffer == "begin") {
				localFrameSize = globalFrameSize;
				localFrameOffset = globalFrameOffset;
				localFrameCount = globalFrameCount;
				boundaries = sf::IntRect(0, 0, 0, 0);
				stateName = "";
				state = 1;
			}
			else if (buffer == "end") {
				if (stateName.empty()) {
					std::cerr << "AnimationData::loadFromFile(...) - Undefined state name\n";
					throw 1;
				}
				else if (boundaries == sf::IntRect(0, 0, 0, 0)) {
					std::cerr << "AnimationData::loadFromFile(...) - Undefined boundaries\n";
					throw 1;
				}

				clip.init(localFrameSize, boundaries, localFrameCount, globalFrameOffset);
				(*this)[stateName] = clip;
				state = 0;
			}
			else {
				Strings::split('=', buffer, split);

				if (split.size() != 2) {
					std::cerr << "AnimationData::loadFromFile(...) - Expected key=value, got " << buffer << "\n";
					throw 1;
				}

				if (split[0] == "frameSize") {
					if (state == 1 && !dgm::Conversion::stringToVector2i(':', split[1], localFrameSize)) {
						std::cerr << "AnimationData::loadFromFile(...) - Failed to parse frameSize (" << split[1] << ")\n";
						throw 1;
					}
					else if (state == 0 && !dgm::Conversion::stringToVector2i(':', split[1], globalFrameSize)) {
						std::cerr << "AnimationData::loadFromFile(...) - Failed to parse frameSize (" << split[1] << ")\n";
						throw 1;
					}
				}
				else if (split[0] == "frameOffset") {
					if (state == 1 && !dgm::Conversion::stringToVector2i(':', split[1], localFrameOffset)) {
						std::cerr << "AnimationData::loadFromFile(...) - Failed to parse frameOffset (" << split[1] << ")\n";
						throw 1;
					}
					else if (state == 0 && !dgm::Conversion::stringToVector2i(':', split[1], globalFrameOffset)) {
						std::cerr << "AnimationData::loadFromFile(...) - Failed to parse frameOffset (" << split[1] << ")\n";
						throw 1;
					}
				}
				else if (split[0] == "frames") {
					if (state == 0) {
						globalFrameCount = std::stoi(split[1]);
					}
					else if (state == 1) {
						localFrameCount = std::stoi(split[1]);
					}
				}
				else if (split[0] == "boundaries" && state == 1) {
					if (!dgm::Conversion::stringToIntRect(':', split[1], boundaries)) {
						std::cerr << "AnimationData::loadFromFile(...) - Failed to parse boundaries (" << split[1] << ")\n";
						throw 1;
					}
				}
				else if (split[0] == "stateName" && state == 1) {
					stateName = split[1];
				}
				else {
					std::cerr << "AnimationData::loadFromFile(...) - Unknown key " << split[1] << "\n";
					throw 1;
				}
			}
		}
	}
	catch (...) {
		load.close();
		load.clear();
		return false;
	}

	load.close();
	load.clear();

	return true;
}*/