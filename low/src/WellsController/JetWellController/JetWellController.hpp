#ifndef JET_WELL_CONTROLLER_HPP
#define JET_WELL_CONTROLLER_HPP

#include "../BaseWellController/BaseWellController.hpp"

#include <Arduino.h>

#include "../../MessageQueue/MessageQueue.hpp"

class JetWellController : public BaseWellController {
	public:
		JetWellController(
			short id, 
			MessageQueue *messageQueue,
			CurrentObserver *currentObserver,
			TankLevelObserver *tankLevelObserver,
			short wellPumpPin,
			short wellValvePin
		);

		void update();
		String startWellCmd(short minRiseRate, float cutoffCurrent);
		void stopWell();

	private:
		const unsigned long REPRESSURIZE_MS = 4*60*1000UL;
		const unsigned long WARMUP_MS = 2*60*1000UL;
		
		unsigned long repressurizeTimer = 0;

		short wellPumpPin = 0;
		short wellValvePin = 0;

		bool repressurize = false;

		bool disabled = false;
};

#endif
