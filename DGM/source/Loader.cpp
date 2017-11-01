#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <pair>
#include <fstream>

namespace dgm {
	struct LoaderAttribute {
		std::string name;
		std::size_t size;
		
		LoaderAttribute(const std::string name, const std::size_t size);
	};
	
	typedef std::vector<LoaderAttribute>									LoaderAttributes;
	typedef std::vector<std::pair<std::string, LoaderAttributes>>			LoaderInput;
	typedef std::vector<std::pair<std::string, std::unordered_map<std::string, std::vector<int>>>>	LoaderOutput;
	
	class Loader {
	public:
		static bool loadFromFile(const std::string &filename, const std::string &identifier, const LoaderInput &input, LoaderOutput &output);
	}
}

int main() {
	LoaderAttributes clipAttrs;
	attrs.push_back(LoaderAttribute("boundaries", true, 4));
	attrs.push_back(LoaderAttribute("frameSize", true, 2));
	attrs.push_back(LoaderAttribute("frameOffset", false, 2));
	attrs.push_pack(LoaderAttribute("frames", false, 1));
	
	LoaderInput loadInput;
	loadInput.push_back("clip", clipAttrs);
	
	LoaderOutput loadOutput;
	
	if (not dgm::Loader::loadFromFile("file.tex", "entity-id", loadInput, loadOutput)) {
		std::cerr << "I could not load anything.\n";
		return 1;
	}
	
	// Read output
	if (not loadOutput[0].first == "clip") {
		std::cerr << "Something bad happened.\n";
		return 1;
	}
	
	std::cout << "Frame size: " << loadOutput[0].second["frameSize"][0] << "; " << loadOutput[0].second["frameSize"][1] << std::endl;

	return 0;
}

bool loadFromFile(const std::string &filename, const std::string &identifier, const LoaderInput &input, LoaderOutput &output) {
	std::ifstream load(filename);
	std::string ErrHeader("Loader::loadFromFile(...) - ");
	
	if (not load.good()) {
		std::cerr << ErrHeader << "Could not open '" << filename << "' for reading.\n";
		return false;
	}
	
	std::string buffer;
	std::vector<std::string> split;
	bool run = true, error = false;
	while(run and not error) {
		if (not load.good()) {
			std::cerr << ErrHeader << "'" << filename << "' is corrupted.\n";
			error = true;
			break;
		}
		
		load >> buffer;
		
		switch(state) {
		case 0:	// Start
			if (load == "start") state = 1;
			break;
			
		case 1: // Test entity and id
			split = dgm::Strings::split('=', buffer);
			
			if (split.size() != 2) {
				std::cerr << ErrHeader << "Expected entity name and entity identifier, got " << buffer << " instead.\n";
				error = true;
			}
			else if (split[0] == input[0].first and split[1] == identifier) {
				output.push_back(std::make_pair(input[0].first, LoaderAttributes));
				state = 2;
			} 
			break;
			
		case 2: // Loading
			if (buffer == "start") {
				// TODO: subentity
			}
			else if (buffer == "end") run = false;
			else {
				split = dgm::Strings::split('=', buffer);
				
				if (split.size() != 2) {
					std::cerr << ErrHeader << "Expected attribute name and attribute values, got " << buffer << " instead.\n";
					error = true;
				}
				else {
					if (not validAttribute(split[0], input)) {
						std::cerr << ErrHeader << split[0] << " is not valid attribute of " << input[0].first << ".\n";
						error = true;
					}
					else {
						
					}
				}
			}
			break;
		}
		
		split.clear();
	}
	
	load.close();
	load.clear();
	
	return false;
}