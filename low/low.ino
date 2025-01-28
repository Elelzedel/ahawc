#include "src/CommandHandler/CommandHandler.hpp"
#include "src/MessageQueue/MessageQueue.hpp"
#include "src/CurrentObserver/CurrentObserver.hpp"
#include "src/TankLevelObserver/TankLevelObserver.hpp"
#include "src/TPumpController/TPumpController.hpp"
#include "src/WellsController/WellsController.hpp"

MessageQueue *messageQueue;

CurrentObserver *currentObserver;
TankLevelObserver *tankLevelObserver;

TPumpController *tPumpController;
WellsController *wellsController;

CommandHandler *commandHandler;

void setup() {
	Serial.begin(9600);

	messageQueue = new MessageQueue();

	currentObserver = new CurrentObserver();
	tankLevelObserver = new TankLevelObserver();

	tPumpController = new TPumpController(messageQueue, tankLevelObserver);

	wellsController = new WellsController(
		messageQueue,
		currentObserver,
		tankLevelObserver
	);

	commandHandler = new CommandHandler(
		messageQueue,
		currentObserver,
		tankLevelObserver,
		wellsController
	);
}

void loop() {
	commandHandler -> update();
	tankLevelObserver -> update();
	tPumpController -> update();
	wellsController -> update();
}
