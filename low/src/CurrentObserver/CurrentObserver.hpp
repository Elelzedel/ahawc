#ifndef CURRENT_OBSERVER_HPP
#define CURRENT_OBSERVER_HPP

#include <Arduino.h>

class CurrentObserver {
	public:
		CurrentObserver();

		float getCurrent(short sensor);
		String getCurrentCmd(short sensor);

	private:
		const short SENSORS_COUNT = 4;
		const short CURR_SENSORS_PINS[4] = {A3, A0, A1, A2};

		const unsigned int SAMPLES = 1024;
		const short OFFSET = 512;
		const double CALIBRATION = 0.149;

		void waitForOffsetCross(short sensor);
};

#endif
