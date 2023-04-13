#pragma once

#include <vector>
#include <Arduino.h>
#include <mcp2515_can.h>
#include <freertos/semphr.h>

#include "templates/controller.hpp"
#include "commands.hpp"
#include "limits.hpp"
#include "model/command.hpp"

class MotorController : public Controller
{
public:
	MotorController() {};

	bool init();
	void loop();
private:
	static const size_t CAN_BUSES_COUNT {1};
	static mcp2515_can can_buses[CAN_BUSES_COUNT];

	// Delays
	static const uint8_t DELAY {8};
	static const uint16_t SET_ORIGIN_DELAY {1500};
	static const uint8_t CHECK_DELAY {64};

	void start_motor(Motor &motor);
	void stop_motor(Motor &motor);
	void zero_motor(Motor &motor);
	void check_motor(Motor &motor);
	void control_motor(Motor &motor);
	void __control_motor(Motor &motor);

	static unsigned int float_to_uint(float x, float x_min, float x_max, float bits);
	static float uint_to_float(unsigned int x_int, float x_min, float x_max, float bits);

	static byte can_pack(mcp2515_can *can, unsigned long id, float t_pos = 0, float t_kp = 0, float t_vel = 0, float t_kd = 0, float t_trq = 0);
	static byte can_unpack(mcp2515_can *can, unsigned long id, float *c_pos, float *c_vel, float *c_trq, unsigned long *m_id = nullptr);
};
