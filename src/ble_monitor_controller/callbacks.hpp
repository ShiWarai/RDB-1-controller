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

    void onConnect(BLEServer *server) override
    {
        #if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
        Serial.println("Connect");
        #endif

        return;
    };

    void onDisconnect(BLEServer *server) override
    {
        this->advertising->start();

        #if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
        Serial.println("Disconnect");
        #endif

        return;
    }
};

class BLEMotorOnCharacteristicCallbacks : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *characteristic) override
    {
        upload_motors_on(characteristic);

        return;
    }

    void onWrite(BLECharacteristic *characteristic) override
    {

        for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
            if(!Model::motors[m].set_origin) {
                if(Model::motors[m].turn_on) {
                    #if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
                    Serial.println("You should turn off drivers!");
                    #endif
                    
                    return;
                }

                Model::push_command(Command{ SET_ORIGIN, m, 0 });
            }
        }

        load_motors_on(characteristic);

        xSemaphoreGive(model_changed);
        vTaskDelay(32);
        xSemaphoreTake(model_changed, portMAX_DELAY);

        return;
    }
};

class BLEReadMotorsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *characteristic) override
    {

        for(uint8_t m = 1; m <= MOTORS_COUNT; m++)
            Model::push_command(Command{CHECK, m, 0});

        xSemaphoreGive(model_changed);
        vTaskDelay(32);
        xSemaphoreTake(model_changed, portMAX_DELAY);

        upload_motors_model(characteristic);

        return;
    }
};

class BLEWriteMotorsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *characteristic) override
    {

        load_motors_model(characteristic);

        xSemaphoreGive(model_changed);
        vTaskDelay(32);
        xSemaphoreTake(model_changed, portMAX_DELAY);

        upload_motors_model(characteristic);

        return;
    }
};