#include "joystick_controller/joystick_controller.hpp"

bool JoystickController::init()
{
	clear_paired_devices();

	PS4.begin(MAC_PS4_JOYSTICK);
	PS4.setLed(255, 255, 0);

	return true;
}

void JoystickController::loop()
{
	extern SemaphoreHandle_t model_changed;

	float pos1, pos2, pos3;
	float p_pos1, p_pos2, p_pos3;
	float n_pos1, n_pos2, n_pos3;

	int movement_tick = 0;
	const int MAX_MOVENENT_TICKS = 4;
	const float MOVEMENT_STATES_POS[MAX_MOVENENT_TICKS][3] = {{0.7, 0.7, 0.8}, {0.7, 0.85, 0.6}, {0.7, 1.0, 0.4}, {0.7, 0.85, 0.6}};

	ButtonWithState motorSwitch;
	bool motorOnLast = false;
	ButtonWithState sharePosesButton;
	ButtonWithState legsMovingButton;
	bool legsMovingLast = false;
	ClickableButton setOriginButton;
	ClickableButton moveToOriginButton;

	Serial.println("🔁 Joystick begin");
	while (1) {
		if (PS4.isConnected()) {
			pos1 = PS4.LStickX();
			pos2 = PS4.LStickY();
			pos3 = PS4.RStickY();


			if (motorSwitch.turn(PS4.PSButton())) {
				if (motorSwitch.state() != motorOnLast)
				{
					PS4.setLed(0, 128, 0);
					for (short i = 1; i <= MOTORS_COUNT; i++)
						Model::push_command(Command{ MOTOR_ON, i});

					update_model(model_changed);

					motorOnLast = true;
				}
			}
			else {
				if (motorSwitch.state() != motorOnLast)
				{
					PS4.setLed(255, 0, 0);
					for (short i = 1; i <= MOTORS_COUNT; i++)
						Model::push_command(Command{ MOTOR_OFF, i});

					update_model(model_changed);

					motorOnLast = false;
					legsMovingLast = false;
				}
			}

			if (moveToOriginButton.turn(PS4.Circle())) {
				for (short i = 1; i <= MOTORS_COUNT; i++)
					Model::push_command(Command{ MOTOR_NONE, i});

				update_model(model_changed);
			}
			
			if (setOriginButton.turn(PS4.Options())) {
				for (short i = 1; i <= MOTORS_COUNT; i++)
					Model::push_command(Command{ SET_ORIGIN, i});

				update_model(model_changed);
				PS4.setLed(255, 0, 0);
			}

			if (legsMovingButton.turn(PS4.Cross())) {
				legsMovingLast = true;

				Model::motors[1].set_position_by_procent(MOVEMENT_STATES_POS[movement_tick][1-1]);
				Model::motors[2].set_position_by_procent(MOVEMENT_STATES_POS[movement_tick][2-1]);
				Model::motors[3].set_position_by_procent(MOVEMENT_STATES_POS[movement_tick][3-1]);

				Model::motors[4].set_position_by_procent(1 - MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][1-1]);
				Model::motors[5].set_position_by_procent(1 - MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][2-1]);
				Model::motors[6].set_position_by_procent(1 - MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][3-1]);

				Model::motors[7].set_position_by_procent(1 - MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][1-1]);
				Model::motors[8].set_position_by_procent(MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][2-1]);
				Model::motors[9].set_position_by_procent(MOVEMENT_STATES_POS[(movement_tick + (MAX_MOVENENT_TICKS / 2)) % MAX_MOVENENT_TICKS][3-1]);

				Model::motors[10].set_position_by_procent(MOVEMENT_STATES_POS[movement_tick][1-1]);
				Model::motors[11].set_position_by_procent(1 - MOVEMENT_STATES_POS[movement_tick][2-1]);
				Model::motors[12].set_position_by_procent(1 - MOVEMENT_STATES_POS[movement_tick][3-1]);

				movement_tick++;

				if(movement_tick >= MAX_MOVENENT_TICKS) {
					movement_tick = 0;
				}

				xSemaphoreGive(model_changed);
				vTaskDelay(500);
				xSemaphoreTake(model_changed, portMAX_DELAY);

				PS4.setRumble(20, 0);
				PS4.setLed(124, 0, 255);
			}
			else {
				if (legsMovingLast) {

					movement_tick = 0;
					legsMovingLast = false;

					PS4.setRumble(0, 0);

					if (motorOnLast)
						PS4.setLed(0, 128, 0);
					else
						PS4.setLed(255, 0, 0);
				}
			}

			if (sharePosesButton.turn(PS4.Share())) {

				p_pos1 = float(128 + pos1) / 256;
				p_pos2 = float(128 + pos2) / 256;
				p_pos3 = float(128 + pos3) / 256;

				n_pos1 = float(128 + -pos1) / 256;
				n_pos2 = float(128 + -pos2) / 256;
				n_pos3 = float(128 + -pos3) / 256;

				Model::motors[1].set_position_by_procent(n_pos1);
				Model::motors[2].set_position_by_procent(n_pos2);
				Model::motors[3].set_position_by_procent(n_pos3);

				Model::motors[4].set_position_by_procent(p_pos1);
				Model::motors[5].set_position_by_procent(n_pos2);
				Model::motors[6].set_position_by_procent(n_pos3);

				Model::motors[7].set_position_by_procent(p_pos1);
				Model::motors[8].set_position_by_procent(p_pos2);
				Model::motors[9].set_position_by_procent(p_pos3);

				Model::motors[10].set_position_by_procent(n_pos1);
				Model::motors[11].set_position_by_procent(p_pos2);
				Model::motors[12].set_position_by_procent(p_pos3);

				PS4.setRumble(20, 0);

				Model::update_model();
			} else {
				PS4.setRumble(0, 0);
			}

			PS4.sendToController(); // !!! Replace !!!
		}

		vTaskDelay(32);
	}
}