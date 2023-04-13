#pragma once

#define BLE_NAME                       "RDB-1"
#define SERVICE_UUID                   "e7112e6c-c396-11ed-afa1-0242ac120002"
#define CHARACTERISTICS_UUID_BEGIN      0xffffff00

#ifndef BLE_PINCODE
	#define BLE_PINCODE 228228
#endif

#include <Arduino.h>
#include <vector>
#include <freertos/semphr.h>

// Bluetooth
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <esp_ota_ops.h>

#include "templates/controller.hpp"
#include "model/command.hpp"
#include "callbacks.hpp"

class BLEMonitorController: public Controller
{
public:
	BLEMonitorController() {};

	bool init();
	void loop();	// Just infinite loop
private:
	BLEServer* server = NULL;
	BLEService* service = NULL;
	BLECharacteristic* motors_on_characterestic = NULL;
	BLECharacteristic* motors_current_states_characteristic = NULL;
	BLECharacteristic* motors_target_states_characteristic = NULL;

	void update_model(SemaphoreHandle_t model_changed);
};