#include "input_controller.hpp"

bool InputController::init()
{
	return true;
}

void InputController::loop()
{
	extern SemaphoreHandle_t model_changed;

	const int msg_len {32};	// Max message length
	char symb;	// Char buffer-var
	char buffer[msg_len] {0};	// Collect chars into message
	uint8_t symb_pos {0};	// Current message chars position

	short id;
	unsigned short pos;

	Serial.println("🔁 Serial input begin");
	while (1)
	{
		while (Serial.available())
		{
			symb = Serial.read();
			if ((symb != '\r') && (symb_pos < msg_len - 1))
			{
				buffer[symb_pos] = symb;
				symb_pos++;
				continue;
			}

			switch (buffer[0])
			{
				case 'a':	// Start all motors
					if (symb_pos > 1)
					{
						id = (buffer[1] - 48) * 10 + (buffer[2] - 48);

						if (id <= 0 || id > MOTORS_COUNT)
						{
							Serial.println("Wrong id!");
							break;
						}
						Model::push_command(Command{ MOTOR_ON, id});
					} else {
						for (short i = 1; i <= MOTORS_COUNT; i++)
							Model::push_command(Command{ MOTOR_ON, i});
					}

					break;

				case 'b': // Stop all motors
					if (symb_pos > 1)
					{
						id = (buffer[1] - 48) * 10 + (buffer[2] - 48);

						if (id <= 0 || id > MOTORS_COUNT)
						{
							Serial.println("Wrong id!");
							break;
						}

						Model::push_command(Command{ MOTOR_OFF, id});
					}
					else {
						for (short i = 1; i <= MOTORS_COUNT; i++)
							Model::push_command(Command{ MOTOR_OFF, i});
					}

					break;

				case 'c': // Check a motor
					id = (buffer[1] - 48) * 10 + (buffer[2] - 48);

					if (id <= 0 || id > MOTORS_COUNT)
					{
						Serial.println("Wrong id!");
						break;
					}
					
					Model::push_command(Command{CHECK, id});

					break;

				case 'f': // Zero all motors
					if (symb_pos > 1)
					{
						id = (buffer[1] - 48) * 10 + (buffer[2] - 48);

						if (id <= 0 || id > MOTORS_COUNT)
						{
							Serial.println("Wrong id!");
							break;
						}

						Model::push_command(Command{ SET_ORIGIN, id});
					}
					else {
						for (short i = 1; i <= MOTORS_COUNT; i++)
							Model::push_command(Command{ SET_ORIGIN, i});
					}

					break;

				case 'm': // Control motor
					id = (buffer[1] - 48) * 10 + (buffer[2] - 48);
					pos = (buffer[4] - 48) * 100 + (buffer[5] - 48) * 10 + (buffer[6] - 48);

					if (id <= 0 || id >= 100)
					{
						Serial.println("Wrong id!");
						break;
					}

					if (!(pos >= 0 && pos <= 100)) {
						Serial.println("Wrong position!");
						break;
					}

					#if defined SERIAL_OUTPUT && defined SERIAL_INPUT // Cringe
					Model::push_command(Command{ RELATIVE_CONTROL, id, float(pos) / 100 });
					#endif

					break;
			}

			// Reset the message
			memset(buffer, 0, sizeof(buffer));
			symb_pos = 0;

			xSemaphoreGive(model_changed);
			vTaskDelay(128);
			xSemaphoreTake(model_changed, portMAX_DELAY);
		}

		vTaskDelay(1);
	}
}