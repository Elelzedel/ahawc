#ifndef T_PUMP_CONTROLLER_HPP
#define T_PUMP_CONTROLLER_HPP

#include <Arduino.h>

#include "../MessageQueue/MessageQueue.hpp"
#include "../TankLevelObserver/TankLevelObserver.hpp"

class TPumpController {
	public:
		TPumpController(
			MessageQueue *messageQueue,
			TankLevelObserver *tankLevelObserver
		);

		void update();

	private: 
		const unsigned int COOLDOWN_MS = 20000;

		const short PUMP_PIN = 5;
		const short PRESSURE_SENSOR_PIN = 10;

		const short CUTOFF_LEVEL = 2;
		const short RE_ENABLE_LEVEL = 4;

		MessageQueue *messageQueue;
		TankLevelObserver *tankLevelObserver;

		bool active = false;
		unsigned long cooldown = 0;

		bool cutoffLevelTriggered = false;

		bool shouldRun();
		bool safeTankLevel();
		bool pressureLow();
		void tryStartPump();
		void stopPump();
};

#endif
