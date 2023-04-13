#pragma once

#include <vector>
#include <Arduino.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "templates/controller.hpp"
#include "model/command.hpp"

class InputController: public Controller
{
public:
	InputController() {};

	bool init();
	void loop();
private:
	void update_model(SemaphoreHandle_t model_changed);
};