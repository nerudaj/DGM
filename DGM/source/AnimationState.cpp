#include <DGM/dgm.hpp>

using std::cerr;
using std::string;
using std::ifstream;
using std::vector;

using namespace dgm;

/* =============== */
/* ANIMATION STATE */
/* =============== */
const std::string	&AnimationState::getName() {
	return stateName;
}

void AnimationState::setName(std::string name) {
	stateName = name;
}

/* ========================== */
/* CONSTRUCTORS & DESTRUCTORS */
/* ========================== */
AnimationState::AnimationState() {
	stateName = "null";
}

AnimationState::~AnimationState() {
	clear();
}