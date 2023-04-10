#include <Arduino.h>
#include <mcp2515_can.h>
#include "freertos/semphr.h"

#include "input_controller/input_controller.hpp"
#include "joystick_controller/joystick_controller.hpp"
#include "motor_controller/motor_controller.hpp"
#include "remote_debug/remote_debug.hpp"
#include "templates/controller_task.hpp"

SemaphoreHandle_t model_changed = xSemaphoreCreateBinary();

void setup()
{
	#ifdef SERIAL_OUTPUT
		Serial.begin(115200);
	#endif

	Model::init();

	#if defined SERIAL_OUTPUT && defined SERIAL_INPUT
		xTaskCreate(controller_task<InputController>, "Input controller", 1024, NULL, 1, NULL);
		delay(5);
	#endif

	#if BT_CONTROL_TYPE == 0
		xTaskCreate(controller_task<JoystickController>, "Joystick controller", 10240, NULL, 1, NULL);
	#else
		xTaskCreate(controller_task<RemoteDebug>, "Remote debug", 4096, NULL, 1, NULL);
	#endif
	delay(5);

	xTaskCreate(controller_task<MotorController>, "Motor controller", 4096, NULL, 1, NULL);
	delay(5);
}

void loop()
{
	vTaskDelete(NULL);
}
