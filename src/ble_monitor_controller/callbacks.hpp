#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <esp_ota_ops.h>

#include "misc.hpp"

extern SemaphoreHandle_t model_changed;

class BLECustomServerCallbacks : public BLEServerCallbacks
{
public:
	BLECustomServerCallbacks(BLEAdvertising *adv) : advertising {adv} {}
private:
	BLEAdvertising *advertising;

	void onConnect(BLEServer *server) override;
	void onDisconnect(BLEServer *server) override;
};

class BLEMotorOnCharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onRead(BLECharacteristic *characteristic) override;
	void onWrite(BLECharacteristic *characteristic) override;
};

class BLEReadMotorsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onRead(BLECharacteristic *characteristic) override;
};

class BLEWriteMotorsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onWrite(BLECharacteristic *characteristic) override;
};