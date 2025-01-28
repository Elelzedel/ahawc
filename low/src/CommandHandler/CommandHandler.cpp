#include "CommandHandler.hpp"

CommandHandler::CommandHandler(
	MessageQueue *messageQueue,
	CurrentObserver *currentObserver,
	TankLevelObserver *tankLevelObserver,
	WellsController *wellsController
) {
	this -> messageQueue = messageQueue;
	this -> currentObserver = currentObserver;
	this -> tankLevelObserver = tankLevelObserver;
	this -> wellsController = wellsController;
}

void CommandHandler::update() {
	if (Serial.available() <= 0) {
		return;
	}

	// Command is in format
	// [SYSTEM ABBREVIATION] [SET]|[GET] [ARGs]
	String cmd = Serial.readString();

	// WC SET ACTIVE [WELL] [RISE RATE] [MINIMUM CURRENT]
	if(cmd.startsWith("WC SET ACTIVE")) {
		short well = cmd[14] - '0';
		short minRiseRate = 0;
		float cutoffCurrent = 0.0;

		if (cmd[14] == '-') {
			well = -1;
		} else {
			minRiseRate = cmd[16] - '0';
			cutoffCurrent = cmd.substring(18).toFloat();
		}

		Serial.println(this -> wellsController -> setActiveWellCmd(well, minRiseRate, cutoffCurrent));
	} else if(cmd.startsWith("WC GET ACTIVE")) {
		Serial.println(this -> wellsController -> getActiveWellCmd());
	} else if(cmd.startsWith("LO GET")) {
		Serial.println(this -> tankLevelObserver -> getLevelCmd());
	} else if(cmd.startsWith("CO GET")) {
		int cmdSensor = cmd[7] - '0';
		Serial.println(this -> currentObserver -> getCurrentCmd(cmdSensor));
	} else if(cmd.startsWith("MQ GET")) {
		Serial.println("ACK MQ GET");
		messageQueue -> serialPrintAll();
		Serial.println("END");
		messageQueue -> clear();
	}
}
