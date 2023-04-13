#pragma once

#define MAC_PS4_JOYSTICK "4A:30:10:19:10:1A"

#include <vector>
#include <Arduino.h>
#include <PS4Controller.h>

#include "templates/controller.hpp"
#include "model/command.hpp"
#include "buttons.hpp"
#include "bluetooth_misc.hpp"

class JoystickController : public Controller
{
public:
	JoystickController() {};
		
	bool init();
	void loop();
};