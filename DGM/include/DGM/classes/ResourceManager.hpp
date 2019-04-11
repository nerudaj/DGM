#pragma once

#include <DGM\dgm.hpp>
#include <map>
#include <regex>

namespace dgm {
	class ResourceManager {
	protected:
		std::map<std::string, void*> database;

		void deinit();

	public:
		enum class Type : size_t {
			Graphic, Sound, Font, AnimationData
		};
		
		/**
		 *  \brief Converts filename of a resource into identifier within ResourceManager
		 *  
		 *  \param [in] filename Resource filename
		 *  \param [out] name Output resource identifier
		 *  
		 *  \details Resource identifier is basically its filename without extension.
		 *  This method also removes prepending slashes of folder path. After that,
		 *  a special prefix is added, namespacing each class name.
		 */
		template<typename T>
		std::string resourceName(const std::string &filename) {
			std::string name;
			
			std::regex exprPrefix(".*\\/.*\\/");
			name = std::regex_replace(filename, exprPrefix, "");
			std::regex exprPrefixAlt(".*\\\\.*\\\\");
			name = std::regex_replace(name, exprPrefixAlt, "");
			std::regex exprSuffix("\\....$");
			name = std::regex_replace(name, exprSuffix, "");

			if (std::is_same<sf::Texture, T>::value) name = "t-" + name;
			else if (std::is_same<sf::Font, T>::value) name = "f-" + name;
			else if (std::is_same<sf::SoundBuffer, T>::value) name = "s-" + name;
			else if (std::is_same<dgm::AnimationData, T>::value) name = "a-" + name;

			return name;
		}

		/**
		 *  \brief get resource pointer
		 *
		 *  \param [in] name Identifier of the resource
		 *  \return nullptr if name was not found, pointer to resource otherwise
		 */
		template<typename T>
		T *get(const std::string &name) {
			if (database.find(name) == database.end()) return nullptr;

			return dynamic_cast<T*>((T*)(database.at(name)));
		}
		
		/**
		 *  \brief Load resource into manager
		 *  
		 *  \param [in] filename Path to resource
		 *  \return TRUE on success, FALSE otherwise
		 */
		template<typename T>
		bool loadResource(const std::string &filename) {
			std::string name = resourceName<T>(filename);

			if (database.find(name) != database.end()) {
				std::cerr << "ResourceManager::loadResource(...) - Resource with name " << name << " already exists\n";
				return false;
			}

			T* res = new T;
			if (res == nullptr) return false;

			if (not res->loadFromFile(filename)) {
				delete res;
				return false;
			}
			
			database[name] = res;
			
			return true;
		}

		/**
		 *  \brief Loads every resource of a given type from a directory
		 *  
		 *  \param [in] foldername Path to folder to autoload
		 *  \param [in] type Type of resource
		 *  \param [out] names Names of loaded resources
		 *  \return TRUE on success, FALSE otherwise
		 */
		bool loadFromDir(const std::string &foldername, dgm::ResourceManager::Type type, std::vector<std::string> *names = nullptr);
		
		template<typename T>
		bool addResource(const std::string &name, T *resource) {
			std::string realname;
			resourceName(name, realname);
			if (database.find(realname) != database.end()) {
				std::cerr << "ResourceManager::addResource(...) - Resource with name " << realname << " already exists\n";
				return false;
			}

			T* res = new T;
			if (res == nullptr) return false;

			&res = &resource; // NOTE: Does this perform deep copy?
			database[realname] = res;

			return true;
		}

		void freeResource(const std::string &name);
		
		ResourceManager();
		~ResourceManager();
	};
}