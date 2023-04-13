#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <cstdint>

#include "model/command.hpp"
#include "model/model.hpp"

// Calculations
const size_t motors_model_vars_count {MOTORS_COUNT * 5};
const size_t motors_on_buffer_size {2};

void upload_motors_model(BLECharacteristic* motors_characteristic);
void load_motors_model(BLECharacteristic* motors_characteristic);
void upload_motors_on(BLECharacteristic* characteristic);
void load_motors_on(BLECharacteristic* characteristic);