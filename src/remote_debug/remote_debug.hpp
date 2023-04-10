#pragma once

#include <Arduino.h>
#include <vector>
#include <freertos/semphr.h>

// Bluetooth
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <esp_ota_ops.h>

#include "../model/command.hpp"
#include "../model/model.hpp"
#include "callbacks.hpp"

class RemoteDebug
{
public:
	RemoteDebug() {};

	bool init();
	void loop();
private:
	BLEServer* pServer = NULL;
	BLEService* pService = NULL;
	BLECharacteristic* pMotorsOnCharacterestic = NULL;
	BLECharacteristic* pMotorsCurrentCharacteristic = NULL;
	BLECharacteristic* pMotorsTargetCharacteristic = NULL;
};