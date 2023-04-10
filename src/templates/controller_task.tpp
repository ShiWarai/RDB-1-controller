#pragma once

template<class ControllerClass>
void controller_task(void* pv) {
	ControllerClass controller_object;

	if(controller_object.init()) {
		controller_object.loop();
	}
}