/**
 *  \file Animation.hpp
 *  \author doomista
 */

#pragma once

namespace dgm {
	class Animation {
	private:
		sf::Sprite			*sprite;
		dgm::AnimationData	*states;
		dgm::AnimationState	currentState;
		dgm::Counter		timer;
		dgm::Counter		frame;
		unsigned int		i_flags;
		bool				disallowAutomaticCleanup;
		
		void reset();
		void setCurrentFrameToSprite();
	public:
		bool update();
		
		unsigned int getSpeed();
		unsigned int getFlags();
		
		void setState(unsigned int state, unsigned int type = 1);
		void setState(std::string state, unsigned int type = 1);
		bool setSprite(sf::Sprite *sprite);
		void setSpeed(unsigned int speed);
		bool loadFromFile(std::string filename, std::string identifier);
		
		/**
		 *  \brief Uses the preloaded database of states to initializes the object
		 *  
		 *  \details This function simply copies the pointer to data.
		 *  Also disallows automatic cleanup triggered by a function destructor.
		 *  Use this function when you have a lots of animated objects in a 
		 *  dgm::Buffer.
		 *  
		 *  \returns TRUE on success, FALSE if data are NULL.
		 */
		bool loadFromMemory(dgm::AnimationData *data);
		
		void clear();
		
		enum AnimationType {
			Simple = 0,
			Loop = 1,
		};
		
		Animation();
		~Animation();
	};
};