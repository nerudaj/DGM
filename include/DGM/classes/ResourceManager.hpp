#pragma once

#include <DGM\dgm.hpp>
#include <map>
#include <filesystem>

namespace dgm {
	class ResourceManager {
	private:
		std::map<std::string, void*> database;
		std::string commonPrefix;

		void loadResourceFromFile(const std::string &filename, sf::Texture &texture);
		void loadResourceFromFile(const std::string &filename, sf::Font &font);
		void loadResourceFromFile(const std::string &filename, sf::SoundBuffer &sound);
		void loadResourceFromFile(const std::string &filename, std::shared_ptr<dgm::AnimationStates> &states);

	public:
		bool isResourceInDatabase(const std::string &id) {
			return database.find(id) != database.end();
		}

		/**
		 *  \brief Get resource ID based on its filename
		 */
		std::string getResourceName(const std::string &filename);

		template<typename T>
		T * get(const std::string &id) const {
			if (not isResourceInDatabase(id)) {
				throw dgm::GeneralException("Resource '" + id + "' is not in database.");
			}

			return dynamic_cast<T*>((T*)(database.at(name)));
		}

		template<typename T>
		void loadResource(const std::string &filename) {
			std::string name = getResourceName<T>(filename);
			if (isResourceInDatabase(name)) {
				throw dgm::GeneralException("Resource called '" + name + "' is already in database!");
			}

			T *resource = new T;
			if (!T) {
				throw dgm::GeneralException("Could not allocate memory for resource");
			}

			try {
				loadResourceFromFile(filename, resource);
			}
			catch (std::exception &e) {
				throw dgm::GeneralException("Could not load resource '" + filename + "', reason: '" + std::string(e.what()) + "'");
			}

			try {
				database[name] = resource;
			}
			catch (std::exception &e) {
				throw dgm::GeneralException("Could not insert resource into database");
			}
		}

		template<typename T>
		void loadResourceDir(const std::string &folder, bool recursive) {
			namespace fs = std::experimental::filesystem::v1;

			fs::path path(folder);
			if (not fs::is_directory(path)) {}

			fs::directory_iterator itr(path);
			for (auto item : itr) {
				fs::path itemPath(item);

				if (fs::is_directory(itemPath) && recursive) {
					loadResourceDir<T>(folder, recursive);
				}

				loadResource<T>(itemPath);
			}
		}

		ResourceManager();
		ResourceManager(const ResourceManager &other) = delete;
		ResourceManager(ResourceManager &&other);
		~ResourceManager();
	};
}