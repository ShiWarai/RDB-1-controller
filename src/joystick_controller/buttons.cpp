#include "joystick_controller/buttons.hpp"

bool ClickableButton::state() {
	return __state;
}

bool ClickableButton::press() {
	if (!__buttonPressed) {
		__state = true;
		__buttonPressed = true;
	}
	else {
		__state = false;
	}

	return __state;
}

bool ClickableButton::release() {
	__state = false;
	__buttonPressed = false;

	return false;
}

bool ClickableButton::turn(bool direction) {
	return direction ? this->press() : this->release();
}

ButtonWithState::ButtonWithState(bool state) {
	this->__state = state;
}

bool ButtonWithState::state() {
	return __state;
}

bool ButtonWithState::turn(bool direction) {
	if (direction) {
		if (!__buttonPressed)
		{
			__state = !__state;
		}
		__buttonPressed = true;
	}
	else {
		__buttonPressed = false;
	}

	return __state;
}