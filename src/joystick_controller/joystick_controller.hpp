#pragma once

#define MAC_PS4_JOYSTICK "4A:30:10:19:10:1A"
#define PAIR_MAX_DEVICES 20

#include <vector>
#include <Arduino.h>
#include <PS4Controller.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
#include <esp_err.h>

#include "../model/command.hpp"
#include "../model/model.hpp"
#include "buttons.hpp"

class JoystickController
{
public:
    JoystickController() {};
        
    bool init();
    void loop();
private:
    // Misc
    void update_model(SemaphoreHandle_t model_changed);

    // Bluetooth
    void clear_paired_devices();
    char* bda_to_str(const uint8_t* bda, char* str, size_t size);
    bool init_bluetooth();
    bool disable_bluetooth();
};