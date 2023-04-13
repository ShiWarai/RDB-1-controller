#pragma once

#include <queue>
#include <Arduino.h>
#include <freertos/semphr.h>

#include "motor.hpp"
#include "command.hpp"

const unsigned char MOTORS_COUNT {12};

class Model {
public:
	inline static bool need_update[MOTORS_COUNT + 1];
	inline static Motor motors[MOTORS_COUNT + 1]; // 0 is empty
	inline static std::queue<Command> commands;

	static bool init();
	static void push_command(Command command);
	static void update_model();

	Model() = delete;
};