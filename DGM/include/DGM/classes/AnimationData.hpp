/**
 *  \file AnimationData.hpp
 *  \author doomista
 */

#pragma once

namespace dgm {
	class AnimationData {
	private:
		AnimationState	*states;
		unsigned int	size;
		bool			disallowAutomaticCleanup;
		
	public:
		unsigned int			getSize();
		const AnimationState	&getState(std::string identifier) const;
		const AnimationState	&getState(unsigned int index) const;
	
		bool loadFromFile(std::string filename, std::string identifier);
		bool loadFromMemory(AnimationState *data, unsigned int dataSize);
		
		void clear();
		
		AnimationData();
		~AnimationData();
	};
};