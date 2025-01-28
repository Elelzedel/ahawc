#include "JetWellController.hpp"

JetWellController::JetWellController(
	short id, 
	MessageQueue *messageQueue,
	CurrentObserver *currentObserver,
	TankLevelObserver *tankLevelObserver,
	short wellPumpPin,
	short wellValvePin
) {
	this -> id = id;

	this -> messageQueue = messageQueue;
	this -> currentObserver = currentObserver;
	this -> tankLevelObserver = tankLevelObserver;

	this -> wellPumpPin = wellPumpPin;
	this -> wellValvePin = wellValvePin;

	pinMode(this -> wellPumpPin, OUTPUT);
	pinMode(this -> wellValvePin, OUTPUT);

	digitalWrite(this -> wellPumpPin, LOW);
	digitalWrite(this -> wellValvePin, LOW);
}

void JetWellController::update() {
	if (isActive == false) {
		return;
	}

	if (this -> repressurize) {
		if (millis() >= this -> repressurizeTimer) {
			this -> cooldown = millis() + COOLDOWN_MS;

			digitalWrite(this -> wellPumpPin, LOW);
			this -> repressurize = false;
			this -> isActive = false;
			this -> consecutiveUndercurrents = 0;
		}
		// Skip other checks if repressurizing
		return;
	}

	bool isBelowCutoffCurrent = this -> isBelowCutoffCurrent();
	bool isMeetingMinRiseRate = this -> isMeetingMinRiseRate();
	if (isBelowCutoffCurrent || !isMeetingMinRiseRate) {
		String message;
		if(isBelowCutoffCurrent) {
			message = "JWC STOPPED "
					+ String(this -> id)
					+ " UNDERCURRENT";
		} else {
			message = "JWC STOPPED "
					+ String(this -> id)
					+ " NOT MEETING RISE RATE";
		}

		this -> isActive = false;
		this -> messageQueue -> put(message.c_str());

		// Don't attempt to repressurize, immediately shutoff and disable
		digitalWrite(this -> wellPumpPin, LOW);
		digitalWrite(this -> wellValvePin, LOW);
		this -> disabled = true;
	}
}

String JetWellController::startWellCmd(short minRiseRate, float cutoffCurrent) {
	if (disabled) {
		return "FAIL JWC DISABLED";
	}

	if (millis() < cooldown) {
		return "FAIL JWC SET ACTIVE AWAITING COOLDOWN";
	}

	digitalWrite(this -> wellPumpPin, HIGH);
	digitalWrite(this -> wellValvePin, HIGH);

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

void JetWellController::stopWell() {
	// Already in the process of stopping
	if (this -> repressurize) {
		return;
	}

	digitalWrite(this -> wellValvePin, LOW);
	this -> repressurize = true;
	this -> repressurizeTimer = millis() + REPRESSURIZE_MS;
}
