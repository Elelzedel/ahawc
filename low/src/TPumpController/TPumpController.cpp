#include "TPumpController.hpp"

TPumpController::TPumpController(
	MessageQueue *messageQueue,
	TankLevelObserver *tankLevelObserver
) {
	this -> messageQueue = messageQueue;
	this -> tankLevelObserver = tankLevelObserver;

	pinMode(PUMP_PIN, OUTPUT);
	digitalWrite(PUMP_PIN, LOW);

	pinMode(PRESSURE_SENSOR_PIN, INPUT);
	// Use internal pull-up resistor for pressure switch
	digitalWrite(PRESSURE_SENSOR_PIN, HIGH);
}

void TPumpController::update() {
	if (!active) {
		if (shouldRun()) {
			tryStartPump();
		}
	} else {
		if (!shouldRun()) {
			stopPump();
		}
	}
}

bool TPumpController::shouldRun() {
	if (!pressureLow()) {
		return false;
	}

	if (!safeTankLevel()) {
		return false;
	}

	return true;
}

bool TPumpController::safeTankLevel() {
	// If the cutoff was previously triggered, the water has to rise above the
	// RE_ENABLE_LEVEL before the water level is considered safe again
	if (this -> cutoffLevelTriggered) {
		if (this -> tankLevelObserver -> getLevel() <= RE_ENABLE_LEVEL) {
			return false;
		} else {
			this -> messageQueue -> put("PC REENABLED TANK PUMP"); 
			cutoffLevelTriggered = false;
			return true;
		}
	} else {
		if (this -> tankLevelObserver -> getLevel() <= CUTOFF_LEVEL) {
			this -> messageQueue -> put("PC DISABLED TANK PUMP"); 
			cutoffLevelTriggered = true;
			return false;
		} else {
			return true;
		}
	}
}

bool TPumpController::pressureLow() {
	if (digitalRead(PRESSURE_SENSOR_PIN) == LOW)
		return true;
	return false;
}

void TPumpController::tryStartPump() {
	if (millis() < this -> cooldown) {
		return;
	}

	digitalWrite(PUMP_PIN, HIGH);
	this -> active = true;
}

void TPumpController::stopPump() {
	digitalWrite(PUMP_PIN, LOW);
	this -> active = false;
	this -> cooldown = millis() + COOLDOWN_MS; 
}
