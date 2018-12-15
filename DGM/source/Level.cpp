#include <DGM\dgm.hpp>

sf::IntRect dgm::Level::findClipBoundaries() {
	const dgm::Clip &clip = renderer.getClip();
	sf::IntRect boundaries = clip.getFrame(0);
	for (size_t i = 1; i < clip.getFrameCount(); i++) {
		const sf::IntRect &r = clip.getFrame(i);

		if (boundaries.width < r.left + r.width) {
			boundaries.width = r.left + r.width;
		}
		if (boundaries.height < r.top + r.height) {
			boundaries.height = r.top + r.height;
		}
	}

	boundaries.width -= boundaries.left;
	boundaries.height -= boundaries.top;

	return boundaries;
}

sf::Vector2i dgm::Level::findClipFrameOffset(const sf::IntRect &bounds) {
	const dgm::Clip &clip = renderer.getClip();

	int framesPerRow = bounds.width / clip.getFrameSize().x;
	int framesPerColumn = bounds.height / clip.getFrameSize().y;

	sf::Vector2i result(0, 0);
	int differenceX = bounds.width - framesPerRow * clip.getFrameSize().x;
	if (differenceX != 0) {
		result.x = differenceX / (framesPerRow - 1);
	}
	int differenceY = bounds.height - framesPerColumn * clip.getFrameSize().y;
	if (differenceY != 0) {
		result.y = differenceY / (framesPerColumn - 1);
	}

	return result;
}

void dgm::Level::loadCompressed(std::ifstream & load) {
	sf::Vector2i frameSize;
	sf::Vector2i frameOffset;
	sf::Vector2i dataSize;
	sf::Vector2i tileSize;
	sf::IntRect bounds;
	int frames = 0;
	dgm::Clip clip;

	// Load header
	const std::size_t headerSize = sizeof(sf::Vector2i) * 4 + sizeof(int) + sizeof(sf::IntRect);
	char header[headerSize];
	load.read(header, headerSize);

	// Assign header to items
	dataSize = *(sf::Vector2i*)(header);
	tileSize = *(sf::Vector2i*)(header + sizeof(sf::Vector2i));
	frameSize = *(sf::Vector2i*)(header + sizeof(sf::Vector2i) * 2);
	frameOffset = *(sf::Vector2i*)(header + sizeof(sf::Vector2i) * 3);
	frames = *(int*)(header + sizeof(sf::Vector2i) * 4);
	bounds = *(sf::IntRect*)(header + sizeof(sf::Vector2f) * 4 + sizeof(int));

	if (not clip.init(frameSize, bounds, frames, frameOffset)) throw std::exception("Invalid values for constructing clip");

	int item;
	for (int y = 0; y < dataSize.y; y++) {
		for (int x = 0; x < dataSize.x; x++) {
			load.read((char*)(&item), sizeof(int));
			imageData[y * dataSize.x + x] = item;
			load.read((char*)(&item), sizeof(int));
			mesh[y * dataSize.x + x] = item;
		}
	}

	renderer.build(clip, tileSize, imageData, dataSize);
}

void dgm::Level::loadUncompressed(std::ifstream & load) {
	sf::Vector2i frameSize;
	sf::Vector2i frameOffset = sf::Vector2i(0, 0);
	sf::Vector2i dataSize;
	sf::Vector2i tileSize;
	sf::IntRect bounds;
	int frames = 0;
	dgm::Clip clip;

	std::string buffer;
	std::vector<std::string> split;
	int mode = 0; // 0 - headers, 1 - data, 2 - end
	size_t tileIndex = 0;
	while (mode < 2) {
		load >> buffer;

		if (mode == 0) {
			Strings::split('=', buffer, split);
			
			if (split.size() != 2) throw std::exception("Expected key=value pair in header section");

			if (split[0] == "data") {
				if (split[1] == "begin") {
					if (not clip.init(frameSize, bounds, frames, frameOffset)) {
						throw std::exception("Invalid values for constructing clip");
					}
					if (tileSize.x <= 0 || tileSize.y <= 0) {
						throw std::exception("Non-positive values are not allowed for tileSize");
					}
					if (dataSize.x <= 0 || dataSize.y <= 0) {
						throw std::exception("Non-positive values are not allowed for dataSize");
					}

					mesh.setDataSize(dataSize);
					mesh.setVoxelSize(frameSize);
					imageData.resize(dataSize.x * dataSize.y);

					mode = 1;
				}
				else throw std::exception("Data section must begin with data=begin statement");
			}
			else if (split[0] == "dataSize") {
				if (not dgm::Conversion::stringToVector2i(':', split[1], dataSize)) {
					throw std::exception("Could not properly parse dataSize");
				}
			}
			else if (split[0] == "tileSize") {
				if (not dgm::Conversion::stringToVector2i(':', split[1], tileSize)) {
					throw std::exception("Could not properly parse tileSize");
				}
			}
			else if (split[0] == "frameSize") {
				if (not dgm::Conversion::stringToVector2i(':', split[1], frameSize)) {
					throw std::exception("Could not properly parse frameSize");
				}
			}
			else if (split[0] == "frameOffset") {
				if (not dgm::Conversion::stringToVector2i(':', split[1], frameOffset)) {
					throw std::exception("Could not properly parse frameOffset");
				}
			}
			else if (split[0] == "frames") {
				frames = std::stoi(split[1]);
			}
			else if (split[0] == "boundaries") {
				if (not dgm::Conversion::stringToIntRect(':', split[1], bounds)) {
					throw std::exception("Could not properly parse boundaries");
				}
			}
			else {
				throw std::exception("Unexpected key in header section. Valid keys are: dataSize, tileSize, frameSize, frameOffset, frames, boundaries and data");
			}
		}
		else if (mode == 1) {
			Strings::split(':', buffer, split);

			if (split.size() != 2) throw std::exception("Expected key=value pair in header section");

			imageData[tileIndex] = std::stoi(split[0]);
			mesh[tileIndex] = std::stoi(split[1]);

			tileIndex++;
			if (dataSize.x * dataSize.y == tileIndex) mode = 2;
		}
	}

	renderer.build(clip, tileSize, imageData, dataSize);
}

void dgm::Level::saveCompressed(std::ofstream & save) {
	save.write((char*)(&mesh.getDataSize()), sizeof(sf::Vector2i));
	save.write((char*)(&mesh.getVoxelSize()), sizeof(sf::Vector2i));

	sf::IntRect bounds = findClipBoundaries();
	sf::Vector2i frameOffset = findClipFrameOffset(bounds);
	save.write((char*)(&(renderer.getClip().getFrameSize())), sizeof(sf::Vector2i));
	save.write((char*)(&frameOffset), sizeof(sf::Vector2i));
	int frames = renderer.getClip().getFrameCount();
	save.write((char*)(&frames), sizeof(int));
	save.write((char*)(&bounds), sizeof(sf::IntRect));

	int item;
	for (int y = 0; y < mesh.getDataSize().y; y++) {
		for (int x = 0; x < mesh.getDataSize().x; x++) {
			item = imageData[y * mesh.getDataSize().x + x];
			save.write((char*)(&item), sizeof(int));
			item = mesh[y * mesh.getDataSize().x + x];
			save.write((char*)(&item), sizeof(int));
		}
	}
}

void dgm::Level::saveUncompressed(std::ofstream & save) {
	save << "u\n";
	save << "dataSize=" << mesh.getDataSize().x << ":" << mesh.getDataSize().y << "\n";
	save << "tileSize=" << mesh.getVoxelSize().x << ":" << mesh.getVoxelSize().y << "\n";

	sf::IntRect boundaries = findClipBoundaries();
	sf::Vector2i frameOffset = findClipFrameOffset(boundaries);

	save << "frameSize=" << renderer.getClip().getFrameSize().x << ":" << renderer.getClip().getFrameSize().y << "\n";
	if (frameOffset.x != 0 || frameOffset.y != 0) {
		save << "frameOffset=" << frameOffset.x << ":" << frameOffset.y << "\n";
	}
	save << "frames=" << renderer.getClip().getFrameCount() << "\n";
	save << "boundaries=" << boundaries.left << ":" << boundaries.top << ":" << boundaries.width << ":" << boundaries.height << "\n";

	save << "data=begin\n";

	for (int y = 0; y < mesh.getDataSize().y; y++) {
		for (int x = 0; x < mesh.getDataSize().x; x++) {
			int i = y * mesh.getDataSize().x + x;
			save << imageData[i] << ":" << mesh[i] << " ";
		}
		save << "\n";
	}
}

void dgm::Level::changeTile(uint32_t tileX, uint32_t tileY, uint32_t tileValue) {
	assert(tileX < uint32_t(mesh.getDataSize().x) && tileY < uint32_t(mesh.getDataSize().y));
	renderer.changeTile(tileX, tileY, tileValue);
	imageData[tileY * mesh.getDataSize().x + tileX] = tileValue;
}

void dgm::Level::build(const dgm::Clip & clip, const sf::Vector2i tileSize, const std::vector<int>& imageData, const std::vector<int> &collisionData, const sf::Vector2i & dataSize) {
	assert(imageData.size() == collisionData.size());

	mesh.setVoxelSize(tileSize);
	mesh.setDataSize(dataSize);
	for (std::size_t i = 0; i < collisionData.size(); i++) {
		mesh[i] = collisionData[i];
	}

	Level::imageData = imageData;

	renderer.build(clip, tileSize, imageData, dataSize);
}

bool dgm::Level::loadFromFile(const std::string & filename) {
	std::ifstream load(filename);

	if (not load.good()) {
		std::cerr << "Level::loadFromFile(...) - Could not open file " << filename << ".\n";
		return false;
	}

	try {
		char mode;
		load.read(&mode, 1);

		if (mode == 'c') {
			loadCompressed(load);
		}
		else {
			loadUncompressed(load);
		}
	}
	catch (std::exception &e) {
		std::cerr << "Level::loadFromFile(...) - Exception: " << e.what() << "\n";
		return false;
	}

	return true;
}

bool dgm::Level::saveToFile(const std::string & filename, bool compressed) {
	std::ofstream save(filename);

	if (not save.good()) {
		std::cerr << "Level::saveToFile(...) - Could not open file " << filename << ".\n";
		return false;
	}

	try {
		if (compressed) {
			saveCompressed(save);
		}
		else {
			saveUncompressed(save);
		}
	}
	catch (std::exception &e) {
		std::cerr << "Level::saveToFile(...) - Exception: " << e.what() << "\n";
		return false;
	}

	return true;
}

dgm::Level::Level() {
}

dgm::Level::~Level() {
}
