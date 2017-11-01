#include <DGM/dgm.hpp>

using std::cerr;
using std::cout;
using std::string;
using std::ifstream;
using std::vector;

using namespace dgm;

/* ====================== */
/* LOCAL HELPER FUNCTIONS */
/* ====================== */
void logError(string msg) {
	cerr << "dgm::Clip::loadFromFile(...) - " << msg;
}

// Returns value of expression, prints errorMsg using logError if expression was false
bool evaluateExpression(bool expression, string errorMsg) {
	if (!expression) {
		logError(errorMsg);
	}
	
	return expression;
}

/* =========================== */
/* CLASS FUNCTIONS DEFINITIONS */
/* =========================== */
	/* -------- */
	/* GET SIZE */
	/* -------- */
unsigned int Clip::getSize() const {
	return size;
}

sf::Vector2i Clip::getTileSize() const {
	return sf::Vector2i(clips[0].width, clips[0].height);
}

	/* --------- */
	/* GET FRAME */
	/* --------- */
const sf::IntRect &Clip::getFrame(unsigned int index) const {
	#ifdef _DEBUG
	if (index >= size) {
		cerr << "dgm::Clip::getFrame(...) - index has value of " << index << " but size of the Clip is " << size << ". Returning first item in the array so the program won't crash.\n";
		return clips[0];
	}
	#endif
	
	return clips[index];
}

	/* -------------- */
	/* LOAD FROM FILE */
	/* -------------- */
bool Clip::loadFromFile(string filename, string identifier) {
	ifstream Load(filename);
	
	if (!evaluateExpression(Load.good(), "Could not open file '" + filename + "'.\n")) {
		return false;
	}
	
	string buffer;
	vector<string> split;
	
	// *** output ***
	unsigned long 	frames = 0;
	sf::Vector2i	frameSize	(0, 0);
	sf::Vector2i	frameOffset	(0, 0);
	sf::IntRect		bounds		(0, 0, 0, 0);
	bool loading = false, error = false;

	while (Load >> buffer) {
		if (!loading) {
			if (buffer == "start") {
				Load >> buffer;
				dgm::Strings::split('=', buffer, split);
				
				if (split[0] == "clip" && split[1] == identifier) {
					loading = true;
				}
				
				split.clear();
			}
			
			continue;
		}
		
		dgm::Strings::split('=', buffer, split);
		
		if (split.size() == 2) {
			buffer = split[1];
		}

		if (split[0] == "frames") {
			frames = strtol(split[1].c_str(), NULL, 10);
		}
		else if (split[0] == "frameSize") {
			error = !evaluateExpression(dgm::Conversion::stringToVector2i(':', buffer, frameSize), "Frame size does not have two parameters concatenated with a ':' character.\n\tExample: frameSize=64:64\n");
		}
		else if (split[0] == "frameOffset") {
			error = !evaluateExpression(dgm::Conversion::stringToVector2i(':', buffer, frameOffset), "Frame offset does not have two parameters concatenated with a ':' character.\n\tExample: frameOffset=0:0\n");
		}
		else if (split[0] == "boundaries") {
			error = !evaluateExpression(dgm::Conversion::stringToIntRect(':', buffer, bounds), "Boundaries do not have four parameters concatenated with a ':' character.\n\tExample: bounds=0:0:256:128\n");
		}
		else if (split[0] == "end") {
			loading = false;
		}
		else {
			error = !evaluateExpression(false, "Keyword '" + split[0] + "' is not valid. Valid keywords are: 'frames', 'frameSize', 'frameOffset' and 'boundaries'");
		}
		
		split.clear();
		
		if (!loading || error) {
			break;
		}
	}

	Load.close();
	Load.clear();
	
	// *** error checking ***
	// If loading is true, then it's an error (no 'end' expression)
	error = !evaluateExpression(!loading, 								"Keyword '' is not valid. Valid keywords are: 'frames', 'frameSize', 'frameOffset' and 'bounds'");
	error = !evaluateExpression((frameSize.x >= 1 || frameSize.y >= 1),	"Frame size has an invalid value. Check it.\n");
	
	if (error) {
		return false;
	}

	#ifdef _DEBUG
	cout << "Loaded " << identifier << std::endl;
	cout << "FrameSize = [" << frameSize.x << ", " << frameSize.y << "]\n";
	cout << "FrameOffs = [" << frameOffset.x << ", " << frameOffset.y << "]\n";
	cout << "Bounds = [" << bounds.left << ", " << bounds.top << "] x [" << bounds.width << ", " << bounds.height << "]\n";
	cout << "Size = " << size << "\n";
	#endif
	
	if(!evaluateExpression(loadFromParameters(frameSize, bounds, size, frameOffset), "Something really bad happened during computeClippingData(...).\n")) {
		return false;
	}
	
	return true;
}

	/* ---------------- */
	/* LOAD FROM MEMORY */
	/* ---------------- */
bool Clip::loadFromMemory(sf::IntRect *data, unsigned int frames) {
	if (data == NULL) {
		cerr << "dgm::classes::Clip::loadFromMemory - Provided data pointer is NULL.\n";
		return false;
	}
	
	clips = data;
	size = frames;
	
	return true;
}

	/* -------------------- */
	/* LOAD FROM PARAMETERS */
	/* -------------------- */
bool Clip::loadFromParameters(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frames, const sf::Vector2i &frameOffset) {
	#ifdef _DEBUG
	if (frameSize.x <= 0 || frameSize.y <= 0) {
		cerr << "dgm::Clip::loadFromParameters(...) - Neither dimension of frameSize can have zero or negative value. Fix it! [DEBUG msg only]\n";
		return false;
	}
	#endif
	
	if (frames == 0) {
		int x = frameSize.x + frameOffset.x;
		int y = frameSize.y + frameOffset.y;
		
		frames = (boundaries.width / x) * (boundaries.height / y);
		#ifdef _DEBUG
		std::cout << "Computed frames = " << frames << std::endl;
		#endif
	}
	
	clips = new sf::IntRect[frames];
	
	if (clips == NULL) {
		cerr << "dgm::Clip::loadFromParameters(...) - Could not allocate memory for " << frames << " frames.\n";
		return false;
	}
	
	unsigned long i = 0;
	
	for(unsigned long y = boundaries.top; y < unsigned long(boundaries.top + boundaries.height); y+=(frameSize.y + frameOffset.y)) {
		for(unsigned long x = boundaries.left; x < unsigned long(boundaries.left + boundaries.width); x+=(frameSize.x + frameOffset.x)) {
			clips[i] = sf::IntRect(x, y, frameSize.x, frameSize.y);
			i++;
			
			if (i == frames) {
				x = (boundaries.left + boundaries.width);
				y = (boundaries.top + boundaries.height);
				break;
			}
		}
	}
	
	size = frames;
	return true;
}

	/* ---- */
	/* FREE */
	/* ---- */
void Clip::clear() {
	if (clips != NULL) {
		delete[] clips;
	}

	clips = NULL;
	size = 0;
}


/* ================================ */
/* CLASS CONSTRUCTORS & DESTRUCTORS */
/* ================================ */
Clip::Clip() {
	size = 0;
	clips = NULL;
}

Clip::Clip(std::string filename, std::string identifier) {
	loadFromFile(filename, identifier);
}

Clip::Clip(const sf::Vector2i &frameSize, const sf::IntRect &boundaries, unsigned int frames, const sf::Vector2i &frameOffset) {
	loadFromParameters(frameSize, boundaries, frames, frameOffset);
}

Clip::~Clip() {
	clear();
}