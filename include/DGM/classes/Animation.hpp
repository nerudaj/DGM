#pragma once

#include <DGM\dgm.hpp>
#include <map>
#include <functional>

namespace dgm {
	typedef std::map<std::string, dgm::Clip> AnimationStates;

	/**
	 *  \brief Object animating a single sprite
	 * 
	 *  \details Animation contains a database of animation states
	 *  and a pointer to some sprite. When you set a certain named state
	 *  and start calling update, then the image displayed on sprite
	 *  will be changed.
	 */
	class Animation {
	private:
		std::reference_wrapper<const AnimationStates> states;
		AnimationStates *localInstance;
		sf::Sprite *boundSprite;
		sf::Time elapsedTime;
		sf::Time timePerFrame;
		std::size_t currentFrameIndex;
		AnimationStates::const_iterator currentState;
		bool looping;

		bool isLocallyInstantiated() const {
			return localInstance;
		}

		void updateSprite();

		void instantiateLocally();

	public:
		/**
		 *  \brief Update animation object with time
		 *
		 *  \details If time progressed by enough amount (defined by \ref setSpeed)
		 *  then boundSprite is updated to show next frame of
		 *  the animation. If no more frames are available for
		 *  currently set state and looping is set to false, then
		 *  update will return false. Otherwise it returns true.
		 */
		bool update(const dgm::Time &time);

		/**
		 *  \brief Set the new animation state
		 *
		 *  \details Animation will play from the first
		 *  frame of the new animation state. If looping is set
		 *  to true then animation will start over when it finishes
		 *  and \ref update will always return true.
		 */
		void setState(const std::string &state, bool looping = false);

		/**
		 *  \brief Bind sprite to animation
		 */
		void bindSprite(sf::Sprite *sprite);

		/**
		 *  \brief Set speed of animation in frames per second
		 */
		void setSpeed(int framesPerSecond);

		void setLooping(bool looping) {
			Animation::looping = looping;
		}

		/**
		 *  \brief Get speed as number of frames per second
		 */
		int getSpeed() const {
			return 1000.f / timePerFrame.asMilliseconds();
		}

		/**
		 *  \brief Get name of currently selected state
		 */
		const std::string &getStateName() const {
			return currentState->first;
		}

		bool isLooping() const {
			return looping;
		}

		static AnimationStates loadStatesFromFile(const std::string &filename);

		Animation();
		Animation(const std::string &filename, int framesPerSecond = 30);
		Animation(const AnimationStates &states, int framesPerSecond = 30);
		Animation(Animation &&other);
		Animation(const Animation &other);
		~Animation();
	};
}