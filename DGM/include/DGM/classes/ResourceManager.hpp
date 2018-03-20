#pragma once

#include <DGM\dgm.hpp>
#include <map>

namespace dgm {
	class ResourceManager {
	protected:
		std::map<std::string, void*> database;

	public:
		enum class Type { Graphic, Sound, Font };
		
		/**
		 *  \brief Converts filename of a resource into identifier within ResourceManager
		 *  
		 *  \param [in] filename Resource filename
		 *  \param [out] name Output resource identifier
		 *  
		 *  \details Resource identifier is basically its filename without extension.
		 *  This method also removes prepending slashes of folder path.
		 */
		void resourceName(const std::string &filename, std::string &name);
		
		/**
		 *  \brief Load resource into manager
		 *  
		 *  \param [in] filename Path to resource
		 *  \return TRUE on success, FALSE otherwise
		 */
		template<typename T>
		bool loadResource(const std::string &filename) {
			T* res = new T;
			if (res == nullptr) return false;

			if (not res->loadFromFile(filename)) {
				delete res;
				return false;
			}
			
			std::string name;
			resourceName(filename, name);
			database[name] = res;
			
			return true;
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

			return (T*)(database.at(name));
		}
		
		/**
		 *  \brief Loads every resource of a given type from a directory
		 *  
		 *  \param [in] foldername Path to folder to autoload
		 *  \param [in] type Type of resource
		 *  \return TRUE on success, FALSE otherwise
		 */
		bool loadFromDir(const std::string &foldername, dgm::ResourceManager::Type type);
		
		/**
		 *  \brief initialize ResourceManager from Config
		 *  
		 *  \param [in] config Valid loaded config
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details If config contains section Resources, it then checks for
		 *  key autoloadDirectories which has as a value list of folder paths
		 *  separated by a colon
		 */
		bool init(const dgm::Config &config);
		
		ResourceManager();
		~ResourceManager();
	};
}