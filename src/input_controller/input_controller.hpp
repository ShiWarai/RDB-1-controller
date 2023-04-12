#pragma once

#include <vector>
#include <Arduino.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "../model/command.hpp"
#include "../model/model.hpp"

class InputController
{
public:
	InputController() {};

	bool init();
	void loop();
};