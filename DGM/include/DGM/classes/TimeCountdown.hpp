#pragma once

namespace dgm {
	////////////////////////////////////////////////////////////////////////////////
	/// \brief A simple class for tracking time based actions
	/// 
	/// \details This is an upgraded countdown which can be used for displaying
	/// time based countdown in games. It counts on a formatted string which can
	/// be obtained anytime for binding it to sf::Text object.
	/// Following formats are supported: S, SS, M:SS, MM:SS, H:MM:SS, HH:MM:SS
	////////////////////////////////////////////////////////////////////////////////
	class TimeCountdown {
	private:
		bool			stopped;
		std::string		text;
		dgm::Counter	tick;
		
		// index is indexing from the end
		bool substract(int index, int iter);
		
	public:
		bool	update();
		const	std::string &getValue() const;
		
		////////////////////////////////////////////////////////////////////////////////
		/// \brief Initializes the countdown
		/// 
		/// \param [in] value A string in a correct format, see details
		/// \param [in] ticksInSecond basically this should be equal to your app's FPS.
		/// This value tells the object how many updates have to be called before the
		/// time is decremented by one second.
		///
		/// \details The string is scanned and its validity is checked. Then it is set
		/// as a new time to countdown from.
		/// Following formats are supported: S, SS, M:SS, MM:SS, H:MM:SS, HH:MM:SS
		/// If the format is not correct, error will be printed to stderr.
		///
		/// \returns TRUE on success, FALSE if the format was wrong.
		////////////////////////////////////////////////////////////////////////////////
		bool	start(const std::string &value, unsigned int ticksInSecond);
		bool	ended();
		
		enum Status {
			Working,
			Ended,
		};
		
		TimeCountdown();
		TimeCountdown(const std::string &value, unsigned int ticksInSecond);
		~TimeCountdown();
	};
};