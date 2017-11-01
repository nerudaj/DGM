#include <DGM/dgm.hpp>

using std::cerr;
using std::string;
using std::ifstream;
using std::vector;

using namespace dgm;

/* =========== */
/* PRIVATE FCE */
/* =========== */
bool compareStartHeader (string input, string type, string name) {
	return input == type + "=" + name;
}

bool parseClippingParameter(string input, sf::Vector2i &frameSize, sf::Vector2i &frameOffset, sf::IntRect &boundaries, unsigned int &frames) {
	vector<string> split;
	dgm::Strings::split('=', input, split);
	
	if (split.size() != 2) return false;
	
	if (split[0] == "frameSize") {
		return dgm::Conversion::stringToVector2i(':', split[1], frameSize);
	}
	else if (split[0] == "frameOffset") {
		return dgm::Conversion::stringToVector2i(':', split[1], frameOffset);
	}
	else if (split[0] == "boundaries") {
		return dgm::Conversion::stringToIntRect(':', split[1], boundaries);
	}
	else if (split[0] == "frames") {
		frames = strtol(split[1].c_str(), NULL, 10);
		return frames > 0 ? true : false;
	}
	
	return false;
}

bool parseAnimationParameter(string input, unsigned int &states) {
	vector<string> split;
	dgm::Strings::split('=', input, split);
	
	if (split.size() != 2 || split[0] != "states") return false;
	
	states = strtol(split[1].c_str(), NULL, 10);

	return states > 0 ? true : false;	// If strtol fails and states is 0, it would be equal to 'return false'
}

bool parseStateName(string input, string &stateName) {
	vector<string> split;
	dgm::Strings::split('=', input, split);
	
	if (split.size() != 2) return false;
	
	if (split[0] == "state" || split[0] == "name") {
		stateName = split[1];
		return true;
	}
	
	return false;
}

/* ============== */
/* ANIMATION DATA */
/* ============== */
unsigned int AnimationData::getSize() {
	return size;
}

const AnimationState &AnimationData::getState(std::string identifier) const {
	for(unsigned int i = 0; i < size; i++) {
		if (states[i].getName() == identifier) {
			return states[i];
		}
	}
	
	cerr << "dgm::AnimationData::getState(...) - Invalid identifier given. Identifier: '" << identifier << "' not found in states array. Returning the first state for sake of stability.\n";
	#ifdef _DEBUG
	cerr << "\t[DEBUG ONLY]\n\tThis is a list of names of available states:\n";
	for (unsigned int i = 0; i < size; i++) {
		cerr << "\t\t" << states[i].getName() << "\n";
	}
	#endif
	return states[0];
}

const AnimationState &AnimationData::getState(unsigned int index) const {
	if (index >= size) {
		cerr << "dgm::AnimationData::getState(...) - Invalid index given. Index value is " << index << " whilst the number of states is " << size << ".  Returning the first state for sake of stability.\n";
		return states[0];
	}
	
	return states[index];
}

bool AnimationData::loadFromFile(std::string filename, std::string identifier) {
	enum { STATE_START, STATE_LOADING, STATE_LOADING_STATES_PREP, STATE_LOADING_STATES, STATE_ERROR, STATE_DONE };
	const string ErrHeader ("dgm::AnimationData::loadFromFile(...) - ");
	
	ifstream Load (filename);
	
	if (!Load.good()) {
		cerr << ErrHeader << "Could not open file '" << filename << "' for reading.\n";
		return false;
	}
	
	// Loading
	int			state = STATE_START;
	std::string buffer;
	int			stateCnt = 0;
	
	// Computation globals
	sf::Vector2i	frameSize 	(0, 0);
	sf::Vector2i	frameOffset (0, 0);
	sf::IntRect		boundaries 	(0, 0, 0, 0);
	unsigned int	frames = 0;
	
	// Computation locals
	sf::Vector2i	localFrameSize 		(0, 0);
	sf::Vector2i	localFrameOffset	(0, 0);
	sf::IntRect		localBoundaries		(0, 0, 0, 0);
	unsigned int	localFrames = 0;
	
	while (Load >> buffer && state != STATE_ERROR && state != STATE_DONE) {
		switch (state) {
		case STATE_START:
			if (buffer == "start") {
				Load >> buffer;
				if (compareStartHeader(buffer, "animation", identifier)) {
					state = STATE_LOADING;
				}
			}
			break;
		case STATE_LOADING:
			if (!parseClippingParameter(buffer, frameSize, frameOffset, boundaries, frames)) {
				if (parseAnimationParameter(buffer, size)) {	// NOTE: size is meant to be AnimationData::size variable, not sure about the name at the moment.
					states = new AnimationState[size];
					
					if (states == NULL) {
						cerr << ErrHeader << "could not allocate memory for the animation states.\n";
						state = STATE_ERROR;
					}
					
					state = STATE_LOADING_STATES_PREP;
				}
				else {
					cerr << ErrHeader << "'" << buffer << "' has invalid syntax.\n";
					state = STATE_ERROR;
				}
			}
			break;
		case STATE_LOADING_STATES_PREP:
			if (buffer == "start") {
				Load >> buffer;
				if (!parseStateName(buffer, buffer)) {// NOTE: Nasty
					cerr << ErrHeader << "Expected state name declaration in form 'state=name', got '" << buffer << "'.\n";
					state = STATE_ERROR;
				}
				else {
					states[stateCnt].setName(buffer);

					localFrameSize = frameSize;
					localFrameOffset = frameOffset;
					localBoundaries = boundaries;
					localFrames = frames;
					state = STATE_LOADING_STATES;
				}
			}
			else {
				cerr << ErrHeader << "Expected start block for " << stateCnt + 1 << "th animation state, got '" << buffer << "' instead.\n";
				state = STATE_ERROR;
			}
			break;
		case STATE_LOADING_STATES:
			if (!parseClippingParameter(buffer, localFrameSize, localFrameOffset, localBoundaries, localFrames)) {
				if (buffer == "end") {
					if (states[stateCnt].loadFromParameters(localFrameSize, localBoundaries, localFrames, localFrameOffset)) {
						stateCnt++;
						
						if (stateCnt == size)	state = STATE_DONE;
						else					state = STATE_LOADING_STATES_PREP;
					}
					else {
						cerr << ErrHeader << "Could not compute clipping data for the state " << states[stateCnt].getName() << "\n";
						state = STATE_ERROR;
					}
				}
				else {
					cerr << ErrHeader << "Expected 'end', got '" << buffer << "'.\n";
					state = STATE_ERROR;
				}
			}
			break;
		default:
			break;
		}
	}
	
	Load.close();
	Load.clear();
	
	if (state == STATE_ERROR) {
		if (states != NULL) {
			delete[] states;
		}
		
		return false;
	}
	
	disallowAutomaticCleanup = false;
	
	return true;
}

bool AnimationData::loadFromMemory(AnimationState *data, unsigned int dataSize) {
	if (states == NULL) {
		return false;
	}
	
	states = data;
	size = dataSize;
	disallowAutomaticCleanup = true;
	
	return true;
}

void AnimationData::clear() {
	if (states != NULL) {
		for (unsigned int i = 0; i < size; i++) {
			states[i].clear();
		}

		delete[] states;
	}

	states = NULL;
	size = 0;
}

AnimationData::AnimationData() {
	states = NULL;
	size = 0;
	disallowAutomaticCleanup = false;
}

AnimationData::~AnimationData() {
	if (not disallowAutomaticCleanup) clear();
}