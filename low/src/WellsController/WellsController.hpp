#ifndef WELLS_CONTROLLER_HPP
#define WELLS_CONTROLLER_HPP

#include <Arduino.h>

#include "../MessageQueue/MessageQueue.hpp"
#include "../CurrentObserver/CurrentObserver.hpp"
#include "../TankLevelObserver/TankLevelObserver.hpp"

#include "BaseWellController/BaseWellController.hpp"
#include "SubWellController/SubWellController.hpp"
#include "JetWellController/JetWellController.hpp"

class WellsController {
	public:
		WellsController(
			MessageQueue *messageQueue,
			CurrentObserver *currentObserver,
			TankLevelObserver *tankLevelObserver
		);

		void update();

		String setActiveWellCmd(short cmdWell, short minRiseRate, float minCurrent);
		String getActiveWellCmd();

	private:
		const short NO_ACTIVE_WELLS = -1;

		const short WELLS_COUNT = 3;
		const bool WELLS_IS_JET[3] = {false, false, true};
		const short WELLS_PUMP_PINS[3] = {6, 7, 8};
		// Only jet pumps need solenoid pins
		const short WELLS_VALVE_PINS[3] = {0, 0, 9};

		const short FULL_LEVEL = 32;

		MessageQueue *messageQueue;
		CurrentObserver *currentObserver;
		TankLevelObserver *tankLevelObserver;

		BaseWellController *wellControllers[3];

		short getActiveWell();
};

#endif
