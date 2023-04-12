#pragma once

enum CommandType
{
	MOTOR_NONE,
	MOTOR_OFF,
	MOTOR_ON,
	SET_ORIGIN,
	CHECK,
	#if BT_CONTROL_TYPE == 0 || defined(SERIAL_INPUT)
	RELATIVE_CONTROL,
	#endif
};

struct Command
{
	CommandType type;
	short id;
	#if BT_CONTROL_TYPE == 0 || defined(SERIAL_INPUT)
	float value; // from 0.0 to 1.0
	#endif
};
