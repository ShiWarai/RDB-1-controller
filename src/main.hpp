#pragma once

#define SERIAL_BAUDRATE 115200

#include <Arduino.h>
#include <mcp2515_can.h>
#include <freertos/semphr.h>

#include "input_controller/input_controller.hpp"
#include "joystick_controller/joystick_controller.hpp"
#include "motor_controller/motor_controller.hpp"
#include "ble_monitor_controller/ble_monitor_controller.hpp"
#include "templates/controller_task.tpp"

/// @brief Starts all tasks
void setup();

/// @brief Unused and disabled
void loop();