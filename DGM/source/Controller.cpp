#include <DGM\dgm.hpp>

class dgm::Binding {
public:
	bool released;
	sf::Keyboard::Key key;
	dgm::X360::Input joy;
};

inline bool isJoystickAxis(dgm::X360::Input joy) {
	return joy >= 100; // 100 is where axii start
}

float dgm::Controller::getJoystickAxis(const dgm::X360::Input joy) const {
	float rtn;

	switch (joy) {
	case dgm::X360::LTrigger:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::Z);
		break;
	case dgm::X360::RTrigger:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::Z);
		break;
	case dgm::X360::LStick_Up:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::Y);
		break;
	case dgm::X360::LStick_Down:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::Y);
		break;
	case dgm::X360::LStick_Left:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::X);
		break;
	case dgm::X360::LStick_Right:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::X);
		break;
	case dgm::X360::RStick_Up:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::R);
		break;
	case dgm::X360::RStick_Down:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::R);
		break;
	case dgm::X360::RStick_Left:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::U);
		break;
	case dgm::X360::RStick_Right:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::U);
		break;
	case dgm::X360::POV_Up:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::PovY);
		break;
	case dgm::X360::POV_Down:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::PovY);
		break;
	case dgm::X360::POV_Left:
		rtn = -sf::Joystick::getAxisPosition(index, sf::Joystick::PovX);
		break;
	case dgm::X360::POV_Right:
		rtn = sf::Joystick::getAxisPosition(index, sf::Joystick::PovX);
		break;
	default:
		rtn = 0.0f;
		break;
	}

	if (rtn < deadzone) {
		rtn = 0.0f;
	}

	return rtn;
}

bool dgm::Controller::keyPressed(const int code) {
	if (sf::Keyboard::isKeyPressed(bindings[code].key))
		return (not bindings[code].released);
	else if (sf::Joystick::isConnected(index) and bindings[code].joy != dgm::X360::Empty) {
		if (isJoystickAxis(bindings[code].joy) and (getJoystickAxis(bindings[code].joy) > 0.f))
			return (not bindings[code].released);
		else if (sf::Joystick::isButtonPressed(index, bindings[code].joy))
			return (not bindings[code].released);
	}

	return (bindings[code].released = false);
}

bool dgm::Controller::keyPressed(const int code, float & intensity) const {
	intensity = 0.f;
	if (sf::Keyboard::isKeyPressed(bindings[code].key))
		intensity = 100.f;
	else if (sf::Joystick::isConnected(index) and bindings[code].joy != dgm::X360::Empty) {
		if (isJoystickAxis(bindings[code].joy))
			intensity = getJoystickAxis(bindings[code].joy);
		else if (sf::Joystick::isButtonPressed(index, bindings[code].joy))
			intensity = 100.f;
	}

	return (intensity > 0.f);
}

void dgm::Controller::releaseKey(const int code) {
	bindings[code].released = true;
}

void dgm::Controller::setBinding(const int code, sf::Keyboard::Key key, dgm::X360::Input joy) {
	if (bindings.size() <= size_t(code)) {
		bindings.resize(code + 1);
	}

	bindings[code].released = false;
	bindings[code].key = key;
	bindings[code].joy = joy;
}

void dgm::Controller::setDeadzone(const float threshold) {
	deadzone = threshold;
}

void dgm::Controller::setIndex(const int index) {
	Controller::index = index;
}

dgm::Controller::Controller() {
	bindings.resize(16);
}

dgm::Controller::~Controller() {
}
