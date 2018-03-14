#pragma once

#include <DGM\dgm.hpp>
#include <map>

namespace dgm {
	class AnimationData : public std::map<std::string, dgm::Clip> {
	public:
		/**
		 *  \brief Load database from file
		 *  
		 *  \param [in] name Name of the file
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details TODO: Syntax
		 */
		bool LoadFromFile(const std::string &name);
		
		/**
		 *  \brief Add state to database
		 *  
		 *  \param [in] stateName Name of the state
		 *  \param [in] clip Clip object describing the frames
		 *  \return TRUE on success, FALSE if state with stateName already exists
		 */
		bool AddState(const std::string &stateName, const dgm::Clip &clip);
	};
	
	/**
	 *  \brief Class for animating objects
	 */
	class Animation {
	protected:
		sf::Time elapsed; ///< Time elapsed since last frame change
		sf::Time timePerFrame; ///< Time between two frame changes
		std::size_t frameIndex; ///< Index of current frame within animation
		int flags; ///< Flags for current animation
		bool mallocked; ///< Whether animations were allocated inside or outside of this class
		sf::Sprite *boundSprite; ///< Animated sprite
		AnimationData *animations; ///< Database of animation state
		AnimationData::iterator currentState; ///< Iterator to selected animation state
		
		void Reset();
		
	public:
		class Flags {
		public:
			enum { RunOnce = 0, Looping = 1 };
		};
	
		/**
		 *  \brief Update animation counters
		 *  
		 *  \param [in] time Time elapsed since last call to this method
		 *  \return TRUE if animation continues, FALSE if animation ended
		 *  
		 *  \details Animation can only end when the current state was set as not
		 *  looping.
		 */
		bool Update(const dgm::Time &time);
		
		/**
		 *  \brief Bind sprite to animation object
		 *  
		 *  \param [in] sprite Pointer to valid sprite object
		 */
		void SetSprite(sf::Sprite *sprite);
		
		/**
		 *  \brief Set speed of animation
		 *  
		 *  \param [in] fps How many frames per sec should happen
		 */
		void SetSpeed(int fps);
		
		/**
		 *  \brief Change animation state
		 *  
		 *  \param [in] state Name of the state
		 *  \param [in] flags How animation should behave
		 *  \return TRUE if state was found and set, FALSE otherwise
		 *  
		 *  \details If state was not found, previous state is still running.
		 *  If flags are set to RunOnce, animation will end after passing last
		 *  frame and Update will return FALSE. With Looping, animation will start
		 *  over by itself and Update always returns TRUE.
		 */
		bool SetState(const std::string &state, int flags = dgm::Animation::Flags::Looping);
		
		/**
		 *  \brief Get number of frames per second
		 */
		int GetSpeed() const { return 1000 / timePerFrame.asMilliseconds(); }
		
		/**
		 *  \brief Get name of selected state
		 */
		const std::string &GetState() const { return currentState->first; }
		
		/**
		 *  \brief Load animation database from file
		 *  
		 *  \param [in] filename File with data to load
		 *  \return TRUE on success, FALSE otherwise
		 */
		bool LoadFromFile(const std::string &filename);
		
		/**
		 *  \brief Initialize object from existing database
		 *  
		 *  \param [in] data Initialized animation database
		 *  \return TRUE on success, FALSE otherwise
		 *  
		 *  \details This method will only take pointer to existing data and
		 *  work with them, no copying involved. It is useful when you're using
		 *  ResourceManager and/or want to initialize a lot of animation objects
		 *  with the same data, saving on memory.
		 */
		bool LoadFromMemory(dgm::AnimationData *data);
		
		/**
		 *  \brief Clear the memory
		 *  
		 *  \details This is called automatically by destructor
		 */
		void Deinit();
		
		Animation();
		~Animation();
	};
}