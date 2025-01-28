#ifndef TANK_LEVEL_HPP
#define TANK_LEVEL_HPP

#include <Arduino.h>

class TankLevelObserver {
	public:
		static const short LEVEL_TIMEOUT = -1;

		TankLevelObserver();
		void update();
		String getLevelCmd();
		short getLevel();

	private: 
		const short TANK_BOTTOM_DISTANCE = 40;
		const short TIMEOUT_MS = 1000;
		
		short level;

		unsigned long timeoutTime;

		void initTimeoutTime();
		bool checkTimeout();
};

#endif
