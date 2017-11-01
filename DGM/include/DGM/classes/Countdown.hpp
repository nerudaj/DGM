/**
 *  \file Countdown.hpp
 *  \author doomista
 */

#pragma once

namespace dgm {
	/**
	 *  \brief A simple class for tracking time based actions
	 *  
	 *  \details Countdown has a similar function to the Counter - after it was
	 *  updated N times, it yields Countdown::Finished and stops (unlike Counter
	 *  which resets itself). Countdown is meant to be used for tracking events that
	 *  happens sporadically like reloads and that do not repeat itself automatically.
	 */
	class Countdown {
	private:
		unsigned int cnt;
		
	public:
		/**
		 *  \brief Updates a countdown and yields its status
		 *  
		 *  \return Returns Status::Working if internal counter is nonzero,
		 *  Status::Ended otherwise.
		 *  
		 *  \details The internal counter is decremented. If it reaches zero,
		 *  Status::Ended is returned.
		 */
		bool			update();
		
		/**
		 *  \brief Initializes the countdown
		 *  
		 *  \details The internal counter is set to value.
		 */
		void			start(unsigned int value);
		
		/**
		*  \brief Stops the countdown
		*
		*  \details Internal counter is set to zero.
		*/
		void			stop();

		/**
		 *  \brief Returns status of countdown
		 *  
		 *  \return Returns Status::Working if internal counter is nonzero, Status::Ended otherwise.
		 */
		bool			ended();
		
		/**
		 *  \brief Gets value of internal counter
		 */
		unsigned int	getValue();
		
		enum Status {
			Working,
			Ended,
		};
		
		Countdown();
		Countdown(unsigned int value);
		~Countdown();
	};
};