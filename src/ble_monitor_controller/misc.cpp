#include "misc.hpp"

// Спасибо Bing New за ускорении разработки

void convert_float_array_to_uint8_array(float* input, uint8_t* output, size_t size) 
{
	// Предполагаем, что размер output равен 4 * size
	for (size_t i = 0; i < size; i++) {
		// Преобразуем указатель на float в указатель на uint8_t
		uint8_t* bytes = reinterpret_cast<uint8_t*>(&input[i]);
		// Копируем 4 байта из bytes в output
		memcpy(&output[4 * i], bytes, 4);
	}
}

void convert_uint8_array_to_float_array(uint8_t* input, float* output, size_t size) 
{
	// Предполагаем, что размер input равен 4 * size
	for (size_t i = 0; i < size; i++) {
		// Преобразуем указатель на uint8_t в указатель на float
		float* value = reinterpret_cast<float*>(&input[4 * i]);
		// Копируем значение из value в output
		output[i] = *value;
	}
}

void convert_bool_array_to_uint8_array(bool* boolArray, size_t boolArraySize, uint8_t* uint8_tArray, size_t uint8_tArraySize) {
	uint8_t uint8_tValue = 0;
	int bitPosition = 0;
	for (size_t i = 0; i < boolArraySize; i++) {
		uint8_tValue |= (boolArray[i] << bitPosition);
		bitPosition++;
		if (bitPosition == 8) {
			if (i / 8 < uint8_tArraySize) {
				uint8_tArray[i / 8] = uint8_tValue;
			}
			uint8_tValue = 0;
			bitPosition = 0;
		}
	}

	if (bitPosition > 0 && boolArraySize / 8 < uint8_tArraySize) {
		uint8_tArray[boolArraySize / 8] = uint8_tValue;
	}
}

void convert_uint8_array_to_bool_array(uint8_t *uint8_tArray, size_t uint8_tArraySize, bool *boolArray, size_t boolArraySize)
{
	uint8_t uint8_tValue;
	int bitPosition = 0;
	for (size_t i = 0; i < boolArraySize; i++)
	{
		if (i / 8 < uint8_tArraySize)
		{
			uint8_tValue = uint8_tArray[i / 8];
			boolArray[i] = (uint8_tValue >> bitPosition) & 1;
		}
		else
		{
			boolArray[i] = false;
		}
		bitPosition++;

		if (bitPosition == 8)
		{
			bitPosition = 0;
		}
	}
}

void upload_motors_model(BLECharacteristic* characteristic) 
{
	float motors_data[motors_model_vars_count];
	for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
		for(uint8_t i = 0; i < 5; i++) {
			switch (i)
			{
			case 0:
				motors_data[(m-1)*5 + i] = Model::motors[m].c_pos;
				break;
			case 1:
				motors_data[(m-1)*5 + i] = Model::motors[m].kp;
				break;
			case 2:
				motors_data[(m-1)*5 + i] = Model::motors[m].c_vel;
				break;
			case 3:
				motors_data[(m-1)*5 + i] = Model::motors[m].kd;
				break;
			case 4:
				motors_data[(m-1)*5 + i] = Model::motors[m].c_trq;
				break;
			default:
				break;
			}
		}
	}

	uint8_t buffer[motors_model_vars_count*4];
	convert_float_array_to_uint8_array(motors_data, buffer, motors_model_vars_count);
	
	characteristic->setValue(buffer, sizeof(buffer));

	return;
}

void load_motors_model(BLECharacteristic* characteristic)
{
	float motors_data[motors_model_vars_count];

	convert_uint8_array_to_float_array(characteristic->getData(), motors_data, motors_model_vars_count);

	for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
		for(uint8_t i = 0; i < 5; i++) {
			switch (i)
			{
			case 0:
				Model::motors[m].t_pos = motors_data[(m-1)*5 + i];
				break;
			case 1:
				Model::motors[m].kp = motors_data[(m-1)*5 + i];
				break;
			case 2:
				Model::motors[m].t_vel = motors_data[(m-1)*5 + i];
				break;
			case 3:
				Model::motors[m].kd = motors_data[(m-1)*5 + i];
				break;
			case 4:
				Model::motors[m].t_trq = motors_data[(m-1)*5 + i];
				break;
			default:
				break;
			}
		}

		Model::need_update[m] = true; // need optimize
	}

	return;
}

void upload_motors_on(BLECharacteristic *characteristic)
{
	bool motorsOnStates[MOTORS_COUNT];

	for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
		motorsOnStates[m-1] = Model::motors[m].turn_on;
	}

	uint8_t buffer[motors_on_buffer_size];
	convert_bool_array_to_uint8_array(motorsOnStates, MOTORS_COUNT, buffer, motors_on_buffer_size);

	characteristic->setValue(buffer, motors_on_buffer_size);

	return;
}

void load_motors_on(BLECharacteristic *characteristic)
{
	bool motorsOnStates[MOTORS_COUNT];

	convert_uint8_array_to_bool_array(characteristic->getData(), 2, motorsOnStates, MOTORS_COUNT);
	
	for(uint8_t m = 1; m <= MOTORS_COUNT; m++) {
		if(motorsOnStates[m-1] && !Model::motors[m].turn_on) {
			Model::push_command(Command{ MOTOR_ON, m});
		} else if (!motorsOnStates[m-1] && Model::motors[m].turn_on) {
			Model::push_command(Command{ MOTOR_OFF, m});
		}
	}
	
	return;
}