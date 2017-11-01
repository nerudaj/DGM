#include <DGM/dgm.hpp>

using namespace dgm;

/* ======= */
/* COUNTER */
/* ======= */
bool	Counter::update() {
	if (max == cnt) {
		reset();
		return Status::LimitReached;
	}
	
	cnt++;
	
	return Status::Working;
}

void	Counter::reset() {
	cnt = 0;
}

void	Counter::setLimit(unsigned int limit) {
	max = limit-1;
}

unsigned int Counter::getLimit() {
	return max+1;
}

unsigned int Counter::getValue() {
	return cnt;
}

Counter::Counter() {
	cnt = 0;
	max = 0;
}

Counter::Counter(unsigned int limit) {
	max = limit-1;
}

Counter::~Counter() {
}