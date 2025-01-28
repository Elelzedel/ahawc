#include "WellsController.hpp"

WellsController::WellsController(
	MessageQueue *messageQueue,
	CurrentObserver *currentObserver,
	TankLevelObserver *tankLevelObserver
) {
	this -> messageQueue = messageQueue;
	this -> tankLevelObserver = tankLevelObserver;
	this -> currentObserver = currentObserver;

	for(int i = 0; i < WELLS_COUNT; i++) {
		if (WELLS_IS_JET[i]) {
			this -> wellControllers[i] = new JetWellController(
				i,
				messageQueue,
				currentObserver,
				tankLevelObserver,
				WELLS_PUMP_PINS[i],
				WELLS_VALVE_PINS[i]
			);
		} else {
			this -> wellControllers[i] = new SubWellController(
				i,
				messageQueue,
				currentObserver,
				tankLevelObserver,
				WELLS_PUMP_PINS[i]
			);
		}
	}
}

void WellsController::update() {
	short activeWell = getActiveWell();

	if (activeWell == NO_ACTIVE_WELLS) {
		return;
	}

	if (this -> tankLevelObserver -> getLevel() >= FULL_LEVEL) {
		String message = "WC STOPPING "
					   + String(activeWell)
					   + " TANK FULL";
		
		this -> wellControllers[activeWell] -> stopWell();
		this -> messageQueue -> put(message.c_str());
		return;
	}


	if (this -> tankLevelObserver -> getLevel() == TankLevelObserver::LEVEL_TIMEOUT) {
		String message = "WC STOPPING "
					   + String(activeWell)
					   + " LEVEL OBSERVER TIMEOUT";

		this -> wellControllers[activeWell] -> stopWell();
		this -> messageQueue -> put(message.c_str());
		return;
	}

	this -> wellControllers[activeWell] -> update();
}

String WellsController::setActiveWellCmd(short cmdWell, short minRiseRate, float cutoffCurrent) {
	short activeWell = getActiveWell();

	// Bypass other checks and immediately stop the well if NO_ACTIVE_WELLS is submitted
	if (cmdWell == NO_ACTIVE_WELLS) {
		this -> wellControllers[activeWell] -> stopWell();
		return "ACK WC SET ACTIVE -1";
	}

	if (cmdWell < 0 || cmdWell >= WELLS_COUNT) {
		return "FAIL WC SET ACTIVE BAD WELL";
	}

	if (activeWell != NO_ACTIVE_WELLS) {
		return "FAIL WC SET ACTIVE WELL ALREADY RUNNING";
	}

	if (cutoffCurrent < 0.0) {
		return "FAIL WC SET ACTIVE BAD CURRENT";
	}

	return this -> wellControllers[cmdWell] -> startWellCmd(minRiseRate, cutoffCurrent);
}

String WellsController::getActiveWellCmd() {
	return "ACK WC GET ACTIVE " + String(this -> getActiveWell());
}

short WellsController::getActiveWell() {
	for (short i = 0; i < WELLS_COUNT; i++) {
		if (this -> wellControllers[i] -> getIsActive()) {
			return i;
		}
	}

	return NO_ACTIVE_WELLS;
}
