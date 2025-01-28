#include "CurrentObserver.hpp"

CurrentObserver::CurrentObserver() {
	for(int i = 0; i < SENSORS_COUNT; i++) {
		pinMode(CURR_SENSORS_PINS[i], INPUT);
	}
}

float CurrentObserver::getCurrent(short sensor) {
	double samplesSum = 0;

	waitForOffsetCross(sensor);

	for (unsigned int i = 0; i < SAMPLES; i++) {
		int sample = analogRead(CURR_SENSORS_PINS[sensor]);

		// Find absolute distance from OFFSET and add it to the sum
		samplesSum += abs(sample - OFFSET);
	}

	// Get average size under sine wave
	double average = samplesSum / double(SAMPLES);

	return float(average * CALIBRATION);
}


String CurrentObserver::getCurrentCmd(short sensor) {
	if (sensor < 0 || sensor >= SENSORS_COUNT) {
		return "FAIL CO GET BAD SENSOR";
	}

	return "ACK CO GET " + String(getCurrent(sensor));
}

void CurrentObserver::waitForOffsetCross(short sensor) {
	bool triggerRisingEdge = false;

	if (analogRead(CURR_SENSORS_PINS[sensor]) < OFFSET) {
		triggerRisingEdge = true;
	}

	for (unsigned int i = 0; i < 1024; i++) {
		if (triggerRisingEdge) {
			if (analogRead(CURR_SENSORS_PINS[sensor]) > OFFSET) {
				return;
			}
		} else {
			if (analogRead(CURR_SENSORS_PINS[sensor]) < OFFSET) {
				return;
			}
		}
	}
}
