#pragma once

#define MAC_PS4_JOYSTICK "4A:30:10:19:10:1A"

#include <vector>
#include <Arduino.h>
#include <PS4Controller.h>

#include "../model/command.hpp"
#include "../model/model.hpp"
#include "buttons.hpp"
#include "bluetooth_misc.hpp"

class JoystickController
{
public:
	JoystickController() {};
		
	bool init();
	void loop();
private:
	// Misc
	void update_model(SemaphoreHandle_t model_changed);
};