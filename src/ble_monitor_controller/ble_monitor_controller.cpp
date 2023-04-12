#include "ble_monitor_controller/ble_monitor_controller.hpp"

void BLEMonitorController::loop()
{
	Serial.println("🔁 Remote control begin");
	
	while(1) {
		vTaskDelay(1);
	}
}

bool BLEMonitorController::init() {

	// Create the BLE Device
	BLEDevice::init(BLE_NAME);
	BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);

	// Create the BLE Server
	server = BLEDevice::createServer();
	BLECustomServerCallbacks* callbacks = new BLECustomServerCallbacks(server->getAdvertising());
	server->setCallbacks(callbacks);

	// Create the BLE Service
	service = server->createService(BLEUUID(SERVICE_UUID), 1+3*2);

	// Security
	BLESecurity *security = new BLESecurity();
	uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
	uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;

	uint32_t* passkey {new uint32_t(BLE_PINCODE)};
	esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, passkey, sizeof(uint32_t));
	security->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
	security->setCapability(ESP_IO_CAP_OUT);
	security->setKeySize(16);
	esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
	security->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
	esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
	
	// Create characteristics
	motors_on_characterestic = service->createCharacteristic(
								CHARACTERISTICS_UUID_BEGIN,
								BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
							);
	motors_on_characterestic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	motors_on_characterestic->setCallbacks(new BLEMotorOnCharacteristicCallbacks());

	motors_current_states_characteristic = service->createCharacteristic(
								CHARACTERISTICS_UUID_BEGIN + 1,
								BLECharacteristic::PROPERTY_READ
							);
	motors_current_states_characteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	motors_current_states_characteristic->setCallbacks(new BLEReadMotorsCharacteristicCallbacks());
	upload_motors_model(motors_current_states_characteristic);

	motors_target_states_characteristic = service->createCharacteristic(
										CHARACTERISTICS_UUID_BEGIN + 2,
										BLECharacteristic::PROPERTY_WRITE
									);
	motors_target_states_characteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	motors_target_states_characteristic->setCallbacks(new BLEWriteMotorsCharacteristicCallbacks());

	service->start();
	server->getAdvertising()->start();

	return true;
}