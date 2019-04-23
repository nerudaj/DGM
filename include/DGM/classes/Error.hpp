#pragma once

#include <stdexcept>

namespace dgm {
	class GeneralException : public std::runtime_error {
	public:
		GeneralException(const std::string &message) : std::runtime_error(message) {}
	};
}