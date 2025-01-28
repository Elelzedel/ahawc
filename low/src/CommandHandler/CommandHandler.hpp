#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <Arduino.h>

#include "../MessageQueue/MessageQueue.hpp"
#include "../CurrentObserver/CurrentObserver.hpp"
#include "../TankLevelObserver/TankLevelObserver.hpp"
#include "../WellsController/WellsController.hpp"

class CommandHandler {
	public:
		CommandHandler(
			MessageQueue *messageQueue,
			CurrentObserver *currentObserver,
			TankLevelObserver *tankLevelObserver,
			WellsController *wellsController
		);

		void update();

	private:
		MessageQueue *messageQueue;
		CurrentObserver *currentObserver;
		TankLevelObserver *tankLevelObserver;
		WellsController *wellsController;

};

#endif
