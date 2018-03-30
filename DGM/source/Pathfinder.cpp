#include <DGM\dgm.hpp>
#include <thread>
#include <queue>

using dgm::ai::Pathfinder;

#define inactiveLookup 1-activeLookup
#define INFINITE_LIMIT -1
#define UNSET -1
#define TOPDOWN 5
#define LEFTRIGHT 10

struct TargetIndex {
	int position;
	int distanceTotal;
	int limitLeft;
};

void Pathfinder::swapLookups() {
	activeLookup = inactiveLookup;
}

int power2(int exp) {
	int result = 1;
	for (int e = 0; e < exp; e++) result *= 2;
	return result;
}

void Pathfinder::clearLookup(LookupArray &lookup) {
	for (int i = 0; i < lookupSize.x * lookupSize.y; i++) {
		lookup[i].value = UNSET;
	}
}

void Pathfinder::buildLookup(LookupArray &lookup, TargetArray &targets) {
	const std::vector<int> directions({-1, -lookupSize.x, 1, lookupSize.x});
	
	// Build queue of starting points
	std::queue<TargetIndex> toProcess;
	for (auto target : targets) {
		toProcess.push({target.position.y * lookupSize.x + target.position.x, 0, (target.distance == 0 ? INFINITE_LIMIT: target.distance)});
	}
	
	while (not toProcess.empty()) {
		TargetIndex &t = toProcess.front();
		toProcess.pop();
		
		if (lookup[t.position].value != UNSET && lookup[t.position].value < t.distanceTotal) continue;
		
		lookup[t.position].value = t.distanceTotal;
		t.distanceTotal++;
		
		if (t.limitLeft != INFINITE_LIMIT) t.limitLeft--;
		if (t.limitLeft == 0) continue;
		
		for (auto direction : directions) {
			int pos = t.position + direction;
			
			if (not lookup[pos].isSolid()) {
				// TODO: Check out-of-bounds
				toProcess.push({pos, t.distanceTotal, t.limitLeft});
			}
		}
	}
}

void Pathfinder::markFastForwards(LookupArray &lookup) {
	const std::vector<int> directions({-1, -lookupSize.x, 1, lookupSize.x});
	
	for (int i = 0; i < lookupSize.x * lookupSize.y; i++) {
		int setDirs = 0;
		
		if (lookup[i].isSolid()) continue;
		
		for (int d = 0; d < 3; d++) {
			if (lookup[i + directions[d]].value > -1) {
				setDirs |= power2(d);
			}
		}
		
		lookup[i].setFastForward((setDirs == TOPDOWN || setDirs == LEFTRIGHT));
	}
}

void Pathfinder::recomputeLookup() {
	LookupArray &lookups = lookupArrays[inactiveLookup];
	TargetArray &targets = targetArrays[inactiveLookup];
	
	clearLookup(lookups);
	buildLookup(lookups, targets);
	markFastForwards(lookups);

	for (int y = 0; y < lookupSize.y; y++) {
		for (int x = 0; x < lookupSize.x; x++) {
			std::cerr << lookups[y * lookupSize.x + x].flags << ":" << lookups[y * lookupSize.x + x].value << " ";
		}
		std::cerr << "\n";
	}
	
	progress = 100; // finished
}

bool Pathfinder::recompute() {
	if (getComputationProgress() != 100) {
		return false;
	}
	
	if (syncLookupArray == inactiveLookup) {
		for (int i = 0; i < lookupSize.x * lookupSize.y; i++) {
			lookupArrays[inactiveLookup][i].flags = lookupArrays[activeLookup][i].flags;
		}
		syncLookupArray = -1;
	}
	
	// Sync targets
	targetArrays[inactiveLookup] = targetArrays[activeLookup];
	
	std::thread comp(&Pathfinder::recomputeLookup, this);
	comp.detach();
	
	return true;
}

sf::Vector2i Pathfinder::nextStep(const sf::Vector2i &position) {
	int pos = position.y * lookupSize.x + position.x;
	assert(pos < lookupSize.x * lookupSize.y);
	
	if (at(pos) == 0) return sf::Vector2i(-2, -2);
	else if (at(pos) == -1) return sf::Vector2i(-1, -1);
	
	int dstpos, minval = -1;
	std::vector<int> directions({-1, -lookupSize.x, 1, lookupSize.x});
	
	for (auto direction : directions) {
		int currval = at(pos + direction);
		
		if (currval != -1 && (minval == -1 or minval > currval)) {
			dstpos = pos + direction;
			minval = currval;
		}
	}
	
	// TODO: Take fast forward into account
	
	int x = dstpos % lookupSize.x;
	int y = dstpos / lookupSize.y;
	
	return sf::Vector2i(x, y);
}

void Pathfinder::changeTileValue(const sf::Vector2i &position, bool blocking) {
	assert(position.y * lookupSize.x + position.x < lookupSize.x * lookupSize.y);
	
	lookupArrays[activeLookup][position.y * lookupSize.x + position.x].setSolid(blocking);
	syncLookupArray = inactiveLookup;
}

void Pathfinder::changeTileValue(const int positionX, const int positionY, bool blocking) {
	assert(positionY * lookupSize.x + positionX < lookupSize.x * lookupSize.y);
	
	lookupArrays[activeLookup][positionY * lookupSize.x + positionX].setSolid(blocking);
	syncLookupArray = inactiveLookup;
}

bool Pathfinder::init(const dgm::Mesh &mesh) {
	if (getComputationProgress() != 100) {
		std::cerr << "Pathfinder::init(...) - Current computation has not ended yet!";
		return false;
	}
	
	std::size_t dataSize = mesh.getDataSize().x * mesh.getDataSize().y;
	lookupSize = mesh.getDataSize();
	
	try {
		lookupArrays.clear();
		lookupArrays.push_back(LookupArray(dataSize));
		lookupArrays.push_back(LookupArray(dataSize));
		
		targetArrays.clear();
		targetArrays.push_back(TargetArray());
		targetArrays.push_back(TargetArray());
	}
	catch (...) {
		std::cerr << "Pathfinder::init(...) - Allocation error!\n";
		return false;
	}
	
	for (int i = 0; i < dataSize; i++) {
		for (int l = 0; l < lookupArrays.size(); l++) {
			lookupArrays[l][i].setSolid(mesh[i] > 0);
		}
	}
	
	activeLookup = 0;
	
	return true;
}

dgm::ai::Pathfinder::Pathfinder() {
	progress = 100;
}

dgm::ai::Pathfinder::~Pathfinder()
{
}
