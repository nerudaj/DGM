/**
 *  \file 			Strings.hpp
 *  \author			doomista
 *  \description	std::string manipulation helper methods
 */

#pragma once

namespace dgm {
	class Strings {
	public:
		/**
		 *  \brief Splits the string src by delimiter and stores resulting array of strings to dst
		 *  
		 *  \details dst is always cleared at the start of the function. Multiple delimiters one
		 *  after each other are treated as one.
		 */
		static void split(const char delimiter, const std::string &src, std::vector<std::string> &dst);
		
		/**
		 *  \brief Replaces all occurences of from with to in str
		 */
		static void replaceAll(std::string &str, const std::string &from, const std::string &in);
	};
}