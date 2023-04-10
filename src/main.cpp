#include "main.hpp"

SemaphoreHandle_t model_changed = xSemaphoreCreateBinary();

void setup()
{
	#ifdef SERIAL_OUTPUT
		Serial.begin(SERIAL_BAUDRATE);
	#endif

	Model::init();

	#if defined SERIAL_OUTPUT && defined SERIAL_INPUT
		xTaskCreate(controller_task<InputController>, "Input controller", 1024, NULL, 1, NULL);
	#endif

	#if BT_CONTROL_TYPE == 0
		xTaskCreate(controller_task<JoystickController>, "Joystick controller", 10240, NULL, 1, NULL);
	#else
		xTaskCreate(controller_task<RemoteDebug>, "Remote debug", 4096, NULL, 1, NULL);
	#endif

	xTaskCreate(controller_task<MotorController>, "Motor controller", 4096, NULL, 1, NULL);
}

void loop()
{
	vTaskDelete(NULL);
}
