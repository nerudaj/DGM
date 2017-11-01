#include <DGM/dgm.hpp>

using std::cerr;
using std::cout;
using std::string;
using namespace dgm;

void Animation::reset() {
	frame.setLimit(currentState.getSize());
	frame.reset();
	timer.reset();
	setCurrentFrameToSprite();
}

void Animation::setCurrentFrameToSprite() {
	sprite->setTextureRect(currentState.getFrame(frame.getValue()));
}

bool Animation::update() {
	if (timer.update() == Counter::LimitReached) {		
		if (frame.update() == Counter::LimitReached) {
			if (!(i_flags & AnimationType::Loop)) {
				return false;							// Yield false
			}
		}
		
		setCurrentFrameToSprite();						// Update textureRect on sprite
	}
	
	return true;										// Yield true
}
		
unsigned int Animation::getSpeed() {
	return timer.getLimit();
}

unsigned int Animation::getFlags() {
	return i_flags;
}

void Animation::setState(unsigned int state, unsigned int flags) {
	currentState = states->getState(state);
	i_flags = flags;
	reset();
}

void Animation::setState(std::string state, unsigned int flags) {
	currentState = states->getState(state);
	i_flags = flags;
	reset();
}

bool Animation::setSprite(sf::Sprite *spritePointer) {
	if (spritePointer == NULL) {
		cerr << "dgm::Animation::setSprite(...) - Sprite pointer is NULL.\n";
		return false;
	}
	
	sprite = spritePointer;
	
	return true;
}

void Animation::setSpeed(unsigned int speed) {
	timer.setLimit(speed);
	timer.reset();
}

bool Animation::loadFromFile(std::string filename, std::string identifier) {
	#ifdef _DEBUG
	if (states != NULL) {
		cout << "dgm::Animation::loadFromFile(...) - You're overwriting some older allocated data for the animation. This is totally ok if the data we're initialized using loadFromMemory. Otherwise you really should call free before calling loadFromFile again. Proceeding with opening file '" << filename << "' and loading identifier '" << identifier << "'.\n";
	}
	#endif
	
	states = new AnimationData;
	disallowAutomaticCleanup = false;
	
	return states->loadFromFile(filename, identifier);
}

bool Animation::loadFromMemory(dgm::AnimationData *data) {
	if (data == NULL) {
		cerr << "dgm::Animation::loadFromMemory(...) - NULL pointer provided.\n";
		return false;
	}
	
	states = data;
	disallowAutomaticCleanup = true;
	
	return true;
}

void Animation::clear() {
	if (states != NULL) {
		states->clear();
		delete states;
	}
	
	states = NULL;
}

Animation::Animation() {
	disallowAutomaticCleanup = false;
	states = NULL;
}

Animation::~Animation() {
	/*
	At this point, currentState holds reference to a
	memory block that will soon be freed, but destructor
	of currentState will atempt to delete once more
	resulting in a crash. By making currentState to
	look at empty data, it won't do any further damage.
	*/
	AnimationState dummy;
	currentState = dummy;

	if (not disallowAutomaticCleanup) clear();
}