#pragma once

enum CommandType
{
	MOTOR_NONE,
	MOTOR_OFF,
	MOTOR_ON,
	SET_ORIGIN,
	CHECK,
	RELATIVE_CONTROL,
};

struct Command
{
	CommandType type;
	short id;
	float value; // 0-100% (0.0 - 1.0)
};
