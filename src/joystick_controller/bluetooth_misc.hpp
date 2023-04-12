#pragma once

#define PAIR_MAX_DEVICES 20

#include <Arduino.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
#include <esp_err.h>

void clear_paired_devices();
char* bda_to_str(const uint8_t* bda, char* str, size_t size);
bool init_bluetooth();
bool disable_bluetooth();