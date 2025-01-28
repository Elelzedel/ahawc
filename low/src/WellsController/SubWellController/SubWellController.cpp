#include "SubWellController.hpp"

SubWellController::SubWellController(
	short id, 
	MessageQueue *messageQueue,
	CurrentObserver *currentObserver,
	TankLevelObserver *tankLevelObserver,
	short wellPumpPin
) {
	this -> id = id;

	this -> messageQueue = messageQueue;
	this -> currentObserver = currentObserver;
	this -> tankLevelObserver = tankLevelObserver;

	this -> wellPumpPin = wellPumpPin;

	pinMode(wellPumpPin, OUTPUT);
	digitalWrite(wellPumpPin, LOW);
}

void SubWellController::update() {
	if (isActive == false) {
		return;
	}

	if (isBelowCutoffCurrent()) {
		stopWell();
		String message = "SWC STOPPED "
					   + String(this -> id)
					   + " UNDERCURRENT";

		this -> messageQueue -> put(message.c_str());
		return;
	}

	if (!isMeetingMinRiseRate()) {
		stopWell();
		String message = "SWC STOPPED "
					   + String(this -> id)
					   + " NOT MEETING RISE RATE";

		this -> messageQueue -> put(message.c_str());
		return;
	}
}

String SubWellController::startWellCmd(short minRiseRate, float cutoffCurrent) {
	if (millis() < cooldown) {
		return "FAIL SWC SET ACTIVE AWAITING COOLDOWN";
	}

	digitalWrite(this -> wellPumpPin, HIGH);

	this -> isActive = true;
	this -> warmup = millis() + WARMUP_MS;
	this -> cutoffCurrent = cutoffCurrent;

	this -> minRiseRate = minRiseRate;
	this -> lastLevel = this -> tankLevelObserver -> getLevel();
	this -> nextRiseRateSample = millis() + WARMUP_MS + RISE_RATE_SAMPLE_MS; 

	return "ACK SWC SET ACTIVE "
		   + String(this -> id)
		   + " "
		   + String(this -> minRiseRate)
		   + " "
		   + String(this -> cutoffCurrent);
}

void SubWellController::stopWell() {
	digitalWrite(this -> wellPumpPin, LOW);
	this -> cooldown = millis() + COOLDOWN_MS;
	this -> isActive = false;
	this -> consecutiveUndercurrents = 0;
}
