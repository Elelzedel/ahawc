#include "TankLevelObserver.hpp"

TankLevelObserver::TankLevelObserver() {
	Serial1.begin(9600, SERIAL_8N1);
	this -> level = 0;
}

void TankLevelObserver::update() {
	// Wipe anything older than the 4 newest bytes
	while (Serial1.available() > 4) {
		Serial1.read();
	}

	char distanceASCII[4];

	initTimeoutTime();
	while (Serial1.read() != 'R') {
		if (checkTimeout()) {
			return;
		}
	}

	for(int i = 0; i < 3; i++) {
		while (Serial1.available() <= 0) {
			if (checkTimeout()) {
				return;
			}
		}
		distanceASCII[i] = Serial1.read();
	}
	distanceASCII[4] = '\0';

	int distance = atoi(distanceASCII);

	// Malformed, discard this value
	if (distance <= 0) {
		return;
	}

	if (distance > TANK_BOTTOM_DISTANCE) {
		this -> level = 0;
	} else {
		this -> level = TANK_BOTTOM_DISTANCE - distance;
	}
}

void TankLevelObserver::initTimeoutTime() {
	this -> timeoutTime = millis() + TIMEOUT_MS;
}

bool TankLevelObserver::checkTimeout() {
	if (millis() > this -> timeoutTime) {
		this -> level = LEVEL_TIMEOUT;
		return true;
	}

	return false;
}

String TankLevelObserver::getLevelCmd() {
	return "ACK LO GET " + String(getLevel());
}

short TankLevelObserver::getLevel() {
	return this -> level;
}
