#pragma once

namespace dgm {
	/**
	 *  \brief Class for defining single named animation state (set of frames)
	 */
	class AnimationState: public Clip {
	private:
		std::string stateName;
		
	public:
		/**
		 *  \brief Gets the name of the state
		 */
		const std::string	&getName();
		
		/**
		 *  \brief Sets a new name of state
		 *  
		 *  \param [in] name New name of state
		 */
		void				setName(std::string name);
		
		AnimationState();
		~AnimationState();
	};
};