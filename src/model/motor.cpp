#include "model/motor.hpp"
#include "model/model.hpp"

void Motor::set_position_by_procent(float proc) {
	t_pos = min_pos + proc * abs(max_pos - min_pos);
	Model::need_update[id] = true;
}

String Motor::get_current_stats() {
	return String("Motor #") + String(id) + String(" current stats\r\npos: ") + String(c_pos) + String(", vel: ") + String(c_vel) + String(", trq: ") + String(c_trq) + String(";");
}

String Motor::get_target_stats()
{
	return String("Motor #") + String(id) + String(" target stats\r\npos: ") + String(t_pos) + String(", kp: ") + String(kp) + String(", vel: ") + String(t_vel) + String(", kd: ") + String(kd) + String(", trq: ") + String(t_trq) + String(";");
}

String Motor::get_full_stats()
{
	return String("Motor #") + String(id) + String(" current stats\r\npos: ") + 
		String(c_pos) + String(", vel: ") + String(c_vel) + String(", trq: ") + String(c_trq) + String(";\r\n") + 
		String("and target stats\r\npos: ") + String(t_pos) + String(", kp: ") + String(kp) + String(", vel: ") + String(t_vel) + String(", kd: ") + String(kd) + String(", trq: ") + String(t_trq) + String(";");
}