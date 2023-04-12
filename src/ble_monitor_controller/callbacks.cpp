#include "callbacks.hpp"

void BLECustomServerCallbacks::onConnect(BLEServer *server)
{
	#if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
	Serial.println("Connect");
	#endif

	return;
}

void BLECustomServerCallbacks::onDisconnect(BLEServer *server)
{
	advertising->start();

	#if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
	Serial.println("Disconnect");
	#endif

	return;
}

void BLEMotorOnCharacteristicCallbacks::onRead(BLECharacteristic *characteristic)
{
	upload_motors_on(characteristic);

	return;
}

void BLEMotorOnCharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
	for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
		if(!Model::motors[m].origined) {
			if(Model::motors[m].turn_on) {
				#if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
				Serial.println("You should turn off drivers!");
				#endif

				return;
			}

			Model::push_command(Command{ SET_ORIGIN, m});
		}
	}

	load_motors_on(characteristic);

	xSemaphoreGive(model_changed);
	vTaskDelay(32);
	xSemaphoreTake(model_changed, portMAX_DELAY);

	return;
}

void BLEReadMotorsCharacteristicCallbacks::onRead(BLECharacteristic *characteristic)
{
	for(uint8_t m = 1; m <= MOTORS_COUNT; m++)
		Model::push_command(Command{CHECK, m});

	xSemaphoreGive(model_changed);
	vTaskDelay(32);
	xSemaphoreTake(model_changed, portMAX_DELAY);

	upload_motors_model(characteristic);

	return;
}

void BLEWriteMotorsCharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
	load_motors_model(characteristic);

	xSemaphoreGive(model_changed);
	vTaskDelay(32);
	xSemaphoreTake(model_changed, portMAX_DELAY);

	upload_motors_model(characteristic);

	return;
}
