#include <DGM/dgm.hpp>

using namespace dgm;

/* ============== */
/* TIME COUNTDOWN */
/* ============== */
bool TimeCountdown::substract (int index, int iter) {
	#define NEXT_DIGIT		(iter % 2) ? index - 2 : index - 1
	#define OVERFLOW_VALUE	(iter % 2) ? '5' : '9'
	
	//text[index]--;
	
	if ((--text[index]) < '0') {
		if (index == 0 or substract(NEXT_DIGIT, iter + 1))	text[index] = '-';
		else												text[index] = OVERFLOW_VALUE;
	}
	
	return text[index] == '-';
}

bool TimeCountdown::update () {
	#define LAST_DIGIT text.size() - 1
	
	if (not stopped and tick.update() == dgm::Counter::Status::LimitReached) {
		stopped = substract(LAST_DIGIT, 0);
	}
	
	return stopped;
}

const std::string &TimeCountdown::getValue () const {
	return text;
}


bool sanityCheck(const std::string &value) {
	for (int i = value.size() - 1, p = 1; i >= 0; i--, p++) {
		if (p % 3 == 0) continue;
		else if ('0' <= value[i] and value[i] <= '9') continue;
		else {
			std::cerr << "dgm::TimeCountdown::start(...) - Character at index " << i << " is not a number.\n";
			return false;
		}
	}
	
	return true;
}

bool TimeCountdown::start (const std::string &value, unsigned int ticksInSecond) {
	// *** SANITY CHECK ***
	if (not sanityCheck(value)) return false;
	
	// *** PASSED, INIT ***
	text = value;
	tick.setLimit(ticksInSecond);
	stopped = false;
	
	return true;
}

bool TimeCountdown::ended () {
	return stopped;
}

TimeCountdown::TimeCountdown() {
	stopped = false;
	text = "--:--";
}

TimeCountdown::TimeCountdown(const std::string &value, unsigned int ticksInSecond) {
	start(value, ticksInSecond);
}

TimeCountdown::~TimeCountdown() {
	
}