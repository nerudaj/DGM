#pragma once

#include <DGM\dgm.hpp>
#include <map>
#include <filesystem>

namespace dgm {
	class ResourceManager {
	private:
		bool pedantic;

		std::map<std::string, void*> database;
		std::string commonPrefix;

		void loadResourceFromFile(const std::string &filename, sf::Texture &texture);
		void loadResourceFromFile(const std::string &filename, sf::Font &font);
		void loadResourceFromFile(const std::string &filename, sf::SoundBuffer &sound);
		void loadResourceFromFile(const std::string &filename, std::shared_ptr<dgm::AnimationStates> &states);

	public:
		bool isResourceInDatabase(const std::string &id) const {
			return database.find(id) != database.end();
		}

		void setPedantic(bool enable) {
			pedantic = enable;
		}

		/**
		 *  \brief Get resource ID based on its filename
		 */
		std::string getResourceName(const std::string &filename);

		template<typename T>
		T &get(const std::string &id) const {
			if (not isResourceInDatabase(id)) {
				throw dgm::GeneralException("Resource '" + id + "' is not in database.");
			}

			return dynamic_cast<T&>(*((T*)(database.at(id))));
		}

		template<typename T>
		void loadResource(const std::string &filename);

		template<typename T>
		void loadResourceDir(const std::string &folder, bool recursive = false);

		ResourceManager();
		ResourceManager(const ResourceManager &other) = delete;
		ResourceManager(ResourceManager &&other);
		~ResourceManager();
	};
}