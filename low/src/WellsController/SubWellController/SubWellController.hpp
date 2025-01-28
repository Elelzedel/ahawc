#ifndef SUB_WELL_CONTROLLER_HPP
#define SUB_WELL_CONTROLLER_HPP

#include "../BaseWellController/BaseWellController.hpp"

#include <Arduino.h>

#include "../../MessageQueue/MessageQueue.hpp"

class SubWellController : public BaseWellController {
	public:
		SubWellController(
			short id, 
			MessageQueue *messageQueue,
			CurrentObserver *currentObserver,
			TankLevelObserver *tankLevelObserver,
			short wellPumpPin
		);

		void update();
		String startWellCmd(short minRiseRate, float cutoffCurrent);
		void stopWell();

	private:
		short wellPumpPin = 0;
};

#endif
