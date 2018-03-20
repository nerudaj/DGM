#include <DGM\dgm.hpp>
#include <cassert>
#include <fstream>

bool dgm::AnimationData::LoadFromFile(const std::string &name) {
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
					std::cerr << "AnimationData::LoadFromFile(...) - Undefined state name\n";
					throw 1;
				}
				else if (boundaries == sf::IntRect(0, 0, 0, 0)) {
					std::cerr << "AnimationData::LoadFromFile(...) - Undefined boundaries\n";
					throw 1;
				}

				clip.Init(localFrameSize, boundaries, localFrameCount, globalFrameOffset);
				(*this)[stateName] = clip;
				state = 0;
			}
			else {
				dgm::Strings::Split('=', buffer, split);

				if (split.size() != 2) {
					std::cerr << "AnimationData::LoadFromFile(...) - Expected key=value, got " << buffer << "\n";
					throw 1;
				}

				if (split[0] == "frameSize") {
					if (state == 1 && !dgm::Conversion::StringToVector2i(':', split[1], localFrameSize)) {
						std::cerr << "AnimationData::LoadFromFile(...) - Failed to parse frameSize (" << split[1] << ")\n";
						throw 1;
					}
					else if (state == 0 && !dgm::Conversion::StringToVector2i(':', split[1], globalFrameSize)) {
						std::cerr << "AnimationData::LoadFromFile(...) - Failed to parse frameSize (" << split[1] << ")\n";
						throw 1;
					}
				}
				else if (split[0] == "frameOffset") {
					if (state == 1 && !dgm::Conversion::StringToVector2i(':', split[1], localFrameOffset)) {
						std::cerr << "AnimationData::LoadFromFile(...) - Failed to parse frameOffset (" << split[1] << ")\n";
						throw 1;
					}
					else if (state == 0 && !dgm::Conversion::StringToVector2i(':', split[1], globalFrameOffset)) {
						std::cerr << "AnimationData::LoadFromFile(...) - Failed to parse frameOffset (" << split[1] << ")\n";
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
					if (!dgm::Conversion::StringToIntRect(':', split[1], boundaries)) {
						std::cerr << "AnimationData::LoadFromFile(...) - Failed to parse boundaries (" << split[1] << ")\n";
						throw 1;
					}
				}
				else if (split[0] == "state" && state == 1) {
					stateName = split[1];
				}
				else {
					std::cerr << "AnimationData::LoadFromFile(...) - Unknown key " << split[1] << "\n";
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
}

bool dgm::AnimationData::AddState(const std::string &stateName, const dgm::Clip &clip) {
	if (animations->find(stateName) == animations->end()) {
		std::cerr << "Animation::AddState - State named " << stateName << " already exists\n";
		return false;
	}
	
	(*animations)[stateName] = clip;
	return true;
}

void dgm::Animation::Reset() {
	elapsed = sf::Time::Zero;
	frameIndex = 0;
}

bool dgm::Animation::Update(const dgm::Time &time) {
	elapsed += time.Elapsed();
	if (elapsed > timePerFrame) {
		elapsed -= timePerFrame;
		
		frameIndex++;
		if (frameIndex == currentState->second.GetFrameCount()) {
			frameIndex = 0;
			if (not (flags & Flags::Looping)) {
				return false;
			}
		}
		
		boundSprite->setTextureRect(currentState->second.GetFrame(frameIndex));
	}
	
	return true;
}

void dgm::Animation::SetSprite(sf::Sprite *sprite) {
	boundSprite = sprite;
}

void dgm::Animation::SetSpeed(int fps) {
	timePerFrame = sf::milliseconds(1000 / fps);
}

bool dgm::Animation::SetState(const std::string &state, int flags) {
	currentState = animations->find(state);
	if (currentState == animations->end()) return false;
	
	Reset();
	Animation::flags = flags;
	
	return true;
}

bool dgm::Animation::LoadFromFile(const std::string &filename) {
	animations = new AnimationData;
	if (animations == NULL) {
		return false;
	}
	
	if (not animations->LoadFromFile(filename)) {
		delete animations;
		return false;
	}
	
	mallocked = true;
	return true;
}

bool dgm::Animation::LoadFromMemory(dgm::AnimationData *data) {
	assert(data != NULL);
	
	if (mallocked) {
		std::cerr << "Animation::LoadFromMemory - Memory already occupied\n";
	}
	
	animations = data;
	return true;
}

void dgm::Animation::Deinit() {
	if (mallocked) {
		delete animations;
		animations = nullptr;
		mallocked = true;
	}
}

dgm::Animation::Animation() {
	mallocked = false;
	animations = nullptr;
	boundSprite = nullptr;
	timePerFrame = sf::milliseconds(50);
}

dgm::Animation::~Animation() {
	Deinit();
}
