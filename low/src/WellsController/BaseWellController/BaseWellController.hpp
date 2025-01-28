#ifndef BASE_WELL_CONTROLLER_HPP
#define BASE_WELL_CONTROLLER_HPP

#include <Arduino.h>

#include "../../MessageQueue/MessageQueue.hpp"
#include "../../CurrentObserver/CurrentObserver.hpp"
#include "../../TankLevelObserver/TankLevelObserver.hpp"

class BaseWellController {
	public:
		virtual void update();
		virtual String startWellCmd(short minRiseRate, float cutoffCurrent);
		virtual void stopWell();
		bool getIsActive();

	protected:
		const unsigned long COOLDOWN_MS = 20000;
		const unsigned long WARMUP_MS = 2000;
		const unsigned long RISE_RATE_SAMPLE_MS = 2*60*1000UL;
		const unsigned long MAX_CONSECUTIVE_UNDERCURRENTS = 20;

		short id = 0;

		// Default to unreasonably high values to be safe
		double cutoffCurrent = 20.0;
		short minRiseRate = 5;
		short lastLevel = 50;

		unsigned long cooldown = 0;
		unsigned long warmup = 0;

		unsigned int consecutiveUndercurrents = 0;

		unsigned long nextRiseRateSample = 0;

		MessageQueue *messageQueue;
		CurrentObserver *currentObserver;
		TankLevelObserver *tankLevelObserver;

		bool isActive = false;

		bool isBelowCutoffCurrent();
		bool isMeetingMinRiseRate();
};

#endif
