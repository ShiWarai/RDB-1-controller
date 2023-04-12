#include "bluetooth_misc.hpp"

bool init_bluetooth()
{
	if(!btStart())
		return false;

	if(esp_bluedroid_init() != ESP_OK)
		return false;

	if(esp_bluedroid_enable() != ESP_OK)
		return false;

	return true;
}

bool disable_bluetooth()
{
	if(esp_bluedroid_disable() != ESP_OK)
		return false;

	if(esp_bluedroid_deinit() != ESP_OK)
		return false;

	if(!btStop())
		return false;

	return true;
}

char* bda_to_str(const uint8_t* bda, char* str, size_t size)
{
  if (bda == NULL || str == NULL || size < 18) {
	return NULL;
  }
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
		  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
  return str;
}

void clear_paired_devices() {
	char bda_str[18];
	uint8_t pairedDeviceBtAddr[PAIR_MAX_DEVICES][6];

	init_bluetooth();

	int count = esp_bt_gap_get_bond_device_num(); // Get the numbers of bonded/paired devices in the BT module

	if(count) {
		if(PAIR_MAX_DEVICES < count)
			count = PAIR_MAX_DEVICES;

		esp_err_t tError =  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
		if(ESP_OK == tError) {
			for(int i = 0; i < count; i++) {
				esp_err_t tError = esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);

				#if defined SERIAL_OUTPUT && defined SERIAL_DEBUG
				if(ESP_OK == tError)
					Serial.print("Removed bonded device # "); 
				else
					Serial.print("Failed to remove bonded device # ");
				Serial.println(i);
				#endif
			}
		}        
	}

	disable_bluetooth();
}