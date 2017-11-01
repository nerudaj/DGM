#include <DGM\dgm.hpp>

int dgm::Dice::roll(int sides, int dies) {
	int r = 0;
	
	for (int i = 0; i < dies; i++) {
		r += rand() % sides + 1;
	}
	
	return r;
}