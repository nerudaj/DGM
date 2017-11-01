/**
 *  \file Counter.hpp
 *  \author doomista
 */

#pragma once

namespace dgm {
	/**
	 *  \brief A simple class for handling events that should occur every n-th time.
	 * 
	 *  \details Use this class whenever you need something to happen every N times
	 *  you call update function on this object. For example, dgm::Animation use
	 *  this object for swapping frames of animation and even for handling end of
	 *  animation and looping it.
	 */
	class Counter {
	private:
		unsigned int cnt;	///< Internal counter. Counts from 0 to max.
		unsigned int max;	///< Internal limit for cnt to count to.
		
	public:
		/**
		 *  \brief Updates the counter and checks reaching the limit
		 *  
		 *  \return Counter::Working if limit was not reached. Counter::LimitReached if
		 *  limit was reached. Working is the logic equivalent of FALSE, LimitReached is
		 *  equivalent of TRUE.
		 *  
		 *  \details Compares the internal counter with the limit. If limit was reached,
		 *  resets the counter and yields LimitReached status. Otherwise increments the
		 *  counter and yields Working status.
		 */
		bool			update();
		
		/**
		 *  \brief Resets the counter
		 *  
		 *  \details Internal state of the counter is set to zero, not affecting the
		 *  previously set limit.
		 */
		void			reset();
		
		/**
		 *  \brief Sets a new limit
		 *  
		 *  \param [in] limit Positive integer value
		 *  
		 *  \details New limit for the counter is set. If this limit is set while counter
		 *  is working, you might want to call reset() as well.
		 */
		void			setLimit(unsigned int limit);
		
		/**
		 *  \brief Gets the currently set limit
		 */
		unsigned int	getLimit();
		
		/**
		 *  \brief Gets the current value of the internal counter
		 */
		unsigned int	getValue();
		
		enum Status {
			Working = 0,
			LimitReached = 1,
		};
		
		Counter();
		Counter(unsigned int limit);
		~Counter();
	};
};