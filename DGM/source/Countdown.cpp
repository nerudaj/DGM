#include <DGM/dgm.hpp>

using namespace dgm;

/* ========= */
/* COUNTDOWN */
/* ========= */
bool Countdown::update() {
	if (cnt > 0) cnt--;
	
	return cnt == 0;
}

void Countdown::start(unsigned int value) {
	cnt = value;
}

void dgm::Countdown::stop() {
	cnt = 0;
}

bool Countdown::ended() {
	return cnt == 0;
}

unsigned int Countdown::getValue() {
	return cnt;
}

Countdown::Countdown() {
	cnt = 0;
}

Countdown::Countdown(unsigned int value) {
	start(value);
}

Countdown::~Countdown() {	
}