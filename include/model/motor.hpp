#pragma once

#include <cmath>
#include <Arduino.h>

static short motor_counter {0};

class Motor
{
public:
	short id;
	short can_id {-1};

	bool turn_on = false;
	bool origined = false;

	float t_pos {0};
	float t_vel {0};
	float t_trq {0};
	
	float kp {0};
	float kd {0};

	float c_pos;
	float c_vel;
	float c_trq;

	float min_pos;
	float max_pos;

	Motor(short _id, short _can_id) : id(_id), can_id(_can_id) {}
	Motor(short _can_id) : can_id(_can_id) { id = ++motor_counter; }
	Motor() {}

	void set_position_by_procent(float proc);
	
	String get_current_stats();
	String get_target_stats();
	String get_full_stats();
};