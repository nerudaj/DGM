#include <DGM\dgm.hpp>
#include <cassert>

bool dgm::AnimationData::LoadFromFile(const std::string &name) {
	return false;
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
